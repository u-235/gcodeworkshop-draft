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

#include <QApplication>             // for QApplication, qApp
#include <QCheckBox>                // for QCheckBox
#include <QColor>                   // for QColor
#include <QComboBox>                // for QComboBox
#include <QDateTime>                // for QDateTime
#include <QDir>                     // for QDir, operator|, QDir::NoSymLinks, QDir::Readable, QDir::AllDirs, QDir::F...
#include <QFile>                    // for QFile
#include <QFileDialog>              // for QFileDialog
#include <QFileInfo>                // for QFileInfo
#include <QFont>                    // for QFont, QFont::Normal
#include <QFrame>                   // for QFrame
#include <QIODevice>                // for QIODevice, QIODevice::ReadOnly
#include <QPlainTextEdit>           // for QPlainTextEdit
#include <QProgressDialog>          // for QProgressDialog
#include <QPushButton>              // for QPushButton
#include <QRegularExpression>       // for QRegularExpression, QRegularExpression::CaseInsensitiveOption
#include <QRegularExpressionMatch>  // for QRegularExpressionMatch
#include <QSettings>                // for QSettings
#include <QSplitter>                // for QSplitter
#include <QString>                  // for QString, operator+, QCharRef
#include <QStringList>              // for QStringList
#include <QTableWidget>             // for QTableWidget
#include <QTableWidgetItem>         // for QTableWidgetItem
#include <QTextBlock>               // for QTextBlock
#include <QTextCharFormat>          // for QTextCharFormat
#include <QTextCursor>              // for QTextCursor
#include <QTextDocument>            // for QTextDocument, QTextDocument::FindFlags, QTextDocument::FindWholeWords
#include <QTextOption>              // for QTextOption, QTextOption::NoWrap
#include <QTextStream>              // for QTextStream
#include <QToolButton>              // for QToolButton
#include <QVariant>                 // for QVariant
#include <Qt>                       // for operator|, AlignVCenter, KeyboardModifiers, AlignLeft, NoModifier, AlignR...
#include <QtGlobal>                 // for QFlags, qMakeForeachContainer, Q_UNUSED, foreach, qint64

#include <gcoderstyle.h>    // for HighlightColors
#include <utils/medium.h>   // for Medium

#include "capslockeventfilter.h"
#include "findinfilespanel.h"
#include "highlighter.h"        //  for Highlighter, autoDetectHighligthMode

#include "ui_findinfilespanel.h"     // for Ui::FindInFilesPanel


#define MAXLISTS        20

GUI::FindInFilesPanel::FindInFilesPanel(QSplitter* parent):
	QWidget(parent),
	ui(new Ui::FindInFilesPanel())
{
	f_parent = parent;
	ui->setupUi(this);
	GUI::FindInFilesPanel::loadIcons();
	setAttribute(Qt::WA_DeleteOnClose);
	setObjectName("FindInFiles");

	highlighter = nullptr;
	highlight = false;

	connect(ui->browseButton, SIGNAL(clicked()), SLOT(browse()));
	connect(ui->findButton, SIGNAL(clicked()), SLOT(find()));
	connect(ui->hideToolButton, SIGNAL(clicked()), SLOT(hideDlg()));
	//connect(closeToolButton, SIGNAL(clicked()), SLOT(close()));

	createFilesTable();

	m_textComboBoxEventFilter = new CapsLockEventFilter(ui->textComboBox);
	ui->textComboBox->installEventFilter(m_textComboBoxEventFilter);

	ui->preview->setReadOnly(true);
	ui->preview->setWordWrapMode(QTextOption::NoWrap);
	ui->preview->setFont(QFont("Courier", 12, QFont::Normal));
}

GUI::FindInFilesPanel::~FindInFilesPanel()
{
	delete ui;
}

void GUI::FindInFilesPanel::setCapsLockEnable(bool enable)
{
	m_textComboBoxEventFilter->setCapsLockEnable(enable);
}

