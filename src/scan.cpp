#include "scan.hpp"

#include <iostream>
#include <QProgressDialog>
#include <QTextStream>

Scan::Scan(QImage * mapImage, SpriteSheet * spriteSheet)
{
	this->map = mapImage;
	this->sheet = spriteSheet;
}

bool Scan::MatchPixel( QRgb map_pixel, QRgb sprite_pixel, QMap<QRgb, QRgb> & like_matches )
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

bool Scan::FindMatch( QImage * map, QImage sprite, QRect rect, QList<QRect> & local_hits )
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

void Scan::run( )
{
	QString name;
	QHashIterator<QString, QRect> q(sheet->items);
	QList<QRect> local_hits;
	std::cout << std::unitbuf;
	while (q.hasNext())
	{

		std::cout << ".";
		q.next();

		/* Set sprite name */
		name = sheet->name + ":" + q.key();

		QImage sprite = sheet->GetImage(q.value());
		QRgb sprite_pixel;
		bool has_content = false; //Check if image is empty

		for ( int ix = 0; ix < sprite.width(); ix++ )
		{
			for ( int iy = 0; iy < sprite.height(); iy++ )
			{
				if ( !sprite.valid(ix,iy) )
					break;
				sprite_pixel = sprite.pixel(ix,iy);
				if ( qAlpha(sprite_pixel) == 255 && sprite_pixel != 0xFF48a048)
				{
					has_content = true;
					break;
				}
			}
		}

		if ( has_content )
		{
			FindMatch(map, sheet->GetImage(q.value()), q.value(), local_hits );
		}
	}

	std::cout << std::endl;


	/* Manage Local hits */
	QListIterator<QRect> r(local_hits);
	while (r.hasNext())
	{
		QRect area = r.next();
		DisplayObject * object = new DisplayObject( name, area );
		hits.append(object);

	}
	local_hits.clear();
}
