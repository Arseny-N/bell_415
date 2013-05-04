<?php
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
$sub_tt_prefix = "ring_time_table_";
$main_tt_name = "profiles";
function _drop_ring_tt($id)
{
	global $sub_tt_prefix;
	$query = "DROP TABLE ".$sub_tt_prefix.$id;
	mysql_query($query) or die(mysql_error(). '  ' . $query);
}
function drop_table($tt)
{
	global $main_tt_name;
	
	_drop_ring_tt($tt['id']);
	
	$query = "DELETE FROM ".$main_tt_name." WHERE id='".$tt['id']."'";
	mysql_query($query) or die(mysql_error(). '  ' . $query);
}

function connect_to_mysql($db_name)
{	
	$link = mysql_connect("localhost","arseni",'1') or err_box('mysql_connect', 'mysql');	
	return mysql_select_db($db_name, $link) or or err_box('select db '.$db_name, 'mysql');
}
function _get_ring_tt($id)
{
	global $sub_tt_prefix;
	$query = "SELECT * FROM rings WHERE name='".$id."'" ; 
	$result = mysql_query($query) or or err_box($query, 'mysql');
	$i = 0;
	while($row = mysql_fetch_array($result)){
				 $arr[$i++] = $row['ring_time'];				
	}
	return $arr;
}
function __insert_sub_tt($prof_id,$time)
{
	$query = "INSERT INTO rings (ring_time,) VALUES('".$time."')";
	mysql_query($query)	or die($query." - ".mysql_error());  
}
function __update_sub_tt($tname, $ind, $time)
{
	$query = "UPDATE ".$tname." SET ring_time='".$time."' WHERE ind='".$ind."'";
	mysql_query($query)	or die($query." - ".mysql_error());  
}
function _create_ring_tt($tt, $id)
{
	global $sub_tt_prefix;
	$table = $sub_tt_prefix.$id;
	$query ="CREATE TABLE ".$table."(ind INT NOT NULL AUTO_INCREMENT, PRIMARY KEY(ind), ring_time TIME)";
	mysql_query($query)	or die($query." - ".mysql_error());  
	
	foreach($tt as $entry) {
		__insert_sub_tt($table, $entry);
	}
	
}

function _get_id_from_name($name)
{
	global $main_tt_name;
	$query = "SELECT * FROM " . $main_tt_name . " WHERE name='" . $name ."'"; 
	
	$result = mysql_query($query) or die($query." - ".mysql_error()); 
	$row =  mysql_fetch_array($result);
	
	return $row['id'];
}
function create_tt_from_arr($tt)
{
	global $main_tt_name;
	$query = "INSERT INTO " . $main_tt_name . "(name, armed) 
			  VALUES('".$tt['name']."','".$tt['armed']."')";
	mysql_query($query)	or die(mysql_error());
	$id = _get_id_from_name($tt['name']);
	
	_create_ring_tt($tt['tt'], $id);
}
function get_one_tt_entry()
{
	global $main_tt_name;
	$query = "SELECT * FROM " . $main_tt_name; 
	$result = mysql_query($query) or die(mysql_error());
	$row =  mysql_fetch_array($result);
}
function get_tt_from_name($name)
{
	global $main_tt_name;
	$query = "SELECT * FROM " . $main_tt_name . " WHERE name='" . $name . "'"; 
	$result = mysql_query($query) or die(mysql_error() . $query );
	$row =  mysql_fetch_array($result);
	$row ['tt'] = _get_ring_tt($row['id']);
	return $row;
}
function change_tt_name($id, $name)
{
	global $main_tt_name;
	$query = "UPDATE ".$main_tt_name." SET name='".$name."' WHERE id='".$id."'";
	mysql_query($query) or die(mysql_error());
}
function tt_arm($id)
{
	global $main_tt_name;
	$query = "UPDATE ".$main_tt_name." SET armed='0'";
	mysql_query($query) or die(mysql_error());
	$query = "UPDATE ".$main_tt_name." SET armed='1' WHERE id='".$id."'";
	mysql_query($query) or die(mysql_error());
}

function change_ring_tt($id, $tt)
{
	global $sub_tt_prefix;
	$size = count($tt);
	for($ind = 0; $ind < $size; $ind++ ) {
		if($tt[$ind] != "") {
			echo "Inserting: " . $tt[$ind] . "<br>";
			__update_sub_tt($sub_tt_prefix.$id, $ind ,$tt[$ind]);
		}
	}
}
function echo_tt($tt)
{
	echo ' ';
}
function append_ring_tt($id)
{
	global $sub_tt_prefix;
	__insert_sub_tt($sub_tt_prefix.$id,'0:0:0');
}


function __get_max_ring_num($table)
{
	global $sub_tt_prefix;
	
	$query = "SELECT MAX(ind) FROM " . $table;	
	
	$result = mysql_query($query) or die(mysql_error(). '  ' . $query);
	$row = mysql_fetch_array($result);
	var_dump($row);
	return $row['MAX(ind)'];
}

function del_ring_tt_entry($id)
{
	global $sub_tt_prefix;
	$table = $sub_tt_prefix.$id;
	
	$max_id = __get_max_ring_num($table);
	$query = "DELETE FROM ".$table." WHERE ind='".$max_id."'";
	mysql_query($query) or die(mysql_error(). '  ' . $query);
	
}
function get_rules()
{
	$query = "SELECT * FROM rules";		
	$result = mysql_query($query) or err_box($query,'mysql');
	$row = mysql_fetch_array($result);
	return $row;
}
function get_overrrides()
{
	$query = "SELECT * FROM overrides";		
	$result = mysql_query($query) or err_box($query,'mysql');
	$row = mysql_fetch_array($result);
	return $row;
	
}

?>
