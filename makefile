BFLAGS = -vd 
all: bison flex outfile

bison: sic.y 
	bison ${BFLAGS} sic.y

flex: sic.l
	flex sic.l

outfile: sic.tab.c
	g++ -o sic sic.tab.c lex.yy.c

clean: 
	rm -f sic.output sic.tab.* lex.yy.c sic
