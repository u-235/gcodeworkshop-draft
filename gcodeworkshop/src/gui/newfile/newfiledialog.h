/***************************************************************************
 *   Copyright (C) 2006-2018 by Artur Kozioł                               *
 *   artkoz78@gmail.com                                                    *
 *                                                                         *
 *   This file is part of GCodeWorkShop.                                   *
 *                                                                         *
 *   GCodeWorkShop is free software; you can redistribute it and/or modify *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef GUI_NEWFILEDIALOG_H
#define GUI_NEWFILEDIALOG_H

#include <QDialog>  // for QDialog
#include <QDir>     // for QDir
#include <QObject>  // for Q_OBJECT, slots
#include <QString>  // for QString

class QWidget;

namespace Ui {
class NewFileDialog;
}

namespace GUI {
class NewFileDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NewFileDialog(QWidget* parent = 0);
	~NewFileDialog();

	int exec();
	QString getChosenFile();

private:
	Ui::NewFileDialog* ui;

	QDir path;

private slots:
	void fillFileCombo();
	void saveSettings();
	void browseButtonClicked();
};
} // namespace GUI

#endif // GUI_NEWFILEDIALOG_H
