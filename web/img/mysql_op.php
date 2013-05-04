<?php
include_once 'print_funcs.php';
	/*
		
profiles(
	  id INT NOT NULL AUTO_INCREMENT, 
	  PRIMARY KEY(id),
	  name VARCHAR(30), 
	  armed BOOL
);
rings (
       id INT NOT NULL AUTO_INCREMENT, 
       PRIMARY KEY(id), 
       profile_id INT NOT NULL,
       ring_time TIME
);
rules (
       name VARCHAR(30), 
       PRIMARY KEY(name),
       profile_id INT NOT NULL,
       rule_date DATE
);

overrides(
       over_date DATE,
       profile_id INT NOT NULL
);
	 */


function connect_to_mysql($db_name)
{	
	$link = mysql_connect("localhost","arseni",'1') or err_box('mysql_connect', 'mysql');	
	return mysql_select_db($db_name, $link) or err_box('select db '.$db_name, 'mysql');
}

function mk_query($query)
{
	$result = mysql_query($query) or err_box($query,'mysql');
	return $result;
}
function foreach_query($query,$printfunc,$arg=NULL) 
{
	$result = mk_query($query);
	while($row = mysql_fetch_array($result)) {
		$printfunc ($row,$arg);
	}
}
function get_query($query)
{
	$result = mk_query($query);
	$ret = NULL;
	while($row = mysql_fetch_array($result)) {
		$ret [] = $row;
	}
	return $ret;
}




function print_rings($prof_id)
{
	return foreach_query("SELECT * FROM rings WHERE profile_id='".$prof_id."' GROUP BY ring_time",'print_ring');
}
function print_profiles()
{
	return foreach_query('SELECT * FROM profiles','print_profile');
}
function print_rules()
{
	return foreach_query('SELECT * FROM rules','print_rule');
}
function print_overrides()
{
	return foreach_query('SELECT * FROM overrides GROUP BY over_date','print_override');
	
}




function get_ring($prof_id)
{
	return get_query("SELECT * FROM rings WHERE profile_id='".$prof_id."' GROUP BY ring_time");
}
function get_ring_time($prof_id,$time)
{
	return get_query("SELECT * FROM rings WHERE profile_id='".$prof_id."' HAVING ring_time='".$time."'");
}
function get_profile($prof_id)
{
	return get_query("SELECT * FROM profiles WHERE id='".$prof_id."'")[0];
}
function get_profile_by_name($name)
{
	return get_query("SELECT * FROM profiles WHERE name='".$name."'")[0];
}
function get_ids($prof_id)
{
	return get_query("SELECT id FROM rings WHERE profile_id='".$prof_id."'");
}
function get_profile_name_by_id($id)
{
	return get_profile($id)['name'];
}
function get_profile_id_by_name($name)
{
	return get_profile_by_name($name)['id'];
}
function get_armed_profile()
{
	return get_profile(get_rule_profile_id_by_day(today()));
}
function get_rule_profile_id_by_day($day)
{
	return get_query("SELECT profile_id FROM rules WHERE rule_day='".mk_day($day)."'")[0]['profile_id'];
}



function add_ring($prof_id,$time)
{
	if(!get_ring_time($prof_id,$time)) {
		return mk_query("INSERT INTO rings (ring_time,profile_id) VALUES('".$time."','".$prof_id."')");
	}
	return false;
}
function add_profile($name)
{
	return mk_query("INSERT INTO profiles (name) VALUES('".$name."')");
}
function add_override($profn,$date)
{	
	
	return mk_query("INSERT INTO overrides (over_date,profile_id) 
					 VALUES('".$date."','".get_profile_id_by_name($profn)."')");
	return true;
}

function update_ring_time($ring_id,$time)
{
	return mk_query("UPDATE rings SET ring_time='".$time."' WHERE id='".$ring_id."'");
}
function update_ring_duration($ring_id,$time)
{
	return mk_query("UPDATE rings SET ring_duration='".$time."' WHERE id='".$ring_id."'");
}
function update_profile_name($prof_id,$name)
{
	return mk_query("UPDATE profiles SET name='".$name."' WHERE id='".$prof_id."'");
}
function update_rule($rday, $profn)
{
	return mk_query("UPDATE rules SET profile_id='".get_profile_id_by_name($profn)."' WHERE rule_day='".mk_day($rday)."'");
}



function copy_ring_entry($entry, $new_id)
{		
	return add_ring($new_id,$entry['ring_time']);
}




function clone_rings($id,$new_id)
{
	return foreach_query("SELECT * FROM rings WHERE profile_id='".$id."'",'copy_ring_entry',$new_id);
}
function clone_profile($id,$new_name)
{
	add_profile($new_name);
	$prof = get_profile_by_name($new_name);	
	return clone_rings($id, $prof['id']);
	
}




function cnt_profiles()
{
	return get_query("SELECT COUNT(*) FROM profiles")[0]['COUNT(*)'];
}


function mk_day($day)
{
	return $day;
}
function today()
{
	return mk_day(date('N'));
}

function arm_profile($id)
{	
	echo 'Today:' . today() .'Id'.$id;
	return mk_query("UPDATE rules SET profile_id='".$id."' WHERE rule_day='".mk_day($rday)."'");
}
function is_armed_profile($id)
{
	return (get_rule_profile_id_by_day(today()) == $id);
}




function drop_profile($id)
{
	if(!is_armed_profile($id)) {
		mk_query("DELETE FROM profiles WHERE id='".$id."'");
		mk_query("DELETE FROM overrides WHERE profile_id='".$id."'");
		mk_query("DELETE FROM rules WHERE profile_id='".$id."'");
		return mk_query("DELETE FROM rings WHERE profile_id='".$id."'");
	}
	return false;
}
function drop_override($id)
{
	return mk_query("DELETE FROM overrides WHERE id='".$id."'");
}
function del_ring($prof_id)
{
	$id = get_query("SELECT MAX(id) FROM rings WHERE profile_id='".$prof_id."'")[0]['MAX(id)'];
	return mk_query("DELETE FROM rings WHERE id='".$id."'");
	
}
?>
