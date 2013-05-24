<?php

function stdf_error_echo($err)
{
	global $php_errormsg;
	echo $err .' :<br>';
	echo $php_errormsg.'<br>';
}
function user_error_echo($err)
{
	
	echo 'Consult the help page:<br>';	
	echo $err .'<br>';
}
function mysql_error_echo($err)
{
	echo 'MySQL error:<br>';
	echo mysql_error() .'<br>';
	echo 'Query: '.$err;
}
function err_box($err = NULL,$source = NULL)
{
	if(!$err) {
		$err = 'Unspecified error, might be a bug.';
	}	
	sub_box_open('class="err_box"',NULL,'width="45px" ');
	echo 
	'
	<table><tr>'.
		//<td><img src="img/red-cross.png"></td>
		'<td>
	';
	switch(strtolower($source)) {
		case 'mysql':
			mysql_error_echo($err);
			break;
		case 'user':
				
			user_error_echo($err);
			break;
		case 'stdf':
		case 'system':
			stdf_error_echo($err);
			break;
		default:
			echo '<h3>'. $err .'</h3>';
			break;
	}
	
	echo 
	'
		</td>
	</tr></table>
	';
	sub_box_close();
	
	exit;
		
}
?>
