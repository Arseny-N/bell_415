<?php
include_once 'site.php';
function print_sub_conf()
{
	global $edit_flag;
	$SUB_RING_CONF = get_query("select * from sub_conf")[0];
	
	table_open('SUB Bell Config', ["value", "Result"]);
		echo '<tr><td>Before:</td>';
		if($edit_flag)
				echo '<td class="entry"><input type="text" name="sub_before" placeholder="'.$SUB_RING_CONF['ring_before'].'" /> </td>';
		else
				echo '<td class="entry">'.$SUB_RING_CONF['ring_before'].'</td>';
		
		echo '</tr><tr><td>Duration:</td>';
		
		if($edit_flag)
			echo'<td class="entry"><input type="text" name="sub_for" placeholder="'.$SUB_RING_CONF['ring_for'].'" /> </td>';
		else
			echo '<td class="entry">'.$SUB_RING_CONF['ring_for'].'</td>';
		echo '</tr>';
		
		echo '</tr><tr><td>Global Enable:</td>';
		//var_dump($SUB_RING_CONF);
		if($edit_flag) {
			echo '<td class="entry"><select size="1" name="sub_global">';	
				echo '<option value="on" '.($SUB_RING_CONF['global_enable']?'selected="true"':"") .'>On</option>';
				echo '<option value="off" '.(!$SUB_RING_CONF['global_enable']?'selected="true"':"") .'>Off</option>';
			echo '</select>	</td>';		
				
		} else
			echo '<td class="entry">'.($SUB_RING_CONF['global_enable'] ? "enabled" : "disabled").'</td>';
				

		echo '</tr>';
		
	table_close();
}
function print_panic_ctrl()
{
	
}
function print_buttons()
{
	global $shown_tt, $edit_flag;	
	if(!$edit_flag) { 
		echo '	  <button type="submit" name="edit"  value='. $shown_tt['id'].'>редактировать</button>		
			  <button type="submit" name="clone" value='. $shown_tt['id'].'>клонировать</button>					  
			  <button type="submit" name="drop"  value='. $shown_tt['id'].'>удалить</button>';
	} else { 
		echo '
			  <button type="submit" name="add" value='. $shown_tt['id'].'>добавить пустой</button>
			  <button type="submit" name="submit" value='. $shown_tt['id'].'>сохранить</button>		
			  <button type="submit" name="del" value='. $shown_tt['id'].'>удалить последний</button>			  
			  ';
	}
	

}

function print_profiles_list()
{
	
	table_open('Все профили', ["Имя","Перый урок","Используется","Показать"]);
#		form_open();
			print_profiles();			
#		form_close();
	table_close();
	
}
function print_shown_tt()
{
	global $edit_flag, $shown_tt;
	form_open();	
	
	echo '<table><tr class="profile_name">';				
									
	if (!$edit_flag) {
		echo '<td class="profile_name">' ."Имя показываемого профиля: ". $shown_tt['name'] . '</td>';
	} else {
		echo '<td class="profile_name"><input type="text" name="new_name" placeholder="' . $shown_tt['name'] . '" />' . '</td>';
	}
	echo '</tr></table>';
	
		table_open('Звонки',['№','Урок','Время активации','Продолжительность','Sub ring']);
			print_rings($shown_tt['id']);
		table_close();
	form_close();
	
}

function print_rules_list()
{	
	table_open('Правила',["День недели","Имя профиля","Изменение профиля","Изменить"]);
		form_open();	
			print_rules();		
		form_close();
	table_close();
		
}

function print_new_ov_box()
{	
	form_open();
	echo '<tr><td>Дата</td>
		<td align="left" class="entry">
		<input type="text" name="ov_date"/>
		</td>
	</tr>';
	echo '<tr><td>Профиль</td><td>';
	select_profile_elem();
	echo '</td></tr>';
	echo '<tr><td><button type="submit" name="new_ov" value="blank">Изменить</button></td></tr>';
	form_close();
	return 0;
}
function print_overrides_list()
{
	global $new_ov_flag,$argv;
	
	
	table_open('Исключения',["День активации","Имя профиля","Удалить"]);	
		form_open();
			print_overrides();		 
		form_close();
	table_close();
	echo '<table>';
	if($new_ov_flag) {		
		print_new_ov_box();
	} else {
		form_open();
			echo '<tr><td><button type="submit" name="ov_blank" value="blank">Новый</button></td></tr>';
		form_close();
	}
	echo'</table>';
}
function select_profile_elem($rule_day)
{
	$id = get_rule_profile_id_by_day($rule_day);	
	echo '<select size="1" name="select_elem">';
	$result = mk_query("SELECT * FROM profiles");
	while($row = mysql_fetch_array($result)) {		
		echo '<option value="'.$row['name'].'" '. ($row['id'] == $id ? ' selected="" ' : '') .'>'.$row['name'].'</option>';
	}							
	echo '</select>	';
}

class profiles
{
	public function print_elem($p)
	{	
		global $argv;
		echo '<tr class="entry">';

		$ring = get_ring($p['id']);
		echo '<td align="center" class="entry">' . $p['name'] . '</td>' . "\n";
		echo '<td align="center" class="entry">' . $ring[0]['ring_time'] . '-' .  $ring[1]['ring_time'] . '</td>' . "\n";	
		echo '<td align="center" class="entry">' . (is_armed_profile($p['id'])? 'да' : 'нет') . '</td>' . "\n";
		echo '		
				<td align="center" class="entry">
					<button type="submit" name="show" value='.$p['id'].'>Показать</button>
				</td>';		
		
		echo '</tr>';
	}
}
$days = ['Unknown value','Понедельник', 'Вторник','Среда','Четверг','Пятница','Суббота','Воскресение'];
function num_to_day($day)
{
	global $days;		
	return $days[$day];
}
class rules
{

	public function print_elem($rule)
	{	
		global $argv;
		echo '<tr class="entry">';					
		echo '<td align="center" class="entry">' . num_to_day($rule['rule_day']) . '</td>' . "\n";	
		echo '<td align="center" class="entry">' . get_profile_name_by_id(($rule['profile_id'])) . '</td>' . "\n";
		echo '<td align="center" class="entry">';
		form_open();
		select_profile_elem($rule['rule_day']);
		echo '</td>
				<td align="center" class="entry">
					<button type="submit" name="rule_submit" value='.$rule['rule_day'].'>Изменить</button>
				</td>
			</tr>';
		form_close();
	}
}
class overrides {
	public function print_elem($o)
	{
		global $argv;
	
		echo '<tr class="entry">';
	
		echo '<td align="center" class="entry">' . $o['over_date'] . '</td>' . "\n";	
		echo '<td align="center" class="entry">' . get_profile_name_by_id($o['profile_id']) . '</td>' . "\n";
		
		echo '		
				<td align="center" class="entry">
					<button type="submit" name="ov_drop" value='.$o['id'].'>Удалить</button>
				</td>
			 ';		
			
		echo '</tr>';
	}
}

class rings {
	public function print_elem($ring)
	{
		static $i = 0;
		global $edit_flag;
	
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

		echo '</td>' . "\n" ;
		echo '<td align="center" class="entry">';
								
		if(!$edit_flag) {
			echo (have_sub($ring)?"yes":"no");
		} else {
			echo '<input type="checkbox" name="sub_ring['.$ring['id'].']" value="chch_box" ' . (have_sub($ring)?'checked="true"':"") .' >';
		}
		
			
		echo '</td></tr>' . "\n" ;
		$i ++;
	}
}

?>
