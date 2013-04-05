<?php

include_once 'error.php';
include_once 'site.php';
include_once 'pages/show_tt.php';

include_once 'mysql_op.php';
include_once 'daemon_op.php';



$shown_tt = NULL;
$edit_flag = false;


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
	} while ( _get_id_from_name($name) != NULL );
	return $name;
}


function show_tt($name)
{
		global $shown_tt;
		$shown_tt = get_tt_from_name($name);
}

function edit_tt($name)
{
	global $shown_tt, $edit_flag;
	$edit_flag = true;
	$shown_tt = get_tt_from_name($name);		
}
function clone_tt($name)
{
	global $shown_tt;
	$shown_tt = get_tt_from_name($_GET['clone']);
	$rearm = is_armed($shown_tt);
		
	$shown_tt['name'] = uniq_name($shown_tt['name'],'_clone');	
	
	create_tt_from_arr($shown_tt);
	if($rearm) {						
		tt_arm($shown_tt['id']);
	}
	$shown_tt = get_tt_from_name($shown_tt['name']);						
}
function arm_tt($name)
{
	global $shown_tt;
	$shown_tt = get_tt_from_name($_GET['arm']);	
	if(is_armed($shown_tt)){
		err_box("Can't arm already armed time table", "user");
	}
	tt_arm($shown_tt['id']);
	$shown_tt = get_tt_from_name($_GET['arm']);	
	daemon_arm($shown_tt);
}
function add_tt($name)
{
	global $shown_tt, $edit_flag;
	$shown_tt = get_tt_from_name($_GET['add']);
		
	$edit_flag = true;
	append_ring_tt($shown_tt['id']);
	
	$shown_tt = get_tt_from_name($_GET['add']);
}
function drop_tt($name)
{
	global $shown_tt;
	$shown_tt = get_tt_from_name($_GET['drop']);
	if( is_armed($shown_tt)) {
		err_box('Can\'t drop armed time table.');
	}
	drop_table($shown_tt);
	$shown_tt = NULL;
}
function del_tt($name)
{
	global $shown_tt;
	$shown_tt = get_tt_from_name($name);
	$edit_flag = true;
	del_ring_tt_entry($shown_tt['id']);
		
	$shown_tt = get_tt_from_name($name);
}
function submit_tt($name)
{
	global $shown_tt, $edit_flag;
	$shown_tt = get_tt_from_name($name);
	$size = count($_GET);			
	$need = false;
	for( $i = 0; $i < $size; $i ++ ) {			
		$shown_tt['tt'][$i+1] = $_GET[$i];	
		$need = true;
	}
	if ($need) {
		change_ring_tt($shown_tt['id'],$shown_tt['tt']);
	}
	$shown_tt = get_tt_from_name($name);
	if ($_GET['new_name'] !=  "") {
		change_tt_name($shown_tt['id'],$_GET['new_name']);
		$shown_tt = get_tt_from_name($_GET['new_name']);
	}
}
function process_buttons()
{
	global $shown_tt, $edit_flag;
	
	$edit_flag = false;
	$shown_tt = NULL;
	
	if(isset($_GET['show'])) {
		show_tt($_GET['show']);
	}
	if(isset($_GET['edit'])) {
		edit_tt($_GET['edit']);
	}
	if(isset($_GET['clone'])) {
		clone_tt($_GET['clone']);
	}
	if(isset($_GET['arm'])) {
		arm_tt($_GET['arm']);		
	}
	if(isset($_GET['add'])) {
		add_tt($_GET['add']);
	}
	if(isset($_GET['drop'])) {
		drop_tt($_GET['drop']);
	}
	if(isset($_GET['del'])) {
		del_tt($_GET['del']);
	}
	if(isset($_GET['submit'])) {
		submit_tt($_GET['submit']);
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



print_head('style2.css');
//err_box();
load_lib('mysql');
connect_to_mysql("test_1");
err_box("USER FAULT", "user");
process_buttons();
echo '<body dir="ltr">';
main_box_open();

if( $shown_tt != NULL )	{
	sub_box_open('class="bg"');
	
		print_shown_tt();
		
	sub_box_close();

	sub_box_open('class="buttons" align="center"');
	
		print_buttons();
		
	sub_box_close();
}
	sub_box_open('class="bg"');
	
		print_tt_list();
		
	sub_box_close();

main_box_close();

print_banner();
echo '</body>';
//$query = "SELECT * FROM main_table WHERE id=MAX('id')";
//$result = mysql_query($query);	
			
//while($row = mysql_fetch_array($result)){
	//echo $row['name']. " - ". $row['armed'] ." ". $row['id'];
	//echo "<br />";
//}
		/*
			

			//$result = mysql_query("UPDATE example SET age='22' WHERE name='Samdra Mellowman'") 
			//or die(mysql_error());  
			echo "Running... ";
			//$n = "mysql";
			//$f = NULL;
			//echo "You " . (extension_loaded($n)?"":"don\'t") . "need to load".(((PHP_SHLIB_SUFFIX === 'dll') ? 'php_' : '') . ($f ? $f : $n) . '.' . PHP_SHLIB_SUFFIX);
			load_lib('mysql');
			echo "Running... ";
			connect_to_mysql("test");
			
			$query = "SELECT * FROM main_table"; 
	 
			$result = mysql_query($query);
	
			
			while($row = mysql_fetch_array($result)){
				echo $row['name']. " - ". $row['armed'] ." ". $row['id'];
				echo "<br />";
			}
			// store the record of the "example" table into $row
			//$row = mysql_fetch_array( $result );
			// Print out the contents of the entry 

			//echo "Name: ".$row['name'];
			//echo " Age: ".$row['age'];
								

	echo "Table Created!\n"
	*/
?>


