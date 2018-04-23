#sic compiler for CS-UY 3943 by Nikita Georgiou and Jack Martinez
BFLAGS = -vd 
CPPFLAGS = -std=c++1z -Wno-deprecated -g 
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
