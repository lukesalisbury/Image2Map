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
#include "nexttiledialog.hpp"
#include "ui_nexttiledialog.h"

NextTileDialog::NextTileDialog(QImage image, QImage image2, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::NextTileDialog)
{
	ui->setupUi(this);
	QGraphicsScene * scene1 = new QGraphicsScene(0, 0, image.width(), image.height() );
	QGraphicsScene * scene2 = new QGraphicsScene(0, 0, image2.width(), image2.height() );

	scene1->addPixmap( QPixmap::fromImage(image) );
	scene2->addPixmap( QPixmap::fromImage(image2) );
	ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
	ui->graphicsView->setScene(scene1);
	ui->graphicsView->show();

	ui->graphicsView_2->setCacheMode(QGraphicsView::CacheBackground);
	ui->graphicsView_2->setScene(scene2);
	ui->graphicsView_2->show();
}


NextTileDialog::~NextTileDialog()
{
	delete ui;
}
