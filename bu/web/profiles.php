<?php

include_once 'error.php';
include_once 'site.php';
include_once 'show_tt.php';

include_once 'mysql_op.php';
include_once 'dc.php';


$shown_tt = NULL;

$edit_flag = false;
$new_ov_flag = false;

/* load_lib: Thanks to a guy from the php forum :-)*/
function load_lib($n, $f = null) {
    return extension_loaded($n) or 
		   dl(((PHP_SHLIB_SUFFIX === 'dll') ? 'php_' : '') 
		   . ($f ? $f : $n) . '.' . PHP_SHLIB_SUFFIX);
}


function uniq_name($name, $postfix) 
{	
	do {
			$name .= $postfix;
	} while ( get_profile_by_name($name) );
	return $name;
}


function show_tt($name)
{
	global $shown_tt;
	$shown_tt = get_profile($name);		
}

function edit_tt($name)
{
	global $shown_tt, $edit_flag;
	$edit_flag = true;
	$shown_tt = get_profile($name);		
}
function clone_tt($name)
{
	global $shown_tt;
	$shown_tt = get_profile($name);	
			
	$uname = uniq_name($shown_tt['name'],'_clone');		
	clone_profile($shown_tt['id'], $uname);	
	
	$shown_tt = get_profile($uname);						
}
function arm_tt($name)
{
	global $shown_tt;
	$shown_tt = get_profile($name);		
	arm_profile($shown_tt['id']);
	
	$_shown_tt = get_profile($name);
	
	if($_shown_tt != $shown_tt) {
		d_rexec();
	}
	$shown_tt = $_shown_tt;
}
function add_tt($name)
{
	global $shown_tt, $edit_flag;
	$shown_tt = get_profile($name);
		
	$edit_flag = true;
	add_ring($shown_tt['id'],date("H") .":". date("i") .":". date("s")); #or err_box('Unfilled rings.','user');
	
	$shown_tt = get_profile($name);
}
function drop_tt($name)
{
	global $shown_tt;
	$shown_tt = get_profile($name);
	drop_profile($shown_tt['id']) or err_box('Can\'t drop armed time table.','user');
	$shown_tt = NULL;
}
function del_tt($name)
{
	global $shown_tt, $edit_flag;
	$shown_tt = get_profile($name);
	$edit_flag = true;
	del_ring($shown_tt['id']);		
}
function submit_tt($name,$new_name)
{
	global $shown_tt, $edit_flag;
	$shown_tt = get_profile($name);
	
	$ids = get_ids($shown_tt['id']);	
	
	foreach ( $ids as $id ) {	
		
		if($_REQUEST['times'][$id['id']] != "" ) {	
			
			update_ring_time($id['id'],$_REQUEST['times'][$id['id']]);		
		}
		if($_REQUEST['duration'][$id['id']] != "" ) {						
			
			update_ring_duration($id['id'],$_REQUEST['duration'][$id['id']]);		
		}
	}
	
	if ($new_name !=  "") {
		update_profile_name($shown_tt['id'],$new_name);
		$shown_tt = get_profile($new_name);
	}
}
function process_buttons()
{
	global $shown_tt, $edit_flag, $new_ov_flag;
	
	$new_ov_flag = false;
	$edit_flag = false;
	$shown_tt = NULL;
	
	if(isset($_REQUEST['show'])) {
		show_tt($_REQUEST['show']);
	}
	if(isset($_REQUEST['add_rule'])) {
		add_rule($_REQUEST['add_rule']);
	}
	if(isset($_REQUEST['edit'])) {
		edit_tt($_REQUEST['edit']);
	}
	if(isset($_REQUEST['clone'])) {
		clone_tt($_REQUEST['clone']);
	}
	if(isset($_REQUEST['arm'])) {
		arm_tt($_REQUEST['arm']);		
	}
	if(isset($_REQUEST['add'])) {
		add_tt($_REQUEST['add']);
	}
	if(isset($_REQUEST['drop'])) {
		drop_tt($_REQUEST['drop']);
	}
	if(isset($_REQUEST['del'])) {
		del_tt($_REQUEST['del']);
	}
	if(isset($_REQUEST['submit'])) {
		submit_tt($_REQUEST['submit'],$_REQUEST['new_name']);
	}
	if(isset($_REQUEST['rule_submit']) && isset($_REQUEST['select_elem'])) {
		update_rule($_REQUEST['rule_submit'], $_REQUEST['select_elem']);				
		d_rexec();
		error_log('Here all ok');
	}
	if(isset($_REQUEST['ov_blank'])) {
		$new_ov_flag = true;
	}
	if(isset($_REQUEST['new_ov'])) {	
		add_override($_REQUEST['select_elem'],$_REQUEST['ov_date']);
		d_rexec();
	}
	if(isset($_REQUEST['ov_drop'])) {
		drop_override($_REQUEST['ov_drop']);
	}
	return $shown_tt;
}


function is_armed($tt)
{
	return $tt['armed'];
}
function is_shown($tt)
{
	global $shown_tt;
	return $shown_tt === $tt;
}


global $argv;
print_head('style2.css');
	
load_lib('mysql');
connect_to_mysql("ringTimeTable");

process_buttons();
echo '<body dir="ltr">';
echo '<table class="main_box"><tr><td class="navi_box_menu">';	

print_navi_menu();

echo '</td><td class="main_box">';	

if( $shown_tt == NULL )	
	$shown_tt = get_armed_profile();
	
	echo '<form action="'.$argv[0].'" method="post">';	
	sub_box_open('class="bg"');	
		print_shown_tt();		
	sub_box_close();

	sub_box_open('class="buttons" align="center"');	
		print_buttons();		
	sub_box_close();
	echo '</form>';


	sub_box_open('class="bg"');
		print_profiles_list();	
	sub_box_close();
	
	sub_box_open('class="bg"');			
		print_rules_list();	
	sub_box_close();
	
	sub_box_open('class="bg"');	
		print_overrides_list();	
	sub_box_close();
	
echo '</td></tr></table>';
echo '</body>';

?>


