<?php
include_once 'site.php';

function print_help()
{
table_open("Помошь",[])

?>
<tr> <td>
Здравствуйте, вы читаете мануал веб-интерфейса программы осуществляющей активацию школьных звонков.
Веб интерфейс используется для её настройки.

Вверху главной страници распологается таблица 


<center><img src="img/help.png"/></center>
<?php
table_close();
}


print_head('style2.css');

echo '<body dir="ltr">';
echo '<table class="main_box"><tr><td class="navi_box_menu">';	

print_navi_menu();

echo '</td><td class="main_box" width="100%">';	

sub_box_open('class="bg"');
	print_help();
sub_box_close();

?>
