#include "termialscan.hpp"

#include <QFileDialog>
#include <iostream>
#include <QProgressDialog>
#include <QTextStream>

#include "scan.hpp"

TermialScan::TermialScan()
{
	directory = QFileDialog::getExistingDirectory(NULL, "Select Directory", "", QFileDialog::ShowDirsOnly );
	//directory = "C:/Users/luke/openzelda-quests/TheJourneyBegin";
}

void TermialScan::loadSpriteSheets()
{
	QDir currentDir;
	QStringList entries;
	QStringList filters;


	filters << "*.xml";
	currentDir.setNameFilters(filters);


	currentDir.setPath(directory + "/sprites/");
	currentDir.setFilter( QDir::NoDotAndDotDot | QDir::Files );

	entries = currentDir.entryList();
	for( QStringList::ConstIterator entry = entries.begin(); entry != entries.end(); entry++ )
	{
		QFileInfo file;
		SpriteSheet * sheet;

		file.setFile(directory + "/sprites/" + *entry);
		sheet = new SpriteSheet( file );

		if ( sheet->spriteCount() )
			sheetList.append(sheet);
		else
			delete sheet;
	}

}

void TermialScan::run()
{
	QFileInfo outputInfo;
	QString mapImagePath = QFileDialog::getOpenFileName(NULL, "Open Map Image", NULL, "Images (*.png)" );

	outputInfo.setFile(mapImagePath);
	output = outputInfo.baseName();
	map = new QImage(mapImagePath);

	std::cout << "Outputting " << output << std::endl;

	for ( int i = 0; i < sheetList.size(); i++ )
	{
		SpriteSheet * sheet = sheetList.at(i);
		std::cout << "Scanning " << sheet->name.toStdString() << std::endl;

		Scan scanThread( map, sheet );

		scanThread.doScan();
	}


	saveMap();
}


void TermialScan::saveMap()
{
	QString outputPath = directory + "/maps/" +  output + ".xml";

	QTextStream out(stdout);
	QFile data( outputPath );

	if ( data.open(QFile::WriteOnly | QFile::Truncate) )
	{
		out.setDevice(&data);
	}
	std::cout << "Writing " << outputPath.toStdString() << std::endl;

	out << "<map xmlns=\"http://mokoi.info/projects/mokoi\">" << '\n';
	out << "\t<settings>" << '\n';
	out << "\t\t<dimensions width=\"1\" height=\"1\" />" << '\n';
	out << "\t\t<color red=\"0\" blue=\"0\" green=\"0\" mode=\"0\" />" << '\n';
	out << "\t</settings>" << '\n';


	QListIterator<DisplayObject*> r(hits);
	while (r.hasNext())
	{
		DisplayObject * area = r.next();

		out << "\t<object value=\"" << area->name << "\" type=\"sprite\">" << '\n';
		out << "\t\t<color red=\"255\" blue=\"255\" green=\"255\" alpha=\"255\" />" << '\n';
		out << "\t\t<position x=\"" << area->rect.x() << "\" y=\"" << area->rect.y() << "\" ";
		out << "w=\"" << area->rect.width() << "\" h=\"" << area->rect.height() << "\" z=\"1\" />" << '\n';
		out << "\t</object>" << '\n';

	}
	out << "</map>" << '\n';
}