void GUI::FindInFilesPanel::loadTranslations()
{
	ui->retranslateUi(this);
}

void GUI::FindInFilesPanel::loadIcons()
{
	QIcon icon1;
	icon1.addFile(":/images/browse.png", QSize(), QIcon::Normal, QIcon::Off);
	ui->browseButton->setIcon(icon1);
	QIcon icon2;
	icon2.addFile(":/images/findnext.png", QSize(), QIcon::Normal, QIcon::Off);
	ui->findButton->setIcon(icon2);
	QIcon icon3;
	icon3.addFile(":/images/filefind.png", QSize(), QIcon::Normal, QIcon::Off);
	icon3.addFile(":/images/filefind.png", QSize(), QIcon::Disabled, QIcon::Off);
	ui->title->setIcon(icon3);
	QIcon icon4;
	icon4.addFile(":/images/arrow-down.png", QSize(), QIcon::Normal, QIcon::Off);
	icon4.addFile(":/images/arrow-up.png", QSize(), QIcon::Normal, QIcon::On);
	ui->hideToolButton->setIcon(icon4);
}

void GUI::FindInFilesPanel::loadSettings(QSettings* cfg)
{
	QStringList list;
	QString item;
	int i;

	ui->textComboBox->clear();
	ui->directoryComboBox->clear();
	ui->fileComboBox->clear();

	list = cfg->value("Extensions", "").toStringList();

	cfg->beginGroup("FindFileDialog");

	list.append(cfg->value("Filters", "*.nc").toStringList());
	list.removeDuplicates();
	list.sort();
	ui->fileComboBox->addItems(list);
	item = cfg->value("SelectedFilter", QString("*.nc")).toString();
	i = ui->fileComboBox->findText(item);
	ui->fileComboBox->setCurrentIndex(i);

	ui->wholeWordsCheckBox->setChecked(cfg->value("WholeWords", false).toBool());
	ui->subFoldersCheckBox->setChecked(cfg->value("SubFolders", false).toBool());
	ui->commentStyle1CheckBox->setChecked(cfg->value("CommentStyle1", false).toBool());
	ui->commentStyle2CheckBox->setChecked(cfg->value("CommentStyle2", false).toBool());

	list = cfg->value("Dirs", QStringList(QDir::homePath())).toStringList();
	list.removeDuplicates();
	list.sort();
	ui->directoryComboBox->addItems(list);
	item = cfg->value("SelectedDir", QDir::toNativeSeparators(QDir::homePath())).toString();
	i = ui->directoryComboBox->findText(item);
	ui->directoryComboBox->setCurrentIndex(i);

	list = cfg->value("Texts", QStringList()).toStringList();
	list.removeDuplicates();
	list.sort();
	ui->textComboBox->addItems(list);
	item = cfg->value("SelectedText", QString("*")).toString();
	i = ui->textComboBox->findText(item, Qt::MatchExactly);
	ui->textComboBox->setCurrentIndex(i);

	cfg->endGroup();
}

