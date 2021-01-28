run: json.exe Sol4.html
	cat Sol4.html | ./json.exe

json.exe: lex.yy.c aux.o
	gcc -o json.exe lex.yy.c aux.o -ll

aux.o: aux.c aux.h
	gcc -c aux.c aux.h

lex.yy.c: Sol2NetLang.l
	flex Sol2NetLang.l

Sol4.html:
	wget https://natura.di.uminho.pt/~jj/pl-20/TP1/Sol4.html
clean:
	rm *.o
	rm *.html
	rm json.exe
	rm sol_comments.json
	rm *.gch




