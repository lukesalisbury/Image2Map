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
#include "mapsprite.hpp"
#include <QDebug>
#include <QBuffer>

MapSprite::MapSprite(uint16_t checksum, QImage image, uint16_t width, uint16_t height) : QImage(image)
{
	id = checksum;
	mask = 0;
	remove = false;
	w = width;
	h = height;
	replace_id = "";
	flipmode = 0;

}

MapSprite::MapSprite( QTextStream & in )
{
	in >> id;
	in >> w;
	in >> h;
	in >> mask;
	in >> replace_id;
	in >> remove;
	in >> flipmode;

	replace_id = replace_id.simplified();


	QVariant img(in.readLine());
	this->loadFromData( QByteArray::fromBase64(img.toByteArray()), "PNG");
}


void MapSprite::Save( QTextStream & out )
{
	QByteArray ba;
	QBuffer buffer(&ba);
	buffer.open(QIODevice::WriteOnly);
	this->save(&buffer, "PNG");

	out << this->id << "\t";
	out << this->w << "\t";
	out << this->h << "\t";
	out << this->mask << "\t";
	out << this->replace_id << "\t";
	out << this->remove << "\t";
	out << this->flipmode << "\t";
	out << ba.toBase64() << "\n";
}
