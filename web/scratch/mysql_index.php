<html>
<head>
  <meta charset="UTF-8"/>
  <link type="text/css" rel="stylesheet" href="style2.css">
</head>
<body>
<?php

$tt_list;

$vcurr = NULL;                  // The shown tt entry
$edit_flag = false;
include 'fops.php';    //File operations
include 'dops.php';       //Daemon operations 
function is_armed($tt)
{
	return $tt['armed'];
}

function &find_descr($s,$descr)
{
	global $tt_list;
	foreach ( $tt_list as &$entry ) {
		if(strcmp( (string) $entry[$descr],(string) $s) == 0) {
			return $entry;
		}
	}
	return NULL;
}
function &find_by_name($s) 
{
	return find_descr($s,'name');
}
function &find_armed() 
{
	return find_descr(true,'armed');
}
	ini_time_table();

	if(isset($_GET['show'])) {
		
		unset($vcurr);
		$vcurr = &find_by_name($_GET['show']);
	}
	if(isset($_GET['edit'])) {
		unset($vcurr);
		$vcurr = find_by_name($_GET['edit']);
		$edit_flag = true;
	}
	if(isset($_GET['clone'])) {
		unset($vcurr);
		$vcurr = find_by_name($_GET['clone']);
		$new_tt = $vcurr;
		$tt_list[] = $new_tt;	
	}
	if(isset($_GET['arm'])) {
		find_armed()['armed'] = false;
		unset($vcurr);	
		$vcurr = &find_by_name($_GET['arm']);
		find_by_name($_GET['arm'])['armed'] = true;
		daemon_arm($vcurr);
	}
	if(isset($_GET['add'])) {
		unset($vcurr);
		$vcurr = find_by_name($_GET['add']);
		$edit_flag = true;
		find_by_name($_GET['add'])['tt'][] = "00.00";
	}
	if(isset($_GET['submit'])) {
		unset($vcurr);
		$vcurr = &find_by_name($_GET['submit']);
		
		$size = count($_GET);			
		for( $i = 0; $i < $size; $i ++ ) {
			if($_GET[$i] != "") {
				$vcurr['tt'][$i] = $_GET[$i];	
				}
			}
		if ($_GET['new_name'] !=  "") {
			$vcurr['name'] = $_GET['new_name'];
		}
	}
	
//echo "Dumping POST " ;var_dump($_POST);echo "<br>";
//echo "Dumping GET ";var_dump($_GET);echo "<br>";	
	
?>
<table class="main_box">
	<tr><td class="main_box">
		
	<table class="inboxer">
		<form action="index.php" type="post">
			<tr><td class="bg">
				
				<table class="main">				
					<tr>
						<div class="table_heads">
							<td align="center" class="entry">Profile</td>
							<?php 
								if (!$edit_flag) {
									echo '<td align="left" class="entry" colspan="2">' . $vcurr['name'] . '</td>';									
								} else {
									echo '<td align="left" class="entry" colspan="2">
										  <input type="text" name="new_name" 
															 placeholder="' . $vcurr['name'] . '" />' . '</td>';
								}
								?>
						<tr>			
								<td align="center" class="entry">State</td>
								<td align="left" class="entry" colspan="2"> <?php echo  ''. ( is_armed($vcurr) ? "armed" : "not armed") . ''; ?></td>
						</tr>	
							</td>
							</div>
						</td>
					</tr>
					<tr>
						<td align="center" class="entry">n</td>
						<td align="center" class="entry">Lesson</td>
						<td align="center" class="entry">Time</td>
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
		</table>
		<table class="inboxer">
			<tr><td align="center" class="buttons">
				<?php if(!$edit_flag) { ?>
					
					<button type="submit" name="edit" value=<?php echo $vcurr['name'];?>>edit</button>		
					<button type="submit" name="clone" value=<?php echo $vcurr['name'];?>>clone</button>		
					<button type="submit" name="arm" value=<?php echo $vcurr['name'];?>>arm</button>		
					
				<?php } else { ?>
					<button type="submit" name="submit" value=<?php echo $vcurr['name'];?>>submit</button>		
					<!--<button type="submit" name="add" value=<?php echo $vcurr['name'];?>>add</button>-->
				<?php } ?>			
			</td></tr>			
			</td></tr>					
	
		</form>
	</table>
		
	
	<table class="inboxer">
		<tr><td class="bg">
			
			<table class="main">
				<tr><td align="center" class="entry" colspan="4"><div calss="table_heads">All Profiles</div></td></tr>
				<tr>
					<td align="center" class="entry">Name</td>
					<td align="center" class="entry">First Lesson</td>
					<td align="center" class="entry">Armed</td>
					<td align="center" class="entry">Action</td>
				</tr>
			<?php
				$ind = 0;
				$once = true;
				foreach ( $tt_list as $entry ) {
				
				if($vcurr === $entry && $once) {
					$once = false;		
					echo '<tr class="shown_entry">';
				} else {
					echo '<tr class="entry">';
				}
					echo '<td align="center" class="entry">' . $entry['name'] . '</td>' . "\n";
					echo '<td align="center" class="entry">' . $entry['tt'][0] . '-' .  $entry['tt'][1] . '</td>' . "\n";	
					echo '<td align="center" class="entry">' . (is_armed($entry)? 'yes' : 'no') . '</td>' . "\n";
			?>
					<form action="index.php" type="get">
						<td align="center" class="entry">
							<button type="submit" name="show" value=<?php echo $entry['name'];?>>show</button>
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
			
	</td></tr>
</table>
	<?php fini_time_table(); ?>
	
</body>
</html>
