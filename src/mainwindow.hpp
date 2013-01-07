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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "imageconvert.hpp"
#include <QStandardItemModel>
#include <QFile>
#include "spritesheet.hpp"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	ImageConvert * img;
private:
	Ui::MainWindow *ui;
	QAbstractItemModel * model;
	QFile file;
	QString name;
	void saveDatabase();
	void loadDatabase();

	void UpdateTable();

private slots:
	void on_openimage_triggered();
	void on_savemap_triggered();
	void on_savesheet_triggered();
	void on_actionOpen_triggered();
};

#endif // MAINWINDOW_HPP
