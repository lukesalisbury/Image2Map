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
#include "colourmatchdialog.h"
#include "ui_colourmatchdialog.h"

ColourMatchDialog::ColourMatchDialog(QRgb map, QRgb sprite, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ColourMatchDialog)
{
	ui->setupUi(this);


	QBrush mapBrush( QColor::fromRgba(map) );
	QBrush spriteBrush( QColor::fromRgba(sprite) );


	ui->mapColour->setBackgroundBrush(mapBrush);
	ui->mapColour->setScene( new QGraphicsScene(0,0,64,64) );
	ui->mapColour->show();

	ui->spriteColour->setBackgroundBrush(spriteBrush);
	ui->spriteColour->setScene( new QGraphicsScene(0,0,64,64) );
	ui->spriteColour->show();
}

ColourMatchDialog::~ColourMatchDialog()
{
	delete ui;
}
