#ifndef TERMIALSCAN_HPP
#define TERMIALSCAN_HPP

#include <QString>
#include "spritesheet.hpp"
#include "displayobject.hpp"
class TermialScan
{
	private:
		QImage * map;
		QString output;
		QString directory;
		QList<SpriteSheet*> sheetList;
		QList<DisplayObject*> hits;

	public:
		TermialScan();

		void loadSpriteSheets();
		void run();
		void saveMap();

};

#endif // TERMIALSCAN_HPP