void GUI::FindInFilesPanel::saveSettings(QSettings* cfg) const
{
	QStringList list;
	QString item;

	cfg->beginGroup("FindFileDialog");

	cfg->setValue("WholeWords", ui->wholeWordsCheckBox->isChecked());
	cfg->setValue("SubFolders", ui->subFoldersCheckBox->isChecked());
	cfg->setValue("CommentStyle1", ui->commentStyle1CheckBox->isChecked());
	cfg->setValue("CommentStyle2", ui->commentStyle2CheckBox->isChecked());

	list.clear();
	list.append(ui->directoryComboBox->currentText());

	for (int i = 0; i <= ui->directoryComboBox->count(); i++) {
		item = ui->directoryComboBox->itemText(i);

		if (!item.isEmpty())
			if (!list.contains(item)) {
				list.append(item);
			}
	}

	while (list.size() > MAXLISTS) {
		list.removeLast();
	}

	cfg->setValue("Dirs", list);
	cfg->setValue("SelectedDir", ui->directoryComboBox->currentText());

	list.clear();
	list.append(ui->fileComboBox->currentText());

	for (int i = 0; i <= ui->fileComboBox->count(); i++) {
		item = ui->fileComboBox->itemText(i);

		if (!item.isEmpty())
			if (!list.contains(item)) {
				list.append(item);
			}
	}

	while (list.size() > MAXLISTS) {
		list.removeLast();
	}

	cfg->setValue("Filters", list);
	cfg->setValue("SelectedFilter", ui->fileComboBox->currentText());

	list.clear();
	list.append(ui->textComboBox->currentText());

	for (int i = 0; i <= ui->textComboBox->count(); i++) {
		item = ui->textComboBox->itemText(i);

		if (!item.isEmpty())
			if (!list.contains(item, Qt::CaseInsensitive)) {
				list.append(item);
			}
	}

	while (list.size() > MAXLISTS) {
		list.removeLast();
	}

	cfg->setValue("Texts", list);
	cfg->setValue("SelectedText", ui->textComboBox->currentText());

	cfg->endGroup();
}

void GUI::FindInFilesPanel::closeDialog()
{
	setAttribute(Qt::WA_DeleteOnClose);
	close();
}

void GUI::FindInFilesPanel::hideDlg()
{
	QList<int> list;
	list = f_parent->sizes();
	int id = f_parent->indexOf(this);

	f_parent->setUpdatesEnabled(false);

	if (ui->hideToolButton->isChecked()) {
		currentHeight = list;
		list[id] = 18;
		list[0] = list[0] + (currentHeight[id] - list[id]);
		ui->splitter->hide();
		ui->frame->hide();
		ui->hideToolButton->setChecked(true);
	} else {
		list = currentHeight;
		ui->splitter->show();
		ui->frame->show();
		ui->hideToolButton->setChecked(false);
	}

	//qApp->processEvents();
	f_parent->setSizes(list);
	f_parent->updateGeometry();
	f_parent->setUpdatesEnabled(true);
}

void GUI::FindInFilesPanel::browse()
{
	QString directory = QFileDialog::getExistingDirectory(this, tr("Find Files"),
	                    ui->directoryComboBox->currentText());

	if (!directory.isEmpty()) {
		ui->directoryComboBox->addItem(QDir::toNativeSeparators(directory));
		ui->directoryComboBox->setCurrentIndex(ui->directoryComboBox->findText(QDir::toNativeSeparators(
		        directory)));
	}
}

void GUI::FindInFilesPanel::find()
{
	;
	bool notFound;

	QString text = ui->textComboBox->currentText();

	if (text.isEmpty()) {
		return;
	}

	ui->filesTable->setRowCount(0);
	ui->preview->clear();

	QString fileName = ui->fileComboBox->currentText();
	QString path = QDir(ui->directoryComboBox->currentText()).absolutePath();

	ui->findButton->setEnabled(false);
	QApplication::setOverrideCursor(Qt::BusyCursor);
	qApp->processEvents();

	if (fileName.isEmpty()) {
		fileName = "*";
	}

	QProgressDialog* progressDialog = new QProgressDialog(this);
	progressDialog->setCancelButtonText(tr("&Cancel"));
	progressDialog->setRange(0, 100);
	progressDialog->setWindowTitle(tr("Find Files"));
	//progressDialog->setLabelText(tr("Searching in folder: \"%1\"").arg(QDir(path).absolutePath()));
	qApp->processEvents();

	notFound = findFiles(path, path, true, text, fileName, progressDialog);

	progressDialog->close();

	if (notFound) {
		QTableWidgetItem* fileNameItem = new QTableWidgetItem(tr("No"));
		fileNameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		QTableWidgetItem* infoNameItem = new QTableWidgetItem(tr("files"));
		infoNameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		QTableWidgetItem* sizeItem = new QTableWidgetItem(tr("found."));
		sizeItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		ui->filesTable->insertRow(0);
		ui->filesTable->setItem(0, 0, fileNameItem);
		ui->filesTable->setItem(0, 1, infoNameItem);
		ui->filesTable->setItem(0, 2, sizeItem);
	}

	ui->filesTable->resizeRowsToContents();
	ui->filesTable->resizeColumnsToContents();

	ui->findButton->setEnabled(true);
	QApplication::restoreOverrideCursor();
}

