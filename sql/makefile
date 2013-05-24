DB?=""


.PHONY: acid random_acid clean files clean_mysql

mysql: files	
	./mk-mysql.sh $(DB)

acid: files
	./acid_table.sh 120 default 3 > acid.sql
	./mk-mysql.sh $(DB) acid.sql

randoAmcid: files
	./acid_table.sh random random 1 > randomAcid.sql
	./mk-mysql.sh $(DB) randomAcid.sql
files:	
	@a=.*.sql; for i in $$a; do cp $$i $$(echo $$i | sed 's/.//') ;done
clean:
	rm  $$(ls *.sql)

	

clean_mysql:
	@echo 'Not implemented'	
