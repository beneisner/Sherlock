/*
** -- Local Includes --
*/

#include "qtbook.h"
#include "qtbook_book.h"
#include "hyperlinked_text_edit.h"

extern qtbook *qmain;

/*
** -- hyperlinked_text_edit() --
*/

hyperlinked_text_edit::hyperlinked_text_edit(QWidget *parent)
{
  Q_UNUSED(parent);
  connect(this, SIGNAL(anchorClicked(const QUrl &)),
	  this, SLOT(slotAnchorClicked(const QUrl &)));
}

/*
** -- slotAnchorClicked() --
*/

void hyperlinked_text_edit::slotAnchorClicked(const QUrl &url)
{
  QString path = url.toString();
  QString searchKey = "";
  QString searchType = "";
  QString searchValue = "";
  QStringList tmplist;

  tmplist = path.split("?");

  if(tmplist.size() >= 3)
    {
      searchType = tmplist[0];
      searchKey = tmplist[1];
      searchValue = tmplist[2];

      if(searchType == "book_search")
	qmain->bookSearch(searchKey, searchValue);
      else if(searchType == "cd_search")
	qmain->cdSearch(searchKey, searchValue);
      else if(searchType == "dvd_search")
	qmain->dvdSearch(searchKey, searchValue);
      else if(searchType == "journal_search")
	qmain->journSearch(searchKey, searchValue);
      else if(searchType == "magazine_search")
	qmain->magSearch(searchKey, searchValue);
      else if(searchType == "videogame_search")
	qmain->vgSearch(searchKey, searchValue);
    }

  tmplist.clear();
}

/*
** -- setMultipleLinks() --
*/

void hyperlinked_text_edit::setMultipleLinks(const QString &searchType,
					     const QString &searchField,
					     const QString &str)
{
  int i = 0;
  QString html = "";
  QStringList tmplist;

  tmplist = str.split("\n");

  for(i = 0; i < tmplist.size(); i++)
    {
      html += QString
	("<a href=\"%1?%2?%3\">" + tmplist[i] + "</a>").arg
	(searchType).arg(searchField).arg(tmplist[i]);

      if(i != tmplist.size() - 1)
	html += "<br>";
    }

  setText(html);
}
