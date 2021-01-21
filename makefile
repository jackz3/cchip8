files = main.c chip8.c debugger.c renderer.c

mac:
	gcc ${files} -lSDL2 -o a.out
.PHONY : win
win: 
	gcc ${files} -lmingw32 -lSDL2main -lSDL2 -o a.exe

clean :
	rm edit