<?php
include_once 'site.php';

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
	
		table_open('Звонки',['№','Урок','Время активации','Продолжительность']);
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
	echo '<tr><td align="left" class="entry">
		  <input type="text" name="ov_date"/>
		  </td></tr>';
	echo '<tr><tr>';
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
?>
