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
#include "imageconvert.hpp"

ImageConvert::ImageConvert( QFile & db )
{
	Load(db);
}

ImageConvert::ImageConvert( QString file, QFile & db )
{
	fileinfo.setFile(file);

	QImage * img = new QImage( file );
	QString last_sprite = "";
	quint16 checksum = 0;
	tile_width = 32;
	tile_height = 32;
	int my = 0;
	int mx = 0;
	int lx = 0;

	MapObject * last = NULL;
	while ( my < img->height() )
	{
		mx = 0;
		last_sprite = "";
		lx = 0;
		while ( mx < img->width() )
		{
			/*
			xx
			xx

			xx
			--

			x-
			x-

			x-
			--
			*/

			QImage tile = img->copy(mx,my,tile_width,tile_height);
			checksum = qChecksum( (char *) (tile.bits()), tile.byteCount() );
			MapObject * current = NULL;
			if ( !objects.isEmpty() )
			{
				last = objects.last();
				if ( !last->pushWidth(checksum) )
				{
					current = new MapObject( checksum, mx, my );
					objects.push_back(current);
				}
			}
			else
			{
				current = new MapObject( checksum, mx, my );
				objects.push_back(current);
			}
			if ( !sprites.contains(checksum) )
			{
				sprites.insert(checksum, new MapSprite(checksum, tile, 16,16) );
			}
			mx += tile_width;
		}
		my += tile_height;
	}

}

void ImageConvert::Load( QFile & db )
{
	if (db.open(QFile::ReadOnly) )
	{
		QTextStream in(&db);
		while ( !in.atEnd() )
		{
			MapSprite *sprite = new MapSprite( in );
			if ( !sprites.contains(sprite->id) )
			{
				sprites.insert(sprite->id, sprite );
			}
			else
			{
				delete sprite;
			}
		}
	}
	db.close();

}
void ImageConvert::Save( QFile & db )
{
	if (db.open(QFile::WriteOnly | QFile::Truncate) )
	{
		QTextStream out(&db);
		QHashIterator<quint16, MapSprite*> q(sprites);
		while (q.hasNext()) {
			q.next();
			q.value()->Save(out);
		}
	}
	db.close();

}
/*

// generate sprite file
$q = count($sprite_array);
$dh = $th * intval($q/16);
$dh += $th;
$dw = $tw * 16;
$dest_text = fopen('sprites/sprites.png.txt', 'w');
$dest = imagecreatetruecolor ( $dw, $dh  );
echo 'Creating sprites/sprites.png ' .$dw .'x'. $dh . ' ' . count($sprite_array) . " sprites\n";
foreach ($sprite_array as $i => $name)
{
	fprintf($dest_text, '%s	0	%d	0	%d	%d	%d	%d' . "\n", $name, (isset($mask_array[$name]) ? 0:255), intval($i%16)*$tw, intval($i/16)*$th, $tw, $th);
	$tile = imagecreatefrompng ('temp/' . $name . '.png' );
	imagecopy ( $dest, $tile, intval($i%16)*$tw, intval($i/16) * $th, 0, 0, $tw, $th );
	imagedestroy($tile);
	//echo 'Removing temp/' . $name . '.png'."\n";
	//unlink('temp/' . $name . '.png');
}
imagepng($dest, 'sprites/sprites.png', 9);

*/


