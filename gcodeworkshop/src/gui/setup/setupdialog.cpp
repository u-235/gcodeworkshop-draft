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

#include <algorithm>    // for sort, unique

#include <QAbstractButton>              // for QAbstractButton
#include <QButtonGroup>                 // for QButtonGroup
#include <QByteArray>                   // for QByteArray
#include <QCheckBox>                    // for QCheckBox
#include <QColor>                       // for QColor
#include <QColorDialog>                 // for QColorDialog
#include <QComboBox>                    // for QComboBox
#include <QDir>                         // for QDir
#include <QFileDialog>                  // for QFileDialog
#include <QFileInfo>                    // for QFileInfo
#include <QFont>                        // for QFont
#include <QFontDialog>                  // for QFontDialog
#include <QLabel>                       // for QLabel
#include <QLineEdit>                    // for QLineEdit
#include <QList>                        // for QList
#include <QListWidget>                  // for QListWidget
#include <QListWidgetItem>              // for QListWidgetItem
#include <QPalette>                     // for QPalette
#include <QPushButton>                  // for QPushButton
#include <QRegularExpression>           // for QRegularExpression
#include <QRegularExpressionValidator>  // for QRegularExpressionValidator
#include <QTextCodec>
#include <QVariant>                     // for QVariant
#include <Qt>                           // for GlobalColor, WindowFlags
#include <QtAlgorithms>                 // for qDeleteAll
#include <QtGlobal>                     // for QTypeInfo<>::isLarge, QTypeInfo<>::isStatic, Q_OS_WIN32

class QValidator;
class QWidget;

#include <gcoderstyle.h>            // for GCoderStyle, HighlightColors
#include <gcoderwidgetproperties.h> // for GCoderWidgetProperties, SHOW_FILENAME, SHOW_FILEPATH, SHOW_PROGTITLE

#include "highlightmode.h"  // for HighlightMode
#include "setupdialog.h"

#include "ui_setupdialog.h" // for Ui::SetupDialog


