editor: editor.o terminal.o 
	$(CC) build/editor.o build/terminal.o -o build/editor
editor.o:
	mkdir -p build
	$(CC) -c src/editor.c -o build/editor.o
terminal.o:
	$(CC) -c src/terminal.c -o build/terminal.o
