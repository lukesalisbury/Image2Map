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
#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QStandardItem>
#include <QFileDialog>
#include <QObject>
#include <QBuffer>
#include "spritechooser.hpp"
#include "table.hpp"

MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QObject::connect( this->ui->actionOpen_Image, SIGNAL(triggered()), this, SLOT(on_openimage_triggered()) );
	QObject::connect( this->ui->actionSave_map, SIGNAL(triggered()), this, SLOT(on_savemap_triggered()) );
	QObject::connect( this->ui->actionSave_Sheet, SIGNAL(triggered()), this, SLOT(on_savesheet_triggered()) );

	img = NULL;

	file.setFileName(QDir::homePath() + "/imageconvert.dbt");

	model = new TableModel();
	ui->tableView->setModel(model);

	loadDatabase();

}

MainWindow::~MainWindow()
{
	saveDatabase();
	delete ui;
}

void MainWindow::saveDatabase()
{
	return;
	if ( img )
	{
		img->Save(file);
	}
}

void MainWindow::loadDatabase()
{
	return;
	if ( !img )
	{
		img = new ImageConvert(file);
	}

	UpdateTable();
}

void MainWindow::UpdateTable()
{
	if ( !img )
		return;
	model->removeRows(0,model->rowCount());
	QModelIndex index;
	QHashIterator<quint16, MapSprite*> q(img->sprites);
	while (q.hasNext()) {
		q.next();

		model->insertRows(0, 1, QModelIndex());
		index = model->index(0, 0, QModelIndex()); // image
		qobject_cast<TableModel *>(model)->setImage(index, dynamic_cast<QImage*>(q.value()) );
		index = model->index(0, 1, index); // id
		model->setData(index, q.key(), Qt::DisplayRole);
		index = model->index(0, 2, index); // replace
		model->setData(index, q.value()->replace_id, Qt::EditRole);
		index = model->index(0, 3, index); //remove
		model->setData(index, q.value()->remove, Qt::EditRole);
		index = model->index(0, 4, index); //mask
		model->setData(index, q.value()->mask, Qt::EditRole);
		index = model->index(0, 5, index); //rotate
		model->setData(index, q.value()->flipmode, Qt::EditRole);
	}

}

void MainWindow::on_savemap_triggered()
{
	if ( img )
	{
		QFile outfile(QDir::homePath() + "/" + img->fileinfo.baseName() + ".xml");
		if (outfile.open(QFile::WriteOnly | QFile::Truncate) )
		{
			QTextStream out(&outfile);

			qDebug() << outfile.fileName();
			out << "<map xmlns=\"http://mokoi.info/projects/mokoi\">\n<settings></settings>\n";

			for (int i = 0; i < img->objects.size(); ++i)
			{
				out << "<object value=\"itom.png:" << img->objects.at(i)->checksum << "\" type=\"sprite\">\n";
				out << "\t<position x=\""  << img->objects.at(i)->position.x()  << "\" ";
				out << "y=\"" << img->objects.at(i)->position.y()  << "\" ";
				out << "w=\"" << (img->objects.at(i)->position.width() > 1 ? img->objects.at(i)->position.width()*img->tile_width : 0)  << "\" ";
				out << "h=\"" << (img->objects.at(i)->position.width() > 1 ? img->tile_height : 0)  << "\" ";
				out << "z=\"" << 0  << "\" ";
				out << "f=\"" << 0  << "\" ";
				out << "/>\n</object> ";

			}
			out << "</map>";
			out.flush();
		}
		outfile.close();
	}
}

void MainWindow::on_savesheet_triggered()
{
	if ( img )
	{
		uint32_t x = 0;
		QHashIterator<quint16, MapSprite*> q(img->sprites);
		QFile outfile(QDir::homePath() + "/itom.png.txt");
		if (outfile.open(QFile::WriteOnly | QFile::Truncate) )
		{
			QTextStream out(&outfile);
			while (q.hasNext()) {
				q.next();
				// #Sprite Name, Flag, Mask Value/File, Default Entity, X, Y, Width, Height
				out << q.key() << "\t0\t" << 0 << "\t\t" << x << "\t0\t16\t16";
				x += 16;return;
			}
			out.flush();
		}
		outfile.close();
	}
}


void MainWindow::on_openimage_triggered()
{
	QString project_directory = QFileDialog::getOpenFileName(this, tr("Open Image"), NULL, tr("Images (*.png *.xpm *.jpg)") );
	if ( img )
		delete img;


	img = new ImageConvert(project_directory, file);

	UpdateTable();
}


void MainWindow::on_actionOpen_triggered()
{
	QString project_directory = QFileDialog::getOpenFileName(this, tr("Open Spritesheet"), NULL, tr("Spritesheets (*.png.txt)") );
	QFileInfo sheetinfo(project_directory);
	SpriteSheet * sheet = new SpriteSheet(sheetinfo);

	if ( img )
	{
		SpriteChooser * dialog = new SpriteChooser( sheet, this );
		QHashIterator<quint16, MapSprite*> q(img->sprites);

		while (q.hasNext()) {
			q.next();
			if ( q.value()->replace_id.length() <5 && q.value()->width() == 32 && q.value()->height() == 32 )
			{
				qDebug() << q.value()->id;
				dialog->RequestMatch( q.value()->convertToFormat(QImage::Format_ARGB32), q.value()->replace_id );
				if ( dialog->exec() == QDialog::Accepted )
				{
					if ( dialog->GetMatch( q.value()->replace_id ) )
					{
						q.value()->remove = true;
					}
				}
			}
		}
		delete dialog;
		UpdateTable();
	}
	else
	{
		SpriteChooser * dialog = new SpriteChooser( sheet, this );
		dialog->exec();
		delete dialog;
	}

	delete sheet;


}