bool GUI::FindInFilesPanel::findFiles(const QString startDir, QString mainDir, bool notFound,
                                      const QString findText, QString fileFilter, QProgressDialog* progressDialog)
{
	int pos;
	QRegularExpression regex;
	QString comment_tx;
	qint64 size;
	bool textFounded, word;
	QString line;
	QStringList files;
	QStringList dirs;
	bool inComment = false;
	int commentPos;

	if (progressDialog->wasCanceled()) {
		return notFound;
	}

	regex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
	regex.setPattern("\\([^\\n\\r]*\\)|;[^\\n\\r]*");

	pos = 0;

	dirs.clear();

	QDir directory = QDir(startDir);
	//qDebug() << startDir << directory.absolutePath();

	if (ui->subFoldersCheckBox->isChecked()) {
		dirs.append(directory.entryList(QStringList("*"),
		                                QDir::AllDirs | QDir::NoSymLinks | QDir::Readable | QDir::NoDotAndDotDot));
	}

	foreach (const QString& dirName, dirs) {
		notFound = findFiles(directory.absolutePath() + "/" + dirName, mainDir, notFound, findText,
		                     fileFilter, progressDialog);
	}

	files = directory.entryList(QStringList(fileFilter),
	                            QDir::Files | QDir::NoSymLinks | QDir::Readable);

	progressDialog->setLabelText(tr("Searching in folder: \"%1\"").arg(QDir::toNativeSeparators(
	                                 directory.absolutePath())));

	for (int i = 0; i < files.size(); ++i) {
		progressDialog->setRange(0, files.size());
		progressDialog->setValue(i);
		qApp->processEvents();

		if (progressDialog->wasCanceled()) {
			break;
		}

		QFile file(directory.absoluteFilePath(files[i]));

		if (file.open(QIODevice::ReadOnly)) {
			QTextStream in(&file);

			textFounded = false;
			word = false;
			line = in.readAll();

			if (findText == "*") { //files containing anything
				textFounded = true;
			} else {
				commentPos = -1;
				pos = 0;

				do {
					pos = line.indexOf(findText, pos, Qt::CaseInsensitive);
					textFounded = (pos >= 0);

					if (textFounded && (ui->commentStyle1CheckBox->isChecked() || ui->commentStyle2CheckBox->isChecked())) {
						int lineStartPos = line.lastIndexOf('\n', pos);

						if (lineStartPos < 0) {
							lineStartPos = pos;
						}

						if (ui->commentStyle1CheckBox->isChecked() && ui->commentStyle2CheckBox->isChecked()) {
							commentPos  = line.indexOf('(', lineStartPos);

							if (commentPos > pos) {
								commentPos = -1;
							}

							if (commentPos < 0) {
								commentPos  = line.indexOf(';', lineStartPos);
							}
						} else {
							if (ui->commentStyle2CheckBox->isChecked()) {
								commentPos  = line.indexOf('(', lineStartPos);
							}

							if (ui->commentStyle1CheckBox->isChecked()) {
								commentPos  = line.indexOf(';', lineStartPos);
							}
						}

						if (commentPos < 0) {
							commentPos = pos + 1;
						}

						inComment = (commentPos < pos);

					} else {
						inComment = false;
					}

					pos++;

				} while (inComment);
			}

			pos--;

			if (textFounded && ui->wholeWordsCheckBox->isChecked()) {
				if (pos > 0)
					if (line[pos - 1].isLetterOrNumber()) {
						word = true;
					}

				pos = pos + findText.size();

				if (pos < line.size())
					if (line[pos].isLetterOrNumber()) {
						word = true;
					}
			}

			size = file.size();

			if ((textFounded && (!ui->wholeWordsCheckBox->isChecked())) ||
			        (textFounded && (ui->wholeWordsCheckBox->isChecked() && !word))) {
				notFound = false;
				textFounded = false;
				word = false;

				comment_tx.clear();
				auto match = regex.match(line);

				while (match.hasMatch()) {
					comment_tx = match.captured();

					if (!comment_tx.contains(";$")) {
						comment_tx.remove('(');
						comment_tx.remove(')');
						comment_tx.remove(';');
						break;
					}

					match = regex.match(line, match.capturedEnd());
				}

				QString subDir = startDir;
				subDir.remove(mainDir);

				if (!subDir.isEmpty()) {
					subDir += "/";
				}

				QTableWidgetItem* fileNameItem = new QTableWidgetItem(QDir::toNativeSeparators(
				            subDir) + files[i]);
				fileNameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

				QTableWidgetItem* infoNameItem = new QTableWidgetItem(comment_tx);
				infoNameItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

				QTableWidgetItem* sizeItem = new QTableWidgetItem(tr("%1 KB").arg(int((size + 1023) / 1024)));
				sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

				//QTableWidgetItem *dateItem = new QTableWidgetItem(QFileInfo(file).lastModified().toString(Qt::SystemLocaleShortDate));
				//dateItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
				QTableWidgetItem* dateItem = new QTableWidgetItem();
				dateItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
				dateItem->setData(Qt::DisplayRole, QFileInfo(file).lastModified());


				int row = ui->filesTable->rowCount();
				ui->filesTable->insertRow(row);
				ui->filesTable->setItem(row, 0, fileNameItem);
				ui->filesTable->setItem(row, 1, infoNameItem);
				ui->filesTable->setItem(row, 2, sizeItem);
				ui->filesTable->setItem(row, 3, dateItem);
			}

			file.close();
		}
	}

	return notFound;
}

