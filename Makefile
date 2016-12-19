basicNumeric:
	g++ basicNumeric.cpp -std=c++11 -lreadline -o basicNumeric
install:
	cp -p basicNumeric /usr/bin/
	cp -p consts.txt ~/.bnrc
clean:
	rm basicNumeric
uninstall:
	rm /usr/bin/basicNumeric
	rm ~/.bnrc
