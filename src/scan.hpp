#ifndef SCAN_HPP
#define SCAN_HPP

#include <QThread>
#include <QString>
#include "displayobject.hpp"
#include "spritesheet.hpp"

class Scan : public QThread
{
	Q_OBJECT

	private:
		QImage * map;
		QList<DisplayObject*> hits;
		SpriteSheet * sheet;
		bool FindMatch( QImage * map, QImage sprite, QRect rect, QList<QRect> & local_hits );
		bool MatchPixel( QRgb map_pixel, QRgb sprite_pixel, QMap<QRgb, QRgb> & like_matches );

	public:
		Scan(QImage * mapImage, SpriteSheet * spriteSheet );
		void doScan() { run(); }

		void mergeHits( QList<DisplayObject*> & global_hits) { global_hits.append(hits); }

	protected:
		void run();

};

#endif // SCAN_HPP
