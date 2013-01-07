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
#include "reversematchwindow.h"
#include "ui_reversematchwindow.h"
#include <QFileDialog>
#include "spritesheet.hpp"
#include <QGraphicsRectItem>
#include <QDebug>
#include <QProgressDialog>
#include <QApplication>
#include <iostream>
#include "colourmatchdialog.h"
#include "nexttiledialog.hpp"

extern QApplication * app;

ReverseMatchWindow::ReverseMatchWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ReverseMatchWindow)
{
	ui->setupUi(this);

	map = NULL;
}

ReverseMatchWindow::~ReverseMatchWindow()
{
	delete ui;
}


void ReverseMatchWindow::Scan( SpriteSheet * sheet )
{
	int hits_size = sheet->items.size();
	QString name;
	QGraphicsRectItem * rect = NULL;
	QHashIterator<QString, QRect> q(sheet->items);
	QList<QRect> local_hits;

	/* Progress Dialog */
	QProgressDialog dialog( "Scanning", "Cancel", 0, hits_size, this);
	dialog.setWindowModality(Qt::WindowModal);
	dialog.show();
	app->processEvents();

	while (q.hasNext())
	{
		q.next();

		/* Set sprite name */
		name = sheet->name + ":" + q.key();

		/* Progress Dialog */
		dialog.setLabelText(name);
		app->processEvents();

		FindMatch(map, sheet->GetImage(q.value()), q.value(), local_hits );

		/* Manage Local hits */
		QListIterator<QRect> r(local_hits);
		while (r.hasNext())
		{
			QRect area = r.next();
			DisplayObject * object = new DisplayObject( name, area );
			hits.append(object);

			rect = scene->addRect( area, QPen( QColor(255,0,0,255) ), QBrush() );
			rect->setAcceptedMouseButtons(Qt::LeftButton);
			rect->setFlag( QGraphicsItem::ItemIsSelectable, true );
			rect->setToolTip( name );
		}
		local_hits.clear();

		/* Progress Dialog */
		dialog.setValue(dialog.value() + 1);
	}
	dialog.close();
}

void ReverseMatchWindow::on_actionLoad_Sheet_triggered()
{
	QString project_directory = QFileDialog::getOpenFileName(this, tr("Open Spritesheet"), NULL, tr("Spritesheets (*.png.txt)") );

	app->processEvents();

	QFileInfo sheetinfo(project_directory);
	SpriteSheet * sheet = new SpriteSheet(sheetinfo);

	Scan( sheet );
}

void ReverseMatchWindow::on_actionLoad_Map_triggered()
{
	QString project_directory = QFileDialog::getOpenFileName(this, tr("Open Image"), NULL, tr("Images (*.png)") );

	map = new QImage(project_directory);

	ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);

	scene = new QGraphicsScene(0, 0, map->width(), map->height() );
	scene->addPixmap( QPixmap::fromImage(*map) );

	ui->graphicsView->setScene(scene);
	ui->graphicsView->show();

}

bool ReverseMatchWindow::MatchPixel( QRgb map_pixel, QRgb sprite_pixel, QMap<QRgb, QRgb> & like_matches )
{
	unsigned char offset = 10;
	int r2 = qRed(sprite_pixel);
	int g2 = qGreen(sprite_pixel);
	int b2 = qBlue(sprite_pixel);

	int r1 = qRed(map_pixel);
	int g1 = qGreen(map_pixel);
	int b1 = qBlue(map_pixel);

	if ( map_pixel == 0xFF48a048 && qAlpha(sprite_pixel) != 255 )
	{
		// Green grass
		return 1;
	}

	map_pixel = like_matches.value ( sprite_pixel, map_pixel );

	if ( map_pixel == sprite_pixel )
		return 1;
	if ( (r2-offset < r1 && r2+offset > r1) && (g2-offset < g1 && g2+offset > g1) && (b2-offset < b1 && b2+offset > b1) )
		return 1;

	return 0;

}

bool ReverseMatchWindow::FindMatch( QImage * map, QImage sprite, QRect rect, QList<QRect> & local_hits )
{
	QMap<QRgb, QRgb> like_matches;
	QRgb map_pixel, sprite_pixel;
	int x = 320, y = 128;
	int ix, iy;
	int matches, total;

	total = sprite.width() * sprite.height();
	if ( total == 0 )
		return false;
	for ( x = 0; x < map->width(); x += 16 )
	{
		for ( y = 0; y < map->height(); y += 16 )
		{
			matches = 0;
			for ( ix = 0; ix < sprite.width(); ix++ )
			{
				for ( iy = 0; iy < sprite.height(); iy++ )
				{
					if ( !sprite.valid(ix,iy) )
						break;
					if ( !map->valid(x+ix,y+iy) )
						break;
					sprite_pixel = sprite.pixel(ix,iy);
					map_pixel = map->pixel(x+ix,y+iy);
					if ( MatchPixel( map_pixel, sprite_pixel, like_matches) )
					{
						matches++;
					}
				}
			}
			if ( matches == total)
			{
				QRect match;
				match.setX(x);
				match.setY(y);
				match.setWidth( sprite.width() );
				match.setHeight( sprite.height() );
				local_hits.append(match);
			}

		}
	}
	return false;
}


void ReverseMatchWindow::on_actionSave_Map_triggered()
{
	QString map_filename = QFileDialog::getSaveFileName(this, tr("Save Map file"), NULL, tr("Mokoi Map (*.xml)") );

	std::cout << "Map:" << map_filename.toStdString() << std::endl;
	std::cout << "<map xmlns=\"http://mokoi.info/projects/mokoi\">" << std::endl;
	std::cout << "<settings>" << std::endl;
	std::cout << "<dimensions width=\"1\" height=\"1\" />" << std::endl;
	std::cout << "<color red=\"0\" blue=\"0\" green=\"0\" mode=\"0\" />" << std::endl;
	std::cout << "</settings>" << std::endl;


	QListIterator<DisplayObject*> r(hits);
	while (r.hasNext())
	{
		DisplayObject * area = r.next();

		std::cout << "<object value=\"" << area->name.toStdString() << "\" type=\"sprite\">" << std::endl;
		std::cout << "<color red=\"255\" blue=\"255\" green=\"255\" alpha=\"255\" />" << std::endl;
		std::cout << "<position x=\"" << area->rect.x() << "\" y=\"" << area->rect.y() << "\" ";
		std::cout << "w=\"" << area->rect.width() << "\" h=\"" << area->rect.height() << "\" z=\"1\" />" << std::endl;
		std::cout << "</object>" << std::endl;

	}
	std::cout << "</map>" << std::endl;
}
