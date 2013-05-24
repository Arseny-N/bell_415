<?php
$days = ['Unknown value','Monday', 'Tuesday','Wensday','Thirday','Firsday','Sunday','Saturday'];
function rule_day($day)
{
	global $days;		
	return $days[$day] . $day;
}
function select_profile_elem()
{
	echo '<select size="'.cnt_profiles().'" name="select_elem">';							
	foreach_query("SELECT * FROM profiles",'select_profile_print');
	echo '</select>	';
}
function select_profile_print($pname)
{
	
	#echo "Hey ARMED!!!" . $pname['id'];
	if(is_armed_profile($pname['id'])) {		
		echo '<option value="'.$pname['name'].'" selected="">'.$pname['name'].'</option>';
	}else{
		echo '<option value="'.$pname['name'].'">'.$pname['name'].'</option>';
	}
}
function print_rule($rule)
{
	global $argv;
	echo '<tr class="entry">';					
	echo '<td align="center" class="entry">' . rule_day($rule['rule_day']) . '</td>' . "\n";	
	echo '<td align="center" class="entry">' . get_profile_name_by_id(($rule['profile_id'])) . '</td>' . "\n";
	echo '<td align="center" class="entry">';
	select_profile_elem();
	echo '</td>
			<td align="center" class="entry">
				<button type="submit" name="rule_submit" value='.$rule['rule_day'].'>submit</button>
			</td>
		</tr>';
}



function print_profile($p)
{
	global $argv;
	echo '<tr class="entry">';

	$ring = get_ring($p['id']);
	echo '<td align="center" class="entry">' . $p['name'] . '</td>' . "\n";
	echo '<td align="center" class="entry">' . $ring[0]['ring_time'] . '-' .  $ring[1]['ring_time'] . '</td>' . "\n";	
	echo '<td align="center" class="entry">' . (is_armed_profile($p['id'])? 'yes' : 'no') . '</td>' . "\n";
	echo '		
			<td align="center" class="entry">
				<button type="submit" name="show" value='.$p['id'].'>show</button>
			</td>';		
		
	echo '</tr>';
}



function print_override($o)
{
	global $argv;
	
	echo '<tr class="entry">';
	
	echo '<td align="center" class="entry">' . $o['over_date'] . '</td>' . "\n";	
	echo '<td align="center" class="entry">' . get_profile_name_by_id($o['profile_id']) . '</td>' . "\n";
	
	echo '		
			<td align="center" class="entry">
				<button type="submit" name="ov_drop" value='.$o['id'].'>drop</button>
			</td>
		 ';		
		
	echo '</tr>';
}


function print_ring($ring)
{
	global $edit_flag;
	static $i = 0;
	
	echo '<tr class="entry">' . "\n" ;						
	echo '<td align="center" class="entry">' . $i . '</td>' . "\n" ;
					
	if(($i%2) == 0) {
		echo '<td align="center" class="entry" rowspan="2">' . ($i/2+1) . '</td>';
	} 	
						
	echo '<td align="center" class="entry">';
							
	if(!$edit_flag) {
		echo $ring['ring_time']; 						
	} else {
		echo '<input type="text" name="times[' . $ring['id'] . ']" placeholder="' . $ring['ring_time'] . '" />';
	}
					
	echo '</td>' . "\n" ;
	echo '<td align="center" class="entry">';
							
	if(!$edit_flag) {
		echo $ring['ring_duration']; 						
	} else {
		echo '<input type="text" name="duration[' . $ring['id'] . ']" placeholder="' . $ring['ring_duration'] . '" />';
	}
					
	echo '</td></tr>' . "\n" ;
	$i ++;
}


?>
