<?php
function print_buttons()
{
	global $shown_tt, $edit_flag;
	
	if(!$edit_flag) { 
		echo '			  
			  <button type="submit" name="edit"  value='. $shown_tt['name'].'>edit</button>		
			  <button type="submit" name="clone" value='. $shown_tt['name'].'>clone</button>					  
			  <button type="submit" name="drop"  value='. $shown_tt['name'].'>drop</button>
			  <button type="submit" name="arm"   value='. $shown_tt['name'].'>arm</button>			  
			  
			  ';
	} else { 
		echo '
			  <button type="submit" name="add" value='. $shown_tt['name'].'>add</button>
			  <button type="submit" name="submit" value='. $shown_tt['name'].'>submit</button>		
			  <button type="submit" name="del" value='. $shown_tt['name'].'>del</button>			  
			  ';
	}
	echo '</form>'; /* Opened in print_shown_tt*/

}
function print_tt_list()
{
	global $main_tt_name;
	global $argv;
	echo <<<_EOB_
			<table class="main">
				<tr><td align="center" class="entry" colspan="4"><div calss="table_heads">All Profiles</div></td></tr>
				<tr>
					<td align="center" class="entry">Name</td>
					<td align="center" class="entry">First Lesson</td>
					<td align="center" class="entry">Armed</td>
					<td align="center" class="entry">Action</td>
				</tr>
_EOB_;
	$ind = 0;
	$once = true;
	
	$query = "SELECT * FROM " . $main_tt_name; 
	$result = mysql_query($query) or die(mysql_error());
		
	while ( $entry = mysql_fetch_array($result)) {	
		if( is_shown($entry) && $once) {
			$once = false;		
			echo '<tr class="shown_entry">';
		} else {
			echo '<tr class="entry">';
		}
		$ring = _get_ring_tt($entry['id']);
		echo '<td align="center" class="entry">' . $entry['name'] . '</td>' . "\n";
		echo '<td align="center" class="entry">' . $ring[1] . '-' .  $ring[2] . '</td>' . "\n";	
		echo '<td align="center" class="entry">' . (is_armed($entry)? 'yes' : 'no') . '</td>' . "\n";
		echo '
		<form action="'.$argv[0].'" type="get">
			<td align="center" class="entry">
				<button type="submit" name="show" value='.$entry['name'].'>show</button>
			</td>
		</form>';
		
		echo '</tr>';
		$ind++;
	}			
	echo <<< _EOB_
			</table>
	
_EOB_;
}
function print_shown_tt()
{
	global $argv;
	global $edit_flag;
	global $shown_tt;
	echo '
		<form action="'.$argv[0].'" $type="post">
			
				<table class="main">				
					<tr>
					<div class="table_heads">
							<td align="center" class="entry">Profile</td>';

						
						
	if (!$edit_flag) {
		echo '<td align="left" class="entry" colspan="2">' . $shown_tt['name'] .
			 '('.( is_armed($shown_tt) ? "armed" : "not armed").')'. '</td>';									
	} else {
		echo '<td align="left" class="entry" colspan="2">
			  <input type="text" name="new_name" 
			 placeholder="' . $shown_tt['name'] . '" />' . '</td>';
	}
	echo <<<_EOB_
				</td>
			</div>
		</td>
	</tr>
	<tr>
		<td align="center" class="entry">n</td>
		<td align="center" class="entry">Lesson</td>
		<td align="center" class="entry">Time</td>
	</tr>		
_EOB_;
	_print_shown_tt($shown_tt['tt']);
	echo <<< _EOB_
		</table>
	
_EOB_;
}
function _print_shown_tt($tt)
{
	global $edit_flag;
	$ring = $tt;
	$size = count($ring);
	for( $i = 0; $i < $size; $i ++ ) {				
		echo '<tr class="entry">' . "\n" ;						
		echo '<td align="center" class="entry">' . $i . '</td>' . "\n" ;
					
		if(($i%2) == 0) {
			echo '<td align="center" class="entry" rowspan="2">' . ($i/2+1) . '</td>';
		} 	
						
		echo '<td align="center" class="entry">';
							
		if(!$edit_flag) {
			echo $ring[$i+1]; 						
		} else {
			echo '<input type="text" name="' . $i . '" placeholder="' . $ring[$i+1] . '" />';
		}
					
			echo '</td></tr>' . "\n" ;
		}				
}
?>
