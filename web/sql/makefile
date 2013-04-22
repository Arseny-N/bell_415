DB="test_3"

.PHONY: acid random_acid clean

mysql: gen_files
	./mk-mysql.sh $(DB)

acid: gen_files
	./acid_table.sh 120 '' rings.sql 
	./mk-mysql.sh $(DB)

random_acid: gen_files
	./acid_table.sh 120 '' rings.sql yes
	./mk-mysql.sh $(DB)
gen_files:	
	@a=.*.sql; for i in $$a; do cp $$i $$(echo $$i | sed 's/.//') ;done
clean:
	rm  $$(ls *.sql)
