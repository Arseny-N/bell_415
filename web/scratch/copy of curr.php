<html>
<head>
  <meta charset="UTF-8"/>
  <link type="text/css" rel="stylesheet" href="style.css">
</head>
<body>
	<div class="entry">
		<h1>415_bell</h1>
	</div>
<?php

static $tt_list;

$armed = &$tt_list[0];
$vcurr = &$tt_list[0];                  // The shown tt entry
$edit_flag = false;
$action = 'show_curr';


function ini_time_table()
{
		global $tt_list;
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
		$string = $tt_list[$j]['name'] . ':' . $tt_list[$j]['armed'];
		$size = count($tt_list[$j]['tt']);
		for( $i = 0; $i < $size; $i ++ ) {
			$string .= ":" . $tt_list[$j]['tt'][$i];		
		}
		fputs($fp, $string);				
	}
	fclose($fp);
	unset($tt_list);
}
function is_armed($tt)
{
	return $tt['armed'];
}

function mk_tt($type, &$victim_tt) 
{		
	global $vcurr, $edit_flag;
	global $armed, $tt_list;

	switch($type) {
		
		case 'show_curr':
			unset($GLOBALS['vcurr']);
			$GLOBALS['vcurr'] = &$tt_list[0];
			break;
			
		case 'clone':				
			$new_tt = $victim_tt;
			$tt_list[] = $new_tt;			
			break;
						
		case 'show':			
			unset($GLOBALS['vcurr']);
			$GLOBALS['vcurr'] = $victim_tt;			
			break;
			
		case 'arm':
			echo "<br>    ";
			var_dump($tt_list);
			$victim_tt['armed'] = true;	
			$victim_tt['name'] = "min";
			$armed['armed'] = false;
			
			unset($GLOBALS['armed']);
			unset($GLOBALS['vcurr']);
			$GLOBALS['armed'] = $victim_tt;
			$GLOBALS['vcurr'] = $victim_tt;			
			echo "<br>    ";
			var_dump($tt_list);
			break;
			
		case 'edit':
			unset($GLOBALS['vcurr']);	
			$GLOBALS['vcurr'] = $victim_tt;			
			$edit_flag = true;			
			break;
			
		case 'submit':
			$size = count($_GET);			
			for( $i = 0; $i < $size; $i ++ ) {
				if($_GET[$i] != "") {
					$vcurr['tt'][$i] = $_GET[$i];	
					}
				}
			break;
			
		}	
	}
	
	ini_time_table();
	
	
	foreach ( $tt_list as $entry ) {
		if (isset($_GET[$entry['name']])) {
			//echo "SET " . $_GET[$entry['name']] . " " . $entry['name']. "   ";
			$action = $_GET[$entry['name']];		
			break;
		} 
	}
	mk_tt($action, $entry);
	
echo "Dumping POST " ;var_dump($_POST);echo "<br>";
echo "Dumping GET ";var_dump($_GET);echo "<br>";	
	
?>
	<table>
		<form action="curr.php" type="get">
			<tr><td class="bg">
				
				<table class="main">				
					<tr>
						<td class="entry" colspan="4">
							<?php echo 'name: ' . $vcurr['name'] . '  (' . ( is_armed($vcurr) ? "armed" : "not armed") . ')'; ?>
						</td>
					</tr>
					<tr>
						<td class="entry">n</td>
						<td class="entry">Lesson</td>
						<td class="entry">Time</td>
					</tr>				
					<?php					
						
						$curr = $vcurr['tt'];
						$size = count($curr);
						for( $i = 0; $i < $size; $i ++ ) {
							
							echo '<tr class="entry">' . "\n" ;						
							echo '<td align="center" class="entry">' . $i . '</td>' . "\n" ;
							
							if(($i%2) == 0) {
								echo '<td align="center" class="entry" rowspan="2">' . ($i/2+1) . '</td>';
							} 	
							
							echo '<td align="center" class="entry">';
							
							if(!$edit_flag) {
								echo $curr[$i]; 						
							} else {
								echo '<input type="text" name="' . $i . '" placeholder="' . $curr[$i] . '" />';
							}
							
							echo '</td></tr>' . "\n" ;
						}				
					?>				
				</table>
			<tr><td>				
				<?php if(!$edit_flag) { ?>
					<input type="submit" name=<?php echo $vcurr['name'];?> value="clone"/>
					<input type="submit" name=<?php echo $vcurr['name'];?> value="edit"/>
					<input type="submit" name=<?php echo $vcurr['name'];?> value="arm" />
					
				<?php } else { ?>
					<input type="submit" name=<?php echo $vcurr['name'];?> value="submit"/>
					<input type="submit" name=<?php echo $vcurr['name'];?> value="reset"/>
					
				<?php } ?>			
			</td></tr>			
			</td></tr>					
		</form>
		
	</table>
	
	
	
			
				
	<table width="50%">
		<tr><td class="bg">
			
			<table class="main">
				<tr><td class="entry" colspan="4">All Profiles:</td></tr>
				<tr>
					<td class="entry">Name</td>
					<td class="entry">First Lesson</td>
					<td class="entry">Armed</td>
					<td class="entry">Action</td>
				</tr>
			<?php
				$ind = 0;
				foreach ( $tt_list as $entry ) {
				
					echo '<tr class="entry">';
					echo '<td align="center" class="entry">' . $entry['name'] . '</td>' . "\n";
					echo '<td align="center" class="entry">' . $entry['tt'][0] . '-' .  $entry['tt'][1] . '</td>' . "\n";	
					echo '<td align="center" class="entry">' . (is_armed($entry)? 'yes' : 'no') . '</td>' . "\n";
			?>
					<form action="curr.php?ename=1" type="get">
						<td align="center" class="entry">
							<input type="submit" name=<?php echo $entry['name'];?> value="arm" />
							<input type="submit" name=<?php echo $entry['name'];?> value="show"/>
						</td>
					</form>
			<?php
				
					echo '</tr>';
					$ind++;
				}			
			?>
			</table>
			</td>
				</tr>
			</table>
	<?php fini_time_table(); ?>
	<br><br><br><br><br><br>
	
</body>
</html>