void GUI::FindInFilesPanel::createFilesTable()
{
	QStringList labels;
	labels << tr("File Name") << tr("Info") << tr("Size") << tr("Modified");
	ui->filesTable->setHorizontalHeaderLabels(labels);
	connect(ui->filesTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(filesTableClicked(int, int)));
	connect(ui->filesTable, SIGNAL(cellClicked(int, int)), this, SLOT(filePreview(int, int)));
}

void GUI::FindInFilesPanel::filesTableClicked(int x, int y)
{
	Q_UNUSED(y);

	QTableWidgetItem* item = ui->filesTable->item(x, 0);

	QString dir = ui->directoryComboBox->currentText();

	if (!dir.endsWith("/")) {
		dir = dir + "/";
	}

	emit fileClicked(dir + item->text());
}

void GUI::FindInFilesPanel::filePreview(int x, int y)
{
	Q_UNUSED(y);

	QTableWidgetItem* item = ui->filesTable->item(x, 0);

	QApplication::setOverrideCursor(Qt::BusyCursor);

	QTextCursor cursor = ui->preview->textCursor();

	if (!cursor.isNull()) {
		QTextCharFormat format = cursor.charFormat();
		format.setFontPointSize(12);
		cursor.mergeCharFormat(format);
		ui->preview->setTextCursor(cursor);
	}

	QString dir = ui->directoryComboBox->currentText();

	if (!dir.endsWith("/")) {
		dir = dir + "/";
	}

	QFile file(dir + item->text());

	if (file.open(QIODevice::ReadOnly)) {
		QTextStream in(&file);
		ui->preview->setPlainText(in.readAll());
		file.close();

		if (highlight) {
			if (highlighter == nullptr) {
				highlighter = new Highlighter(ui->preview->document());
			}

			if (highlighter != nullptr) {
				int mode = autoDetectHighligthMode(ui->preview->toPlainText().toUpper());
				highlighter->setHighlightMode(mode);
				highlighter->setHColors(highlighterColors, ui->preview->font());
				highlighter->rehighlight();
			}
		}

		qApp->processEvents();

		if ((!ui->textComboBox->currentText().isEmpty()) && !(ui->textComboBox->currentText() == "*")) {
			highlightFindText(ui->textComboBox->currentText(),
			                  (ui->wholeWordsCheckBox->isChecked() ? QTextDocument::FindWholeWords : QTextDocument::FindFlags()));

			if (findText(ui->textComboBox->currentText(),
			             (ui->wholeWordsCheckBox->isChecked() ? QTextDocument::FindWholeWords : QTextDocument::FindFlags())),
			        (ui->commentStyle1CheckBox->isChecked() || ui->commentStyle2CheckBox->isChecked())) {
				ui->preview->centerCursor();
			}
		}
	}

	QApplication::restoreOverrideCursor();
}