GUI::SetupDialog::SetupDialog(QWidget* parent, const AppConfig* prop, Qt::WindowFlags f) :
	QDialog(parent, f),
	ui(new Ui::SetupDialog())
{
	QPalette palette;

	ui->setupUi(this);
	setWindowTitle(tr("GCodeWorkShop configuration"));

	editProp = *prop;

	ui->readOnlyModeCheckBox->setChecked(editProp.defaultReadOnly);

	ui->fontLabel->setText(QString(tr("Current font : <b>\"%1\", %2 pt.<\b>")
	                               .arg(editProp.codeStyle.fontName).arg(editProp.codeStyle.fontSize)));
	ui->fontLabel->setFont(QFont(editProp.codeStyle.fontName, editProp.codeStyle.fontSize));

	connect(ui->changeFontButton, SIGNAL(clicked()), SLOT(changeFont()));
	connect(ui->browseButton, SIGNAL(clicked()), SLOT(ui->browseButtonClicked()));

	colorButtons = new QButtonGroup(this);
	connect(colorButtons, SIGNAL(buttonClicked(QAbstractButton*)),
	        SLOT(changeColor(QAbstractButton*)));

	palette.setColor(ui->backgroundColorButton->foregroundRole(), editProp.codeStyle.hColors.backgroundColor);
	ui->backgroundColorButton->setPalette(palette);
	ui->backgroundColorButton->setAutoFillBackground(true);
	colorButtons->addButton(ui->backgroundColorButton);

	QColor color = editProp.codeStyle.hColors.backgroundColor;

	palette.setColor(QPalette::Button, color);
	palette.setColor(QPalette::Light, color);
	palette.setColor(QPalette::Dark, color);
	palette.setColor(QPalette::Shadow, color);
	palette.setColor(QPalette::Midlight, color);
	palette.setColor(QPalette::Base, color);

	palette.setColor(ui->fontColorButton->foregroundRole(), editProp.codeStyle.hColors.defaultColor);
	ui->fontColorButton->setPalette(palette);
	ui->fontColorButton->setAutoFillBackground(true);
	ui->fontColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->fontColorButton);

	palette.setColor(ui->commentColorButton->foregroundRole(), editProp.codeStyle.hColors.commentColor);
	ui->commentColorButton->setPalette(palette);
	ui->commentColorButton->setAutoFillBackground(true);
	ui->commentColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->commentColorButton);

	palette.setColor(ui->gColorButton->foregroundRole(), editProp.codeStyle.hColors.gColor);
	ui->gColorButton->setPalette(palette);
	ui->gColorButton->setAutoFillBackground(true);
	ui->gColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->gColorButton);

	palette.setColor(ui->mColorButton->foregroundRole(), editProp.codeStyle.hColors.mColor);
	ui->mColorButton->setPalette(palette);
	ui->mColorButton->setAutoFillBackground(true);
	ui->mColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->mColorButton);

	palette.setColor(ui->nColorButton->foregroundRole(), editProp.codeStyle.hColors.nColor);
	ui->nColorButton->setPalette(palette);
	ui->nColorButton->setAutoFillBackground(true);
	ui->nColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->nColorButton);

	palette.setColor(ui->lColorButton->foregroundRole(), editProp.codeStyle.hColors.lColor);
	ui->lColorButton->setPalette(palette);
	ui->lColorButton->setAutoFillBackground(true);
	ui->lColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->lColorButton);

	palette.setColor(ui->fsColorButton->foregroundRole(), editProp.codeStyle.hColors.fsColor);
	ui->fsColorButton->setPalette(palette);
	ui->fsColorButton->setAutoFillBackground(true);
	ui->fsColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->fsColorButton);

	palette.setColor(ui->dhtColorButton->foregroundRole(), editProp.codeStyle.hColors.dhtColor);
	ui->dhtColorButton->setPalette(palette);
	ui->dhtColorButton->setAutoFillBackground(true);
	ui->dhtColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->dhtColorButton);

	palette.setColor(ui->rColorButton->foregroundRole(), editProp.codeStyle.hColors.rColor);
	ui->rColorButton->setPalette(palette);
	ui->rColorButton->setAutoFillBackground(true);
	ui->rColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->rColorButton);

	palette.setColor(ui->macroColorButton->foregroundRole(), editProp.codeStyle.hColors.macroColor);
	ui->macroColorButton->setPalette(palette);
	ui->macroColorButton->setAutoFillBackground(true);
	ui->macroColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->macroColorButton);

	palette.setColor(ui->keyWordColorButton->foregroundRole(), editProp.codeStyle.hColors.keyWordColor);
	ui->keyWordColorButton->setPalette(palette);
	ui->keyWordColorButton->setAutoFillBackground(true);
	ui->keyWordColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->keyWordColorButton);

	palette.setColor(ui->progNameColorButton->foregroundRole(), editProp.codeStyle.hColors.progNameColor);
	ui->progNameColorButton->setPalette(palette);
	ui->progNameColorButton->setAutoFillBackground(true);
	ui->progNameColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->progNameColorButton);

	palette.setColor(ui->operatorColorButton->foregroundRole(), editProp.codeStyle.hColors.operatorColor);
	ui->operatorColorButton->setPalette(palette);
	ui->operatorColorButton->setAutoFillBackground(true);
	ui->operatorColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->operatorColorButton);

	palette.setColor(ui->zColorButton->foregroundRole(), editProp.codeStyle.hColors.zColor);
	ui->zColorButton->setPalette(palette);
	ui->zColorButton->setAutoFillBackground(true);
	ui->zColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->zColorButton);

	palette.setColor(ui->aColorButton->foregroundRole(), editProp.codeStyle.hColors.aColor);
	ui->aColorButton->setPalette(palette);
	ui->aColorButton->setAutoFillBackground(true);
	ui->aColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->aColorButton);

	palette.setColor(ui->bColorButton->foregroundRole(), editProp.codeStyle.hColors.bColor);
	ui->bColorButton->setPalette(palette);
	ui->bColorButton->setAutoFillBackground(true);
	ui->bColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->bColorButton);

	palette.setColor(ui->underlineColorButton->foregroundRole(), editProp.codeStyle.underlineColor);
	ui->underlineColorButton->setPalette(palette);
	ui->underlineColorButton->setAutoFillBackground(true);
	ui->underlineColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->underlineColorButton);

	palette.setColor(ui->curLineColorButton->foregroundRole(), editProp.codeStyle.lineColor);
	ui->curLineColorButton->setPalette(palette);
	ui->curLineColorButton->setAutoFillBackground(true);
	ui->curLineColorButton->setBackgroundRole(QPalette::Base);
	colorButtons->addButton(ui->curLineColorButton);

	ui->capsLockCheckBox->setChecked(editProp.editorProperties.intCapsLock);
	ui->syntaxHCheckBox->setChecked(editProp.editorProperties.syntaxH);
	ui->underlineCheckBox->setChecked(editProp.editorProperties.underlineChanges);

	ui->tabbedModecheckBox->setChecked(editProp.mdiTabbedMode);
	ui->fileNameCheckBox->setChecked(editProp.editorProperties.windowMode & SHOW_FILENAME);
	ui->filePathCheckBox->setChecked(editProp.editorProperties.windowMode & SHOW_FILEPATH);
	ui->titleCheckBox->setChecked(editProp.editorProperties.windowMode & SHOW_PROGTITLE);

	ui->calcLineEdit->setText(editProp.calcBinary);
	ui->clearUndocheckBox->setChecked(editProp.editorProperties.clearUndoHistory);
	ui->clearUnderlinecheckBox->setChecked(editProp.editorProperties.clearUnderlineHistory);
	ui->editorToolTipsCheckBox->setChecked(editProp.editorProperties.editorToolTips);
	ui->startEmptyCheckBox->setChecked(editProp.startEmpty);
	ui->disableFileChangeMonitorCheckBox->setChecked(editProp.disableFileChangeMonitor);
	ui->changeDateOnSave->setChecked(editProp.editorProperties.changeDateInComment);

	QStringListIterator extIterator(editProp.extensions);

	while (extIterator.hasNext()) {
		ui->lstExtensions->addItem(extIterator.next().toLocal8Bit().constData());
	}

	ui->edtSaveExtension->setText(editProp.saveExtension);
	ui->edtSaveDirectory->setText(editProp.saveDirectory);

	if (editProp.editorProperties.guessFileNameByProgNum) {
		ui->progNumCheckBox->setChecked(true);
	} else {
		ui->firstCommCheckBox->setChecked(true);
	}

	ui->highlightModeComboBox->addItem(tr("AUTO"), MODE_AUTO);
	ui->highlightModeComboBox->addItem(tr("FANUC"), MODE_FANUC);
	ui->highlightModeComboBox->addItem(tr("HEIDENHAIN DIALOG"), MODE_HEIDENHAIN);
	ui->highlightModeComboBox->addItem(tr("HEIDENHAIN ISO"), MODE_HEIDENHAIN_ISO);
	ui->highlightModeComboBox->addItem(tr("OKUMA OSP"), MODE_OKUMA);
	ui->highlightModeComboBox->addItem(tr("PHILIPS"), MODE_PHILIPS);
	ui->highlightModeComboBox->addItem(tr("SINUMERIK OLD"), MODE_SINUMERIK);
	ui->highlightModeComboBox->addItem(tr("SINUMERIK NEW"), MODE_SINUMERIK_840);
	ui->highlightModeComboBox->addItem(tr("LinuxCNC"), MODE_LINUXCNC);
	ui->highlightModeComboBox->addItem(tr("TOOLTIPS"), MODE_TOOLTIPS);

	int id = ui->highlightModeComboBox->findData(editProp.editorProperties.defaultHighlightMode);
	ui->highlightModeComboBox->setCurrentIndex(id);

	QRegularExpression regex("(\\*\\.)[A-Z0-9]{1,3}");
	regex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
	QValidator* edtExtensionValid = new QRegularExpressionValidator(regex, this);
	ui->edtExtension->setValidator(edtExtensionValid);

	//   QRegularExpression ext("(\\(|;){1,1}[ \\t]{0,5}(d|dd|M|MM|YYYY)[-./]{1,1}(M|MM|d|dd)[-./]{1,1}(d|dd|M|MM|YYYY)[ \\t]{1,5}(\\)){0,1}");
	//   // (\\(;){1,1}[ \\t]{0,5}(d|dd|M|MM|YYYY)[.-/]{1,1}(M|MM|d|dd)[.-/]{1,1}(d|dd|M|MM|YYYY)[ \\t]{1,5}(\\)){0,1}
	//   ext.setCaseSensitivity(Qt::CaseSensitive);
	//   QValidator *dateFormatValidator = new QRegExpValidator(ext, this);
	//   dateFormatComboBox->setValidator(dateFormatValidator);

	connect(ui->defaultButton, &QPushButton::clicked, this, &GUI::SetupDialog::setDefaultProp);
	connect(ui->okButton, &QPushButton::clicked, this, &GUI::SetupDialog::accept);
	connect(ui->cancelButton, &QPushButton::clicked, this, &GUI::SetupDialog::close);

	connect(ui->showAllCodecs, &QCheckBox::stateChanged, this, &GUI::SetupDialog::showAllCodecsClicked);
	connect(ui->showAllCodecs, &QCheckBox::stateChanged, this, &GUI::SetupDialog::fillCodecs);
	connect(ui->showAliases, &QCheckBox::stateChanged, this, &GUI::SetupDialog::fillCodecs);
	ui->dropControl->setChecked(prop->gcodeConverterOptions.dropControll);
	ui->dropExtra->setChecked(prop->gcodeConverterOptions.dropExtented);
	ui->dropEmptyLine->setChecked(prop->gcodeConverterOptions.dropEmptyLine);
	showAllCodecsClicked();
	fillCodecs();
}

