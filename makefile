#sic compiler for CS-UY 3943 by Nikita Georgiou and Jack Martinez
BFLAGS = -vd 
CPPFLAGS = -std=c++1z -Wno-deprecated -O2
all: sic

sic: sic.tab.o lex.yy.o symbol_table.o semantic_check.o
	@echo ========================================
	@echo \ \ \ \ \ \ \ \ \ \ \ \ \ \  \ \ MAKING
	@echo ========================================
	g++ -o sic ${CPPFLAGS} sic.tab.o lex.yy.o symbol_table.o semantic_check.o

sic.tab.o: sic.y 
	bison ${BFLAGS} sic.y
	g++ $(CPPFLAGS) -c sic.tab.c 

lex.yy.o: sic.l
	flex sic.l
	g++ $(CPPFLAGS) -c lex.yy.c

symbol_table.o: symbol_table.cpp
	g++ ${CPPFLAGS} -c symbol_table.cpp

semantic_check.o: semantic_check.cpp
	g++ ${CPPFLAGS} -c semantic_check.cpp

clean: 
	rm -f sic.output sic.tab.* lex.yy.c sic *.o
