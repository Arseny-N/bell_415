<?php
include_once "site.php";
include_once "error.php";
include_once "dc.php";
###################################
##### !!!! setenforce 0 !!!! ######
###################################



function process_buttons()
{
	global $prog;
	
	if(isset($_REQUEST['kill'])) {
		system($prog.' down');
	}
	
	if(isset($_REQUEST['rexec'])) {
		d_rexec();
	}
	
	if(isset($_REQUEST['start'])) {
		system($prog.' up '. escapeshellcmd($_REQUEST['start_cmd']));
	}
	
	if(isset($_REQUEST['ring'])) {
		system($prog.' ring '. escapeshellcmd($_REQUEST['ring_duration']));
	}
	if(isset($_REQUEST['ring1'])) {
		system( $prog.' ring1');
	}
	if(isset($_REQUEST['ring2'])) {
		system($prog.' ring2');
	}
}
function print_daemon_table()
{
	global $state, $error_logs,$dbg_logs, $argv;
	//echo '<div id="d_box">';
	table_open(NULL,[], 'id="d_box"');
	echo '<form action="'.$argv[0].'" method="post">';
	echo '<tr><td>Deamon state</td><td>' . $state . '</td></tr>';
#	echo '<tr><td>Error log file </td><td>' . $error_logs . '</td></tr>';
#	echo '<tr><td>Normal log file </td><td>' . $dbg_logs . '</td></tr>';
		
	echo '<tr><td>Reboot the daemon</td><td><button type="submit" name="rexec" value="-">RE EXEC</button></td></tr>';
	
	echo '<tr><td>Kill the daemon</td><td><button type="submit" name="kill" value="-">KILL</button></td></tr>';
	echo '<tr><td>Start the daemon</td>
		<td><button type="submit" name="start" value="-">RUN</button>  
		<input type="text" name="start_cmd" placeholder="Costum cmd line args" /> </td></tr>';
		
	echo '<tr><td>Ring The bell</td> 
		<td><button type="submit" name="ring" value="-">RING</button>
		<input type="text" name="ring_duration" placeholder="Costum ring duration" /> </td>
		</tr>';
	echo '</form>';
 	//echo '</div>';
	table_close();
	
	
}
class log_praser
{
	private $handle;
	public $bad_lines = [];
	
		
	public function __construct($file)
	{
		$this->handle = $file;
	}
		
	private function format_log_line($line)
	{
		preg_match("/^\[(\d+-\d+-\d+) (\d+\:\d+\:\d+)\] \[(\d+)\] (\S+)\: \[(\S+)\]\: (.*)$/", $line, $matches);
		return $matches;
	}
	public function format_line($line)
	{
		$logs = $this->format_log_line($line);
	
		if (isset($logs[0])) {	
	
			$formated_log = array();
			$formated_log['date'] = $logs[1];		
			$formated_log['time'] = $logs[2];
			$formated_log['pid'] = $logs[3];
			$formated_log['prio'] = $logs[4];	
			$formated_log['func'] = $logs[5];	
			$formated_log['mesg'] = $logs[6];
			
			return $formated_log;
		} else {
			return false;
		}	
	}
	public function get_log() 
	{
		global $broken;
	       	$formatted = [];
	       	
		while (($line = fgets($this->handle)) !== false) {	
        		$log = $this->format_line($line);

	        	if(!$log) { 
        			$broken[] = $line; 
        			continue; 
        		}
        		$formatted[] = $log;
		}		
		return $formatted;
	}
	public function get_sorted_log($ccol)
	{
		//$l = $this->get_log();
		//return $this->sort_by($l, $ccol);
		return $this->get_log();
	}
}


function date_cmp($a,$b) 
{
	//$result =  $a['date'] > $b['date'] ? -1 : $a['date'] < $b['date'] ? 1 :
	  //     $a['time'] > $b['time'] ? -1 : $a['time'] < $b['time'] ? 1 : 0;
	 //echo " $a[date] $b[date]  $a[time] $b[time] $result ";
	 //return $result;
	 return 0;
}	
	
function sort_by_date($arr)
{
	usort($arr, 'date_cmp' );
	return $arr;
}
function print_line($log)
{

      	echo '<tr>';
      	echo '<td>'.$log['prio'].'</td><td>'.$log['date'].'</td>
        <td>'.$log['pid'].'</td><td>'.$log['time']. '</td><td>'
        .$log['func'].'</td><td>'.$log['mesg'].'</td>';
        echo '</tr>';	
}
function print_log($entry)
{
       	foreach($entry as $line )
       		print_line($line);
}


$state=shell_exec("$prog state");
$error_logs=shell_exec("$prog err-log-path");
$dbg_logs=shell_exec("$prog dbg-log-path");	

$broken= array();

process_buttons();

print_head('style2.css');

echo '<body dir="ltr">';
echo '<table class="main_box"><tr><td class="navi_box_menu">';	


print_navi_menu();

echo '</td><td class="main_box" width="100%">';	

sub_box_open('class="bg"');
	print_daemon_table();
sub_box_close();


sub_box_open('class="bg"');
	table_open('Errror Log',['Type', 'Date','Pid','Time','Function','Message'], 'id="log"');
		$handle = popen('cat /var/log/bell/err', "r");
		if(!$handle) 
			echo 'No file (err-logs)';
		$err = new log_praser($handle);	
		$log = $err -> get_log();
		//$log = sort_by_date($log, 'time');
		$log = array_reverse($log, true);
		print_log($log);
		
		pclose($handle);
	table_close();
sub_box_close();

sub_box_open('class="bg"');
	table_open('Normal Log',['Type', 'Date','Pid','Time','Function','Message'], 'id="log"');
		$handle = popen('cat /var/log/bell/dbg', "r");
		if(!$handle) 
			echo 'No file (dbg-logs)';	
		$dbg = new log_praser($handle);	
		$log = $dbg -> get_log();
		//$log = sort_by_date($log, 'time');
		$log = array_reverse($log, true);
		
		print_log($log);
		pclose($handle);
	table_close();
sub_box_close();

echo '</td></tr></table>';
echo '</body>';	
?>

