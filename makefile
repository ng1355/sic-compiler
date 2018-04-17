BFLAGS = -vd 
CPPFLAGS = -std=c++11 -Wno-deprecated
all: bison flex sic

sic: bison flex
	@echo ========================================
	@echo				  MAKING
	@echo ========================================
	g++ -o sic ${CPPFLAGS} sic.tab.c lex.yy.c symbol_table.cpp

bison: sic.y 
	bison ${BFLAGS} sic.y

flex: sic.l
	flex sic.l

clean: 
	rm -f sic.output sic.tab.* lex.yy.c sic *.o
