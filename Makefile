CFLAGS = -std=c++2a -O2



VMTranslator: *.cpp *.hpp
	g++	$(CFLAGS)	-o	VMTranslator	*.cpp
	chmod u+x+r+w VMTranslator


