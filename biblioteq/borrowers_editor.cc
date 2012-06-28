#include "borrowers_editor.h"

extern qtbook *qmain;
extern QApplication *qapp;

/*
** -- borrowers_editor() --
*/

borrowers_editor::borrowers_editor(QWidget *parent,
				   qtbook_item *bitemArg,
				   const int quantityArg,
				   const QString &ioidArg,
				   const QString &uniqueidArg,
				   const QFont &font,
				   const QString &itemTypeArg,
				   const int stateArg):
  QDialog(parent)
{
  setWindowModality(Qt::WindowModal);
  bd.setupUi(this);
#ifdef Q_WS_MAC
  setAttribute(Qt::WA_MacMetalStyle, true);
#endif
  ioid = ioidArg;
  quantity = quantityArg;
  bitem = bitemArg;
  state = stateArg;
  itemType = itemTypeArg;

  /*
  ** Override the state, if necessary.
  */

  if(qmain->getRoles().contains("administrator") ||
     qmain->getRoles().contains("circulation"))
    state = qtbook::EDITABLE;
  else if(qmain->getRoles().contains("librarian"))
    state = qtbook::VIEW_ONLY;

  if(state == qtbook::EDITABLE)
    {
      connect(bd.saveButton, SIGNAL(clicked(void)), this,
	      SLOT(slotSave(void)));
      connect(bd.eraseButton, SIGNAL(clicked(void)), this,
	      SLOT(slotEraseBorrower(void)));
    }
  else
    {
      bd.saveButton->setVisible(false);
      bd.eraseButton->setVisible(false);
    }

  connect(bd.cancelButton, SIGNAL(clicked(void)), this,
	  SLOT(slotCloseCurrentBorrowers(void)));
  bd.table->verticalHeader()->setResizeMode(QHeaderView::Fixed);
  setGlobalFonts(font);

  if(!uniqueidArg.isEmpty())
    setWindowTitle
      (QString(tr("BiblioteQ: Item Reservation Status (")) + uniqueidArg +
       QString(tr(")")));
  else
    setWindowTitle(tr("BiblioteQ: Item Reservation Status"));
}

/*
** -- ~borrowers_editor() --
*/

borrowers_editor::~borrowers_editor()
{
}

/*
** -- showUsers() --
*/