bool GUI::FindInFilesPanel::findText(const QString& exp, QTextDocument::FindFlags options,
                                     bool ignoreComments)
{
	bool found = false;
	QTextCursor cursor;
	bool inComment = false;
	QString cur_line;
	int cur_line_column;
	int commentPos;

	ui->preview->setUpdatesEnabled(false);

	commentPos = -1;

	do {
		found = ui->preview->find(exp, options);

		if (found && ignoreComments) {
			cursor = ui->preview->textCursor();
			cur_line = cursor.block().text();

			cur_line_column = cursor.columnNumber();

			if (ui->commentStyle1CheckBox->isChecked() && ui->commentStyle2CheckBox->isChecked()) {
				commentPos  = cur_line.indexOf(';', 0);

				if (commentPos < 0) {
					commentPos  = cur_line.indexOf('(', 0);
				}
			} else {
				if (ui->commentStyle2CheckBox->isChecked()) {
					commentPos  = cur_line.indexOf('(', 0);
				}

				if (ui->commentStyle1CheckBox->isChecked()) {
					commentPos  = cur_line.indexOf(';', 0);
				}
			}

			if (commentPos < 0) {
				commentPos = cur_line_column + 1;
			}

			inComment = (commentPos < cur_line_column);
		} else {
			inComment = false;
		}

	} while (inComment);

	if (!found) {
		cursor.clearSelection();
		ui->preview->setTextCursor(cursor);
	}

	ui->preview->setUpdatesEnabled(true);
	return found;
}

void GUI::FindInFilesPanel::setHighlightColors(const HighlightColors colors)
{
	highlight = true;
	highlighterColors = colors;
}

void GUI::FindInFilesPanel::setDir(const QString dir)
{
	ui->directoryComboBox->addItem(QDir::toNativeSeparators(dir));
	ui->directoryComboBox->setCurrentIndex(ui->directoryComboBox->findText(QDir::toNativeSeparators(dir)));
}

void GUI::FindInFilesPanel::highlightFindText(QString searchString, QTextDocument::FindFlags options)
{
	findTextExtraSelections.clear();
	QColor lineColor = QColor(Qt::yellow).lighter(155);
	selection.format.setBackground(lineColor);

	QTextDocument* doc = ui->preview->document();
	QTextCursor cursor = ui->preview->textCursor();
	cursor.setPosition(0);

	do {
		cursor = doc->find(searchString, cursor, options);

		if (!cursor.isNull()) {
			selection.cursor = cursor;

			QTextCharFormat format = cursor.charFormat();
			format.setFontPointSize(16);
			qApp->processEvents();
			cursor.mergeCharFormat(format);

			findTextExtraSelections.append(selection);

		}
	} while (!cursor.isNull());

	ui->preview->setExtraSelections(findTextExtraSelections);
}
