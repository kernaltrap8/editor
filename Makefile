editor: editor.o rawmode.o 
	$(CC) build/editor.o build/rawmode.o -o build/editor
editor.o:
	mkdir -p build
	$(CC) -c src/editor.c -o build/editor.o
rawmode.o:
	$(CC) -c src/rawmode.c -o build/rawmode.o