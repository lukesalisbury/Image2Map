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
#include "spritechooser.hpp"
#include "ui_spritechooser.h"

#include <QGraphicsItem>
#include <QDebug>
SpriteChooser::SpriteChooser(SpriteSheet * sheet, QWidget *parent) : QDialog(parent), ui(new Ui::SpriteChooser)
{
	this->sheet = sheet;
	this->setAttribute(Qt::WA_DeleteOnClose, false);

	ui->setupUi(this);
	ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);

	scene = new QGraphicsScene(0, 0, sheet->image->width(), sheet->image->height() );
	scene->addPixmap( QPixmap::fromImage(*sheet->image) );

	QGraphicsRectItem * rect = NULL;
	QHashIterator<QString, QRect> q(sheet->items);
	while (q.hasNext())
	{
		q.next();
		rect = scene->addRect( q.value(), QPen( QColor(255,0,0,255) ), QBrush() );
		rect->setAcceptedMouseButtons(Qt::LeftButton);
		rect->setFlag( QGraphicsItem::ItemIsSelectable, true );
		rect->setToolTip( q.key() );
	}

	QObject::connect( scene, SIGNAL(selectionChanged()), this, SLOT(on_selection()) );

	QPixmap transPix(16,16);
	QPainter painter(&transPix);
	painter.fillRect(QRect(0,0,8,8), QColor(192,192,192));
	painter.fillRect(QRect(8,8,8,8), QColor(192,192,192));
	painter.fillRect(QRect(8,0,8,8), QColor(128,128,128));
	painter.fillRect(QRect(0,8,8,8), QColor(128,128,128));
	painter.end();

	ui->graphicsView->setBackgroundBrush(QBrush(transPix));
	ui->graphicsView->setScene(scene);
	ui->graphicsView->show();

}

SpriteChooser::~SpriteChooser()
{
	delete ui;
}

bool SpriteChooser::GetMatch( QString & sprite  )
{
	sprite = match;
	return remove;
}

void SpriteChooser::RequestMatch( QImage tile, QString current )
{
	ui->imageLabel->setPixmap( QPixmap::fromImage(tile) );
	ui->noteLabel->setText( current );

}

void SpriteChooser::on_selection()
{
	QList<QGraphicsItem *> items = scene->selectedItems();
	if ( items.size() )
	{
		match = this->sheet->file.completeBaseName() + ":" + items.at(0)->toolTip();;
		remove = false;
		this->close();
	}
}


void SpriteChooser::on_skipButton_clicked()
{
	match = "";
	remove = false;
	this->close();
}

void SpriteChooser::on_ignoreButton_clicked()
{
	remove = false;
	match = "";
	this->close();
}

void SpriteChooser::on_deleteButton_clicked()
{
	remove = true;
	match = "REMOVE";
	this->close();
}
