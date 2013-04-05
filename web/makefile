PATH=/var/www/html

.PHONY: install

install: error.php daemon_op.php mysql_op.php site.php test.php
	/bin/cp error.php daemon_op.php mysql_op.php site.php test.php $(PATH)
	/bin/cp pages/* $(PATH)/pages
	/bin/cp img/*   $(PATH)/img
