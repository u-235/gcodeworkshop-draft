/*
 *  Copyright (C) 2006-2018 by Artur Kozioł, artkoz78@gmail.com
 *  Copyright (C) 2026 Nick Egorrov, nicegorov@yandex.ru
 *
 *  This file is part of GCodeWorkShop.
 *
 *  GCodeWorkShop is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GUI_FILETABLEPANEL_H
#define GUI_FILETABLEPANEL_H


#include <QList>        // for QList
#include <QObject>      // for slots, Q_OBJECT, signals
#include <QString>      // for QString
#include <QTableWidget> // for QTableWidget

class QWidget;

class Document;
class DocumentManager;


namespace GUI {
class FileTablePanel : public QTableWidget
{
	Q_OBJECT

public:
	explicit FileTablePanel(DocumentManager* docman, QWidget* parent = nullptr);
	~FileTablePanel();

public slots:
	void loadTranslations();
	void loadIcons();

	void updateFileList();
	void updateDocumentInfo(Document* doc);
	void updateDocumentModifity(Document* doc);
	void updateActiveDocument(Document* doc);

protected:
	void updateDocumentInfoById(int id);
	void updateDocumentModifityById(int id);
	int documentIndex(Document* doc);

protected slots:
	void tableWidgetClicked(int x, int y);

private:
	DocumentManager* m_documentManager;
	QList<Document*> m_documentList;
};
} // namespace GUI

#endif // GUI_FILETABLEPANEL_H
