basicNumeric:
	g++ basicNumeric.cpp -std=c++11 -lreadline -o basicNumeric
install:
	cp basicNumeric /usr/bin/
	cp consts1.py ~/.basicNumericrc
clean:
	rm basicNumeric
uninstall:
	rm /usr/bin/basicNumeric
	rm ~/.basicNumericrc
