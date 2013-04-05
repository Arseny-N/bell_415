<?php

function mysql_init()
{
	
}
function ini_time_table()
{
		global $tt_list, $vcurr, $link;
		$link = mysql_connect();
		if (!$link) {
			die('Not connected : ' . mysql_error());
		}
		$db = mysql_select_db('bell_db' ,$link);
	
		if (!$db) {
			die ('Can\'t use foo : ' . mysql_error());
		}	
		
		
		$ind = 0;
		while (($buf = fgets($fd)) != false ) {
				
			$name = strstr($buf,':',true);						
			$buf = strchr($buf,':');		
			$buf = substr( $buf, 1 );
		
			$armed = strstr($buf,':',true);		
			$buf = strchr($buf,':');
			$buf = substr( $buf, 1 );
			
			
			unset($ent);
			$ent['name'] = $name;
			$ent['armed'] = (bool)$armed;	
			
			
			while( true ) {
				$entry = strstr($buf,':',true);							
				if( $entry == NULL) {				
					$ent['tt'][] = $buf;	
					break;
				}						
				$buf = strchr($buf,':');
				$buf = substr( $buf, 1 );
				$ent['tt'][] = $entry;			
			}	
		
			$tt_list[$ind++] = $ent;			
		}			
		
}
function fini_time_table()
{
	global $tt_list, $link;
	mysql_close($link);
	$fp = fopen("/usr/local/tfile", "w");
	$msize = count($tt_list);
	for( $j = 0; $j < $msize; $j ++ ) {
		unset($string);
		$string = $tt_list[$j]['name'] . ':' . ( $tt_list[$j]['armed'] ? "1" : "0" );
		$size = count($tt_list[$j]['tt']);
		for( $i = 0; $i < $size; $i ++ ) {
			$string .= ":" . $tt_list[$j]['tt'][$i];		
		}
		fputs($fp, $string);				
	}
	fclose($fp);
	unset($tt_list);
}
?>
