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
#ifndef REVERSEMATCHWINDOW_H
#define REVERSEMATCHWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "spritesheet.hpp"
#include "displayobject.hpp"
namespace Ui {
class ReverseMatchWindow;
}

class ReverseMatchWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit ReverseMatchWindow(QWidget *parent = 0);
	~ReverseMatchWindow();

	bool MatchPixel( QRgb p1, QRgb p2, QMap<QRgb, QRgb> &like_matche );
	bool FindMatch( QImage * map, QImage tile, QRect rect, QList<QRect> &local_hits );
	void Scan( SpriteSheet * sheet );


private slots:
	void on_actionLoad_Sheet_triggered();

	void on_actionLoad_Map_triggered();

	void on_actionSave_Map_triggered();

private:
	Ui::ReverseMatchWindow *ui;

	QList<DisplayObject*> hits;
	QImage * map;
	QGraphicsScene * scene;
};

#endif // REVERSEMATCHWINDOW_H
