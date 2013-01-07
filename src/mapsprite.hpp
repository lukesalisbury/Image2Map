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
#ifndef MAPSPRITE_HPP
#define MAPSPRITE_HPP

#include <inttypes.h>
#include <QImage>
#include <QString>
#include <QVariant>
#include <QTextStream>

class MapSprite : public QImage
{
public:
	explicit MapSprite(uint16_t checksum, QImage image, uint16_t width, uint16_t height);
	MapSprite(QTextStream &in);

	void Save(QTextStream &out);

	uint16_t id;
	uint16_t w;
	uint16_t h;
	uint16_t mask;
	QString replace_id;
	char remove;
	char flipmode;

signals:

public slots:

};

#endif // MAPSPRITE_HPP
