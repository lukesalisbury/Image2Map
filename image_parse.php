<?PHP
ini_set("memory_limit","256M");
$sprite_array = array();
$mask_array = array(
"3040a46297a73b5db6acc8cb268b497f" => 0,
"5942d441dfe7b315164567df7291cdbe" => 0,
"d7bb921bb8f2f9fc0cb354062f712be3" => 0
);

$replace_array = array(

);

$tw = $th = 16;

MakeMap( "Blacksmith" );
/*
MakeMap( "DeathMountain(East)" );
MakeMap( "DeathMountain(West)" );
MakeMap( "DesertOfMystery" );
MakeMap( "EasternPalace" );
MakeMap( "GreatSwamp" );
MakeMap( "HyruleCastle" );
MakeMap( "KakarikoSouth" );
MakeMap( "KakarikoVillage" );
MakeMap( "LakeHylia" );
MakeMap( "LinksHouse" );
MakeMap( "LostWoods" );
MakeMap( "SmallForest" );
MakeMap( "ZorasDomain" );
MakeMap( "LumberJacks" );
MakeMap( "Cemetery" );
MakeMap( "FortureHut" );
MakeMap( "LostWoodsEntrance" );
*/


/* generate sprite file */
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

function MakeMap( $file )
{
	global $sprite_array,$tw, $th;

	$src = imagecreatefrompng ( $file . '.png' );
	$srcw = imagesx($src);
	$srch = imagesy($src);
	$mw = $srcw;
	$mh = $srch;

	$mx_count = 1;
	$my_count = 1;

	$x = 0;
	$y = 0;
	$last_sprite = '';
	$last_sprite_x = 0;

	$my = 0;
	echo('Opening maps/' . $file .'.xml' ."\n");
	$map = fopen('maps/' . $file .'.xml', 'w');
	fprintf($map, '<map xmlns="http://mokoi.sourceforge.net/projects/mokoi"><settings><dimensions width="%d" height="%d" /><color red="0" blue="0" green="0" mode="0" /></settings>' . "\n", $mw / 512 , $mh / 512);
	while ($my < $mh)
	{
		$mx = 0;
		$last_sprite = '';
		$tile_line = array();
		$last_x = 0;
		while ($mx < $mw)
		{
			$tile = imagecreatetruecolor ( $tw, $th );
			imagecopy ( $tile, $src, 0, 0, ($x*$mw)+$mx, ($y*$mh)+$my, $tw, $th );
			imagepng($tile, 'temp/temp.png', 9);
			imagedestroy($tile);

			$md5 = md5_file ( 'temp/temp.png' );
			if ( file_exists('temp/' . $md5 . '.png') )
				unlink('temp/temp.png');
			else
			{
				rename('temp/temp.png', 'temp/' . $md5 . '.png'); 
				$sprite_array[] = $md5;
			}
			if ( $last_sprite != $md5 )
			{
				$last_x = $mx;
				$tile_line[$last_x] = array('name' => $md5, 'width' => 1);
			}
			else
			{
				$tile_line[$last_x]['width']++;
			}
			$last_sprite = $md5;
			$mx += $tw;
		}
		echo '.';
		foreach ($tile_line as $ax => $a)
		{
			fprintf($map, '<object value="sprites.png:%s" type="sprite"><position x="%d" y="%d" w="%d" h="%d" z="%d" /></object>'."\n", $a['name'], $ax, $my, ($a['width'] > 1 ? $a['width']*$tw : 0), ($a['width']>1 ? $th:0), 0);
		}
		$my += $th;
	}
	fprintf($map, '</map>');

	imagedestroy($src);
	echo "\n";

}

?>