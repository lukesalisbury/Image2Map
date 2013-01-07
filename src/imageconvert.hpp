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
#ifndef IMAGECONVERT_HPP
#define IMAGECONVERT_HPP
#include <QDebug>
#include <QString>
#include <QImage>
#include <QList>
#include <QFile>
#include <QVariant>
#include <QFileInfo>

#include "mapobject.hpp"
#include "mapsprite.hpp"

class ImageConvert
{
public:
	ImageConvert( QString file, QFile & db );
	ImageConvert( QFile & db );
	QHash<quint16, MapSprite*> sprites;
	QList<MapObject*> objects;
	int tile_width;
	int tile_height;
	QFileInfo fileinfo;

	void Save( QFile & db );
	void Load( QFile & db );
};

#endif // IMAGECONVERT_HPP
