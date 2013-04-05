<?php
	/*
	 * Time Table entry:
	 * 		name VCHAR(32)
	 * 		armed BOOL
	 * 		id INT NOT NULL AUTO_INCREMENT
	 * 
	 * 
	 * Ring Time Table 
	 *      num INT NOT NULL AUTO_INCREMENT
	 * 	    ring_time TIME
	 */
$sub_tt_prefix = "ring_time_table_";
$main_tt_name = "main_table";
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
	$link = mysql_connect("localhost","arseni",'1') or die ('Not connected: ' . mysql_error());	
	return mysql_select_db($db_name, $link) or die('Can\'t use tset: ' . mysql_error());
}
function _get_ring_tt($id)
{
	global $sub_tt_prefix;
	$query = "SELECT * FROM " . $sub_tt_prefix . $id; 
	$result = mysql_query($query) or die(mysql_error());
	while($row = mysql_fetch_array($result)){
				 $arr[$row['ind']] = $row['ring_time'];				
	}
	return $arr;
}
function __insert_sub_tt($tname, $time)
{
	$query = "INSERT INTO ".$tname. " (ring_time) VALUES('".$time."')";
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
?>
