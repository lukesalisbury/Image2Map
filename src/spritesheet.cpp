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
#include "spritesheet.hpp"
#include <QDebug>
#include "spritematchdialog.hpp"
#include <iostream>
#include "spritechooser.hpp"
#include <QtXml/QDomDocument>

SpriteSheet::SpriteSheet(QFileInfo file)
{
	this->file = file;
	this->image = new QImage(file.path() + "/" + file.completeBaseName());
	this->name = this->file.completeBaseName();

	/*
	QFile stream(file.filePath());
	if ( stream.open(QIODevice::ReadOnly | QIODevice::Text) )
	{
		QByteArray line;
		QList<QByteArray> data;
		while ( !stream.atEnd() ) {
			line = stream.readLine();
			data = line.split('\t');
			if ( data.size() >= 8 )
			{
				// SpriteSheet Format (Tab separated values)
				// #Sprite Name, Flag, Mask Value/File, Default Entity, X, Y, Width, Height
				// #Sprite Name, Flag, Mask Value/File, Default Entity, Animation Keyword, Frame length in ms, [Sprites]
				// - * mask is either a sprite name or solid color
				// - ms: milliseconds for each sprite
				QRect rect;
				QString name = data.at(0);

				rect.setRect(data.at(4).toInt(), data.at(5).toInt(), data.at(6).toInt(), data.at(7).simplified().toInt());

				this->items.insert( name, rect );
			}

		}
	}
	stream.close();
	*/

	QDomDocument doc("");
	QFile stream(this->file.filePath());
	if ( stream.open(QIODevice::ReadOnly | QIODevice::Text) )
	{
		doc.setContent(&stream);

		QDomElement rootElement = doc.documentElement();
		for( QDomNode node = rootElement.firstChildElement("sprite"); !node.isNull(); node = node.nextSibling() )
		{
			QDomElement element = node.toElement();
			QDomElement position = element.firstChildElement("position");
			QRect rect;
			QString name = element.attribute("name");

			rect.setRect( position.attribute("x","0").toInt(),
						  position.attribute("y","0").toInt(),
						  position.attribute("w","0").toInt(),
						  position.attribute("h","0").toInt()
						  );

			this->items.insert( name, rect );
		}
	}
	stream.close();
/*
	qDebug() << "Sprites on " << file.completeBaseName() << " " << items.size();
	QHashIterator<QString, QRect> i(items);
	while (i.hasNext()) {
		i.next();
		qDebug() << i.key() << ": " << i.value();
	}
*/
}

int SpriteSheet::spriteCount()
{
	return this->items.size();
}

QImage  SpriteSheet::GetImage(QRect rect)
{
	return this->image->copy(rect);

}

