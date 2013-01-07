/*-----------------------------------------------------------------------------
Copyright © 2013 Luke Salisbury
This software is provided 'as-is', without any express or implied warranty. In 
no event will the authors be held liable for any damages arising from the use 
of this software.

Permission is granted to anyone to use this software for any purpose, including 
commercial applications, and to alter it and redistribute it freely, subject to 
the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim 
   that you wrote the original software. If you use this software in a product, 
   an acknowledgment in the product documentation would be appreciated but is 
   not required.
2. Altered source versions must be plainly marked as such, and must not be 
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-----------------------------------------------------------------------------*/
#include "spritematchdialog.hpp"
#include "ui_spritematchdialog.h"

SpriteMatchDialog::SpriteMatchDialog(QImage image, QImage match_guess, float precent, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SpriteMatchDialog)
{

	tile = QPixmap::fromImage(image);
	guess = QPixmap::fromImage(match_guess);

	ui->setupUi(this);
	ui->objectLabel->setPixmap(tile);
	ui->spriteLabel->setPixmap(guess);

	QString str_msg = QString("An %1% match").arg(precent*100);

	ui->label_2->setText( str_msg );

	ui->objectLabel->setVisible(true);
	ui->spriteLabel->setVisible(true);

	ui->objectLabel->resize( image.size() );
	ui->spriteLabel->resize( match_guess.size() );
}

SpriteMatchDialog::~SpriteMatchDialog()
{
	delete ui;
}
