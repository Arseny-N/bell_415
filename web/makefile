

DEST_DIR ?="/var/www"

php_objs := *.php *.css
img_objs := *.png
.PHONY: install
install:
	[ ! -d $(DEST_DIR)/img ] && mkdir -v $(DEST_DIR)/img || :
	cp -v $(php_objs) $(DEST_DIR)/	
	cp -v img/$(img_objs) $(DEST_DIR)/img/