void borrowers_editor::showUsers(void)
{
  int i = 0;
  int j = 0;
  int row = 0;
  bool terminate = false;
  QString str = "";
  QString tmpStr = "";
  QString querystr = "";
  QDateEdit *dateEdit = 0;
  QSqlQuery query(qmain->getDB());
  QStringList list;
  QTableWidgetItem *item = 0;
  QProgressDialog progress1(this);
  QProgressDialog progress2(this);

#ifdef Q_WS_MAC
  progress1.setAttribute(Qt::WA_MacMetalStyle, true);
  progress2.setAttribute(Qt::WA_MacMetalStyle, true);
#endif
  bd.table->clear();
  bd.table->setCurrentItem(0);
  bd.table->setColumnCount(0);
  bd.table->setRowCount(0);

  if(state == qtbook::EDITABLE)
    {
      list.append(tr("Copy Number"));
      list.append(tr("Barcode"));
      list.append(tr("Member ID"));
      list.append(tr("First Name"));
      list.append(tr("Last Name"));
      list.append(tr("Reservation Date"));
      list.append(tr("Copy Due Date"));
      list.append(tr("Lender"));
      list.append("MYOID");
    }
  else
    {
      list.append(tr("Copy Number"));
      list.append(tr("Barcode"));
      list.append(tr("Reservation Date"));
      list.append(tr("Copy Due Date"));
    }

  bd.table->setColumnCount(list.size());
  bd.table->setHorizontalHeaderLabels(list);
  list.clear();
  bd.table->setRowCount(quantity);
  bd.table->scrollToTop();
  bd.table->horizontalScrollBar()->setValue(0);

  /*
  ** Hide the OID column.
  */

  if(state == qtbook::EDITABLE)
    bd.table->setColumnHidden(bd.table->columnCount() - 1, true);

  if(!isVisible())
    updateGeometry();

  show();
  progress1.setModal(true);
  progress1.setWindowTitle(tr("BiblioteQ: Progress Dialog"));
  progress1.setLabelText(tr("Constructing objects..."));
  progress1.setMaximum(quantity);
  progress1.setMinimum(0);
  progress1.show();
  progress1.update();

  for(i = 0; i < quantity && !progress1.wasCanceled(); i++)
    {
      for(j = 0; j < bd.table->columnCount(); j++)
	if(j == 6 && state == qtbook::EDITABLE)
	  {
	    if((dateEdit = new(std::nothrow) QDateEdit()) != 0)
	      {
		bd.table->setCellWidget(i, j, dateEdit);
		dateEdit->setDate
		  (QDate::fromString("01/01/2000", "MM/dd/yyyy"));
		dateEdit->setCalendarPopup(true);
		dateEdit->setEnabled(false);
	      }
	    else
	      qmain->addError(QString(tr("Memory Error")),
			      QString(tr("Unable to allocate memory for the "
					 "\"dateEdit\" object. "
					 "This is a serious problem!")),
			      QString(""), __FILE__, __LINE__);
	  }
	else if((item = new(std::nothrow) QTableWidgetItem()) != 0)
	  {
	    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	    if(j == 0)
	      {
		tmpStr.setNum(i + 1);
		item->setText(tmpStr);
	      }

	    bd.table->setItem(i, j, item);
	  }
	else
	  qmain->addError
	    (QString(tr("Memory Error")),
	     QString(tr("Unable to allocate memory for the \"item\" "
			"object. "
			"This is a serious problem!")),
	     QString(""), __FILE__, __LINE__);

      if(i + 1 <= progress1.maximum())
	progress1.setValue(i + 1);

      progress1.update();
#ifndef Q_WS_MAC
      qapp->processEvents();
#endif
    }

  progress1.hide();
  bd.table->setRowCount(i);

  if(state == qtbook::EDITABLE)
    querystr = QString
      ("SELECT borrowers.copy_number, "
       "borrowers.copyid, "
       "member.memberid, "
       "member.first_name, "
       "member.last_name, "
       "borrowers.reserved_date, "
       "borrowers.duedate, "
       "borrowers.reserved_by, "
       "borrowers.myoid "
       "FROM member member, "
       "item_borrower borrowers "
       "WHERE borrowers.type = '%1' AND borrowers.item_oid = %2 AND "
       "borrowers.memberid = member.memberid "
       "ORDER BY borrowers.copy_number").arg(itemType).arg(ioid);
  else
    querystr = QString
      ("SELECT borrowers.copy_number, "
       "borrowers.copyid, "
       "borrowers.reserved_date, "
       "borrowers.duedate "
       "FROM "
       "item_borrower_vw borrowers "
       "WHERE borrowers.type = '%1' AND borrowers.item_oid = %2 "
       "ORDER BY borrowers.copy_number").arg(itemType).arg(ioid);

  qapp->setOverrideCursor(Qt::WaitCursor);

  if(!query.exec(querystr))
    qmain->addError(QString(tr("Database Error")),
		    QString(tr("Unable to retrieve borrower data.")),
		    query.lastError().text(), __FILE__, __LINE__);

  qapp->restoreOverrideCursor();
  progress2.setModal(true);
  progress2.setWindowTitle(tr("BiblioteQ: Progress Dialog"));
  progress2.setLabelText(tr("Retrieving borrower data..."));
  progress2.setMinimum(0);

  /*
  ** SQLite does not support query.size().
  */

  if(qmain->getDB().driverName() == "QSQLITE")
    progress2.setMaximum(misc_functions::sqliteQuerySize(querystr,
							 qmain->getDB(),
							 __FILE__,
							 __LINE__));
  else
    progress2.setMaximum(query.size());

  progress2.show();
  progress2.update();
  i = -1;

  QDate date;
  QDate tomorrow(QDate::currentDate());

  tomorrow = tomorrow.addDays(1);

  while(i++, !progress2.wasCanceled() && query.next())
    {
      if(query.isValid())
	for(j = 0; j < bd.table->columnCount(); j++)
	  {
	    if(query.value(0).isNull())
	      row = i;
	    else
	      row = query.value(0).toInt() - 1;

	    if(j == 0 && query.value(0).isNull())
	      str = QString::number(i + 1);
	    else if(j == 5 && state == qtbook::EDITABLE)
	      {
		date = QDate::fromString(query.value(j).toString(),
					 "MM/dd/yyyy");
		str = date.toString(Qt::SystemLocaleShortDate);
	      }
	    else if((j == 2 || j == 3) && state != qtbook::EDITABLE)
	      {
		date = QDate::fromString(query.value(j).toString(),
					 "MM/dd/yyyy");
		str = date.toString(Qt::SystemLocaleShortDate);
	      }
	    else
	      str = query.value(j).toString();

	    if(j == 6 && state == qtbook::EDITABLE)
	      {
		if(bd.table->cellWidget(row, j) != 0)
		  {
		    static_cast<QDateEdit *> (bd.table->cellWidget
					      (row, j))->setDate
		      (QDate::fromString(str, "MM/dd/yyyy"));
		    static_cast<QDateEdit *> (bd.table->cellWidget
					      (row, j))->setMinimumDate
		      (tomorrow);
		    static_cast<QDateEdit *> (bd.table->cellWidget
					      (row, j))->setEnabled
		      (true);
		  }
	      }
	    else if(bd.table->item(row, j) != 0)
	      bd.table->item(row, j)->setText(str);
	    else
	      terminate = true;
	  }

      if(i + 1 <= progress2.maximum())
	progress2.setValue(i + 1);

      progress2.update();
#ifndef Q_WS_MAC
      qapp->processEvents();
#endif
      if(terminate)
	break; // Out of resources?
    }

  progress2.hide();
  bd.table->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

/*
** -- slotEraseBorrower() --
*/

void borrowers_editor::slotEraseBorrower(void)
{
  int row = bd.table->currentRow();
  QString oid = "";
  QString copyid = "";
  QString errorstr = "";
  QString memberid = "";
  QString availability = "";
  QString returnedDate = QDateTime::currentDateTime().toString("MM/dd/yyyy");
  QSqlQuery query(qmain->getDB());

  if(row < 0)
    {
      QMessageBox::critical
	(this, tr("BiblioteQ: User Error"),
	 tr("Please select the copy that has been returned."));
      return;
    }

  oid = misc_functions::getColumnString(bd.table, row, "MYOID");

  if(oid.isEmpty())
    {
      QMessageBox::critical(this, tr("BiblioteQ: User Error"),
			    tr("It appears that the selected "
			       "item has not been reserved."));
      return;
    }

  if(QMessageBox::question(this, tr("BiblioteQ: Question"),
			   tr("Are you sure that the copy has been returned?"),
			   QMessageBox::Yes | QMessageBox::No,
			   QMessageBox::No) == QMessageBox::No)
    return;

  query.prepare(QString("DELETE FROM item_borrower WHERE "
			"myoid = ? AND type = '%1'").arg(itemType));
  query.bindValue(0, oid);
  qapp->setOverrideCursor(Qt::WaitCursor);

  if(!query.exec())
    {
      qapp->restoreOverrideCursor();
      qmain->addError(QString(tr("Database Error")),
		      QString(tr("Unable to modify the reservation status of "
				 "the selected copy.")),
		      query.lastError().text(), __FILE__, __LINE__);
      QMessageBox::critical(this, tr("BiblioteQ: Database Error"),
			    tr("Unable to modify the reservation status of "
			       "the selected copy."));
      return;
    }
  else
    {
      /*
      ** Record the return in the history table.
      */

      copyid = misc_functions::getColumnString(bd.table, row,
					       tr("Barcode"));
      memberid = misc_functions::getColumnString(bd.table, row,
						 tr("Member ID"));
      query.prepare("UPDATE member_history SET returned_date = ? "
		    "WHERE item_oid = ? AND copyid = ? AND "
		    "memberid = ?");
      query.bindValue(0, returnedDate);
      query.bindValue(1, ioid);
      query.bindValue(2, copyid);
      query.bindValue(3, memberid);

      if(!query.exec())
	qmain->addError(QString(tr("Database Error")),
			QString(tr("Unable to modify the returned date of "
				   "the selected copy.")),
			query.lastError().text(), __FILE__, __LINE__);

      qapp->restoreOverrideCursor();
      qmain->updateMembersBrowser(memberid);

      /*
	qapp->setOverrideCursor(Qt::WaitCursor);
	availability = misc_functions::getAvailability(ioid, qmain->getDB(),
	itemType, errorstr);
	qapp->restoreOverrideCursor();

	if(!errorstr.isEmpty())
	qmain->addError(QString(tr("Database Error")),
	QString(tr("Retrieving availability.")),
	errorstr, __FILE__, __LINE__);

	misc_functions::updateColumn(qmain->getUI().table, bitem->getRow(),
	tr("Availability"), availability);
	misc_functions::updateColumn(qmain->getUI().table, bitem->getRow(),
	tr("Due Date"), "Returned");
      */

      /*
      ** Update the Reservation History panel, if necessary.
      */

      qmain->updateReservationHistoryBrowser(memberid, ioid, copyid,
					     itemType, returnedDate);
      showUsers();

      /*
      ** Update the main window's summary panel, if necessary.
      */

      if(ioid == misc_functions::getColumnString(qmain->getUI().table,
						 bitem->getRow(), "MYOID"))
	qmain->slotDisplaySummary();
    }
}

/*
** -- slotCloseCurrentBorrowers() --
*/

void borrowers_editor::slotCloseCurrentBorrowers(void)
{
  deleteLater();
}

/*
** -- slotSave() --
*/

void borrowers_editor::slotSave(void)
{
  int i = 0;
  bool error = false;
  QDate now = QDate::currentDate();
  QString oid = "";
  QDateEdit *dueDate = 0;
  QSqlQuery query(qmain->getDB());
  QProgressDialog progress(this);

#ifdef Q_WS_MAC
  progress.setAttribute(Qt::WA_MacMetalStyle, true);
#endif
  progress.setModal(true);
  progress.setWindowTitle(tr("BiblioteQ: Progress Dialog"));
  progress.setLabelText(tr("Updating the due date(s)..."));
  progress.setMaximum(bd.table->rowCount());
  progress.setMinimum(0);
  progress.show();
  progress.update();

  for(i = 0; i < bd.table->rowCount(); i++)
    {
      oid = misc_functions::getColumnString(bd.table, i, "MYOID");

      if(!oid.isEmpty())
	{
	  dueDate = static_cast<QDateEdit *> (bd.table->cellWidget(i, 6));

	  if(dueDate->date() <= now)
	    error = true;
	  else
	    {
	      query.prepare("UPDATE item_borrower "
			    "SET duedate = ? "
			    "WHERE myoid = ? AND "
			    "type = ?");
	      query.bindValue(0, dueDate->date().toString("MM/dd/yyyy"));
	      query.bindValue(1, oid);
	      query.bindValue(2, itemType);

	      if(!query.exec())
		qmain->addError(QString(tr("Database Error")),
				QString(tr("Unable to update the due date.")),
				query.lastError().text(), __FILE__,
				__LINE__);
	      else
		{
		  /*
		    misc_functions::updateColumn
		    (qmain->getUI().table,
		    bitem->getRow(), tr("Due Date"),
		    dueDate->date().toString(Qt::SystemLocaleShortDate));
		  */
		}
	    }
	}

      if(i + 1 <= progress.maximum())
	progress.setValue(i + 1);

      progress.update();
#ifndef Q_WS_MAC
      qapp->processEvents();
#endif
    }

  progress.hide();

  if(error)
    QMessageBox::critical(this, tr("BiblioteQ: User Error"),
			  tr("Some or all of the Due Dates were not updated "
			     "because of invalid dates."));

  if(query.lastError().isValid())
    QMessageBox::critical(this, tr("BiblioteQ: Database Error"),
			  tr("Some or all of the Due Dates were not updated "
			     "because of database errors."));
}

/*
** -- keyPressEvent() --
*/

void borrowers_editor::keyPressEvent(QKeyEvent *event)
{
  if(event && event->key() == Qt::Key_Escape)
    slotCloseCurrentBorrowers();

  QDialog::keyPressEvent(event);
}

/*
** -- closeEvent() --
*/

void borrowers_editor::closeEvent(QCloseEvent *e)
{
  slotCloseCurrentBorrowers();
  QDialog::closeEvent(e);
}

/*
** -- setGlobalFonts() --
*/

void borrowers_editor::setGlobalFonts(const QFont &font)
{
  setFont(font);

  foreach(QWidget *widget, findChildren<QWidget *>())
    widget->setFont(font);
}