GUI::SetupDialog::~SetupDialog()
{
}

void GUI::SetupDialog::changeFont()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, QFont(editProp.codeStyle.fontName, editProp.codeStyle.fontSize), this);

	if (ok) {
		editProp.codeStyle.fontName = font.family();
		editProp.codeStyle.fontSize = font.pointSize();
		ui->fontLabel->setText(QString(tr("Current font : <b>\"%1\", %2 pt.<\b>")
		                               .arg(editProp.codeStyle.fontName).arg(editProp.codeStyle.fontSize)));
		ui->fontLabel->setFont(QFont(editProp.codeStyle.fontName, editProp.codeStyle.fontSize));
	}
}

void GUI::SetupDialog::browseButtonClicked()
{
#ifdef Q_OS_WIN32
	QString filter = tr("Executables (*.exe)");
#else
	QString filter = tr("All files (*)");
#endif

	QString fileName = QFileDialog::getOpenFileName(
	                       this,
	                       tr("Select calculator executable"),
	                       ui->calcLineEdit->text(),
	                       filter);

	QFileInfo file(fileName);

	if ((file.exists()) && (file.isReadable())) {
		ui->calcLineEdit->setText(QDir::toNativeSeparators(file.canonicalFilePath()));
	}
}

AppConfig GUI::SetupDialog::getSettings()
{
	QPalette palette;
	int r, g, b;
	bool ok;

	r = 0;

	if (ui->fileNameCheckBox->isChecked()) {
		r |= SHOW_FILENAME;
	}

	if (ui->filePathCheckBox->isChecked()) {
		r |= SHOW_FILEPATH;
	}

	if (ui->titleCheckBox->isChecked()) {
		r |= SHOW_PROGTITLE;
	}

	editProp.mdiTabbedMode = ui->tabbedModecheckBox->isChecked();
	editProp.editorProperties.windowMode = r;
	editProp.editorProperties.intCapsLock = ui->capsLockCheckBox->isChecked();
	editProp.editorProperties.syntaxH = ui->syntaxHCheckBox->isChecked();
	editProp.editorProperties.underlineChanges = ui->underlineCheckBox->isChecked();
	editProp.calcBinary = ui->calcLineEdit->text();
	editProp.editorProperties.clearUndoHistory = ui->clearUndocheckBox->isChecked();
	editProp.editorProperties.clearUnderlineHistory = ui->clearUnderlinecheckBox->isChecked();
	editProp.editorProperties.editorToolTips = ui->editorToolTipsCheckBox->isChecked();
	editProp.defaultReadOnly = ui->readOnlyModeCheckBox->isChecked();
	editProp.startEmpty = ui->startEmptyCheckBox->isChecked();
	editProp.disableFileChangeMonitor = ui->disableFileChangeMonitorCheckBox->isChecked();
	editProp.editorProperties.changeDateInComment = ui->changeDateOnSave->isChecked();

	int id = ui->highlightModeComboBox->currentIndex();

	if (id >= 0) {
		editProp.editorProperties.defaultHighlightMode = ui->highlightModeComboBox->itemData(id).toInt(&ok);
	}

	palette = ui->backgroundColorButton->palette();
	palette.color(ui->backgroundColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.backgroundColor = (r << 16) + (g << 8) + b;

	palette = ui->fontColorButton->palette();
	palette.color(ui->fontColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.defaultColor = (r << 16) + (g << 8) + b;

	palette = ui->commentColorButton->palette();
	palette.color(ui->commentColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.commentColor = (r << 16) + (g << 8) + b;

	palette = ui->gColorButton->palette();
	palette.color(ui->gColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.gColor = (r << 16) + (g << 8) + b;

	palette = ui->mColorButton->palette();
	palette.color(ui->mColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.mColor = (r << 16) + (g << 8) + b;

	palette = ui->nColorButton->palette();
	palette.color(ui->nColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.nColor = (r << 16) + (g << 8) + b;

	palette = ui->lColorButton->palette();
	palette.color(ui->lColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.lColor = (r << 16) + (g << 8) + b;

	palette = ui->fsColorButton->palette();
	palette.color(ui->fsColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.fsColor = (r << 16) + (g << 8) + b;

	palette = ui->dhtColorButton->palette();
	palette.color(ui->dhtColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.dhtColor = (r << 16) + (g << 8) + b;

	palette = ui->rColorButton->palette();
	palette.color(ui->rColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.rColor = (r << 16) + (g << 8) + b;

	palette = ui->macroColorButton->palette();
	palette.color(ui->macroColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.macroColor = (r << 16) + (g << 8) + b;

	palette = ui->keyWordColorButton->palette();
	palette.color(ui->keyWordColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.keyWordColor = (r << 16) + (g << 8) + b;

	palette = ui->progNameColorButton->palette();
	palette.color(ui->progNameColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.progNameColor = (r << 16) + (g << 8) + b;

	palette = ui->operatorColorButton->palette();
	palette.color(ui->operatorColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.operatorColor = (r << 16) + (g << 8) + b;

	palette = ui->zColorButton->palette();
	palette.color(ui->zColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.zColor = (r << 16) + (g << 8) + b;

	palette = ui->aColorButton->palette();
	palette.color(ui->aColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.aColor = (r << 16) + (g << 8) + b;

	palette = ui->bColorButton->palette();
	palette.color(ui->bColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.hColors.bColor = (r << 16) + (g << 8) + b;

	palette = ui->underlineColorButton->palette();
	palette.color(ui->underlineColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.underlineColor = (r << 16) + (g << 8) + b;

	palette = ui->curLineColorButton->palette();
	palette.color(ui->curLineColorButton->foregroundRole()).getRgb(&r, &g, &b);
	editProp.codeStyle.lineColor = (r << 16) + (g << 8) + b;

	editProp.extensions.clear();

	for (int row = 0; row < ui->lstExtensions->count(); row++) {
		QListWidgetItem* item = ui->lstExtensions->item(row);
		editProp.extensions.append(item->text());
	}

	editProp.saveExtension = ui->edtSaveExtension->text();
	editProp.saveDirectory = ui->edtSaveDirectory->text();

	editProp.editorProperties.guessFileNameByProgNum = ui->progNumCheckBox->isChecked();

	editProp.gcodeConverterOptions.dropControll = ui->dropControl->isChecked();
	editProp.gcodeConverterOptions.dropExtented = ui->dropExtra->isChecked();
	editProp.gcodeConverterOptions.dropEmptyLine = ui->dropEmptyLine->isChecked();
	editProp.gcodeConverterOptions.codecName.clear();

	if (ui->encodingCombo->currentIndex() != 0) {
		QTextCodec* codec = QTextCodec::codecForName(ui->encodingCombo->currentText().toLatin1());

		if (codec != nullptr) {
			editProp.gcodeConverterOptions.codecName = codec->name();
		} else {
			editProp.gcodeConverterOptions.codecName = ui->encodingCombo->currentText().toLatin1();
		}
	}

	return (editProp);
}

void GUI::SetupDialog::changeColor(QAbstractButton* button)
{
	QPalette palette;

	palette = button->palette();

	QColor color = QColorDialog::getColor(palette.color(button->foregroundRole()), this);

	if (color.isValid()) {
		palette.setColor(button->foregroundRole(), color);
		button->setPalette(palette);
	}

	palette = ui->backgroundColorButton->palette();
	color = palette.color(ui->backgroundColorButton->foregroundRole());

	palette = ui->fontColorButton->palette();
	palette.setColor(ui->fontColorButton->backgroundRole(), color);
	ui->fontColorButton->setPalette(palette);

	palette = ui->commentColorButton->palette();
	palette.setColor(ui->commentColorButton->backgroundRole(), color);
	ui->commentColorButton->setPalette(palette);

	palette = ui->commentColorButton->palette();
	palette.setColor(ui->commentColorButton->backgroundRole(), color);
	ui->commentColorButton->setPalette(palette);

	palette = ui->gColorButton->palette();
	palette.setColor(ui->gColorButton->backgroundRole(), color);
	ui->gColorButton->setPalette(palette);

	palette = ui->mColorButton->palette();
	palette.setColor(ui->mColorButton->backgroundRole(), color);
	ui->mColorButton->setPalette(palette);

	palette = ui->nColorButton->palette();
	palette.setColor(ui->nColorButton->backgroundRole(), color);
	ui->nColorButton->setPalette(palette);

	palette = ui->lColorButton->palette();
	palette.setColor(ui->lColorButton->backgroundRole(), color);
	ui->lColorButton->setPalette(palette);

	palette = ui->fsColorButton->palette();
	palette.setColor(ui->fsColorButton->backgroundRole(), color);
	ui->fsColorButton->setPalette(palette);

	palette = ui->dhtColorButton->palette();
	palette.setColor(ui->dhtColorButton->backgroundRole(), color);
	ui->dhtColorButton->setPalette(palette);

	palette = ui->rColorButton->palette();
	palette.setColor(ui->rColorButton->backgroundRole(), color);
	ui->rColorButton->setPalette(palette);

	palette = ui->macroColorButton->palette();
	palette.setColor(ui->macroColorButton->backgroundRole(), color);
	ui->macroColorButton->setPalette(palette);

	palette = ui->keyWordColorButton->palette();
	palette.setColor(ui->keyWordColorButton->backgroundRole(), color);
	ui->keyWordColorButton->setPalette(palette);

	palette = ui->progNameColorButton->palette();
	palette.setColor(ui->progNameColorButton->backgroundRole(), color);
	ui->progNameColorButton->setPalette(palette);

	palette = ui->operatorColorButton->palette();
	palette.setColor(ui->operatorColorButton->backgroundRole(), color);
	ui->operatorColorButton->setPalette(palette);

	palette = ui->zColorButton->palette();
	palette.setColor(ui->zColorButton->backgroundRole(), color);
	ui->zColorButton->setPalette(palette);

	palette = ui->aColorButton->palette();
	palette.setColor(ui->aColorButton->backgroundRole(), color);
	ui->aColorButton->setPalette(palette);

	palette = ui->bColorButton->palette();
	palette.setColor(ui->bColorButton->backgroundRole(), color);
	ui->bColorButton->setPalette(palette);

	palette = ui->underlineColorButton->palette();
	palette.setColor(ui->underlineColorButton->backgroundRole(), color);
	ui->underlineColorButton->setPalette(palette);

	palette = ui->curLineColorButton->palette();
	palette.setColor(ui->curLineColorButton->backgroundRole(), color);
	ui->curLineColorButton->setPalette(palette);
}

void GUI::SetupDialog::setDefaultProp()
{
	QPalette palette;

	palette.setColor(ui->commentColorButton->foregroundRole(), 0xde0020);
	ui->commentColorButton->setPalette(palette);

	palette.setColor(ui->gColorButton->foregroundRole(), 0x1600ee);
	ui->gColorButton->setPalette(palette);

	palette.setColor(ui->mColorButton->foregroundRole(), 0x80007d);
	ui->mColorButton->setPalette(palette);

	palette.setColor(ui->nColorButton->foregroundRole(), Qt::darkGray);
	ui->nColorButton->setPalette(palette);

	palette.setColor(ui->lColorButton->foregroundRole(), 0x535b5f);
	ui->lColorButton->setPalette(palette);

	palette.setColor(ui->fsColorButton->foregroundRole(), 0x516600);
	ui->fsColorButton->setPalette(palette);

	palette.setColor(ui->dhtColorButton->foregroundRole(), 0x660033);
	ui->dhtColorButton->setPalette(palette);

	palette.setColor(ui->rColorButton->foregroundRole(), 0x24576f);
	ui->rColorButton->setPalette(palette);

	palette.setColor(ui->macroColorButton->foregroundRole(), 0x000080);
	ui->macroColorButton->setPalette(palette);

	palette.setColor(ui->keyWordColorButton->foregroundRole(), 0x1d8000);
	ui->keyWordColorButton->setPalette(palette);

	palette.setColor(ui->progNameColorButton->foregroundRole(), Qt::black);
	ui->progNameColorButton->setPalette(palette);

	palette.setColor(ui->operatorColorButton->foregroundRole(), 0x9a2200);
	ui->operatorColorButton->setPalette(palette);

	palette.setColor(ui->zColorButton->foregroundRole(), 0x000080);
	ui->zColorButton->setPalette(palette);

	palette.setColor(ui->aColorButton->foregroundRole(), Qt::black);
	ui->aColorButton->setPalette(palette);

	palette.setColor(ui->bColorButton->foregroundRole(), Qt::black);
	ui->bColorButton->setPalette(palette);

	palette.setColor(ui->underlineColorButton->foregroundRole(), Qt::green);
	ui->underlineColorButton->setPalette(palette);

	palette.setColor(ui->curLineColorButton->foregroundRole(), 0xFEFFB6);
	ui->curLineColorButton->setPalette(palette);

	palette.setColor(ui->fontColorButton->foregroundRole(), Qt::black);
	ui->fontColorButton->setPalette(palette);

	palette.setColor(ui->backgroundColorButton->foregroundRole(), 0xFFFFFF);
	ui->backgroundColorButton->setPalette(palette);

	ui->syntaxHCheckBox->setChecked(true);
	ui->capsLockCheckBox->setChecked(true);
	ui->underlineCheckBox->setChecked(true);
	ui->tabbedModecheckBox->setChecked(false);
	ui->fileNameCheckBox->setChecked(true);
	ui->filePathCheckBox->setChecked(false);
	ui->titleCheckBox->setChecked(false);
	ui->editorToolTipsCheckBox->setChecked(true);
	ui->readOnlyModeCheckBox->setChecked(false);
	ui->disableFileChangeMonitorCheckBox->setChecked(false);
	editProp.codeStyle.fontName = "Courier";
	editProp.codeStyle.fontSize = 12;

	editProp.editorProperties.defaultHighlightMode = MODE_AUTO;
	int id = ui->highlightModeComboBox->findData(editProp.editorProperties.defaultHighlightMode);
	ui->highlightModeComboBox->setCurrentIndex(id);

	ui->clearUndocheckBox->setChecked(false);
	ui->clearUnderlinecheckBox->setChecked(true);
	ui->startEmptyCheckBox->setChecked(false);

#ifdef Q_OS_LINUX
	editProp.calcBinary = "kcalc";
#endif

#ifdef Q_OS_WIN32
	editProp.calcBinary = "calc.exe";
#endif

	ui->calcLineEdit->setText(editProp.calcBinary);

	ui->fontLabel->setText(QString(tr("Current font : <b>\"%1\", %2 pt.<\b>")
	                               .arg(editProp.codeStyle.fontName).arg(editProp.codeStyle.fontSize)));
	ui->fontLabel->setFont(QFont(editProp.codeStyle.fontName, editProp.codeStyle.fontSize));

	ui->lstExtensions->clear();
	ui->lstExtensions->addItem("*.nc");
	ui->lstExtensions->addItem("*.cnc");
	//ui->lstExtensions->addItem("*.anc");

	ui->progNumCheckBox->setChecked(true);

	ui->edtSaveExtension->setText("*.nc");
}

void GUI::SetupDialog::on_btnAddExtension_clicked()
{
	QString ext = ui->edtExtension->text().simplified();

	if ((ext == "") || (ext == "*.")) {
		return;
	}

	ui->lstExtensions->addItem(ext);
	ui->edtExtension->setText("*.");
}

void GUI::SetupDialog::on_btnDeleteExtension_clicked()
{
	qDeleteAll(ui->lstExtensions->selectedItems());
}

void GUI::SetupDialog::on_btnBrowseDirectory_clicked()
{
	QString dir = ui->edtSaveDirectory->text();

	if (dir.isEmpty()) {
		dir = QDir::homePath();
	}

	dir = QFileDialog::getExistingDirectory(
	          this,
	          tr("Select default save directory"),
	          dir);

	if (!dir.isEmpty()) {
		ui->edtSaveDirectory->setText(dir);
	}
}

void GUI::SetupDialog::showAllCodecsClicked()
{
	ui->showAliases->setEnabled(ui->showAllCodecs->isChecked());
}

void GUI::SetupDialog::fillCodecs()
{
	ui->encodingCombo->clear();
	ui->encodingCombo->setEditable(!ui->showAllCodecs->isChecked());
	ui->encodingCombo->addItem(tr("System charset (%1)").arg(QString(QTextCodec::codecForLocale()->name())));
	ui->encodingCombo->insertSeparator(1);

	QList<int> mibList;

	if (ui->showAllCodecs->isChecked()) {
		mibList = QTextCodec::availableMibs();
	} else {
		mibList = {
			2250, 2251, 2252, 2253, 2254, 2255, 2256, 2257, 2258
		};
	}

	QList<QByteArray> codecList;

	for (int i : mibList) {
		QTextCodec* tc = QTextCodec::codecForMib(i);

		if (tc != nullptr) {
			codecList.append(tc->name());

			if (ui->showAllCodecs->isChecked() && ui->showAliases->isChecked()) {
				codecList.append(tc->aliases());
			}
		}
	}

	std::sort(codecList.begin(), codecList.end());
	auto last = std::unique(codecList.begin(), codecList.end());
	codecList.erase(last, codecList.end());

	for (auto i = codecList.cbegin(); i < codecList.cend(); ++i) {
		ui->encodingCombo->addItem(*i);
	}

	QByteArray currentCodec = editProp.gcodeConverterOptions.codecName;
	int currentIndex = codecList.indexOf(currentCodec);

	if (currentIndex < 0) {
		if (! currentCodec.isEmpty()) {
			ui->encodingCombo->insertItem(2, currentCodec);
			ui->encodingCombo->insertSeparator(3);
			currentIndex = 2;
		} else {
			currentIndex = 0;
		}
	} else {
		currentIndex += 2;
	}

	ui->encodingCombo->setCurrentIndex(currentIndex);
}
