<?php
function ini_time_table()
{
		global $tt_list, $vcurr;
		$fd = fopen("/usr/local/tfile", "r");
		
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
		fclose($fd);
}
function fini_time_table()
{
	global $tt_list;
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
