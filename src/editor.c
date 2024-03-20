#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "editor.h"

void printChar(int enablePrintCharFunc) {
    if (enablePrintCharFunc == 1) {
        if (read(STDIN_FILENO, &rawTermBuff, 1) == -1 && errno != EAGAIN) {
            die("read");
        }

        if (iscntrl(rawTermBuff)) {
            printf("%d\r\n", rawTermBuff);
        } else {
            printf("%d ('%c')\r\n", rawTermBuff, rawTermBuff);
        }
    }
}

void editorDrawRows() {
    int y;
    for (y = 0; y < E.screenrows; y++) {
        write(STDOUT_FILENO, "/\r\n", 3);
    }
}

void editorRefreshScreen() {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    editorDrawRows();
    write(STDOUT_FILENO, "\x1b[H", 3);
}

void editorProcessKeypress() {
    char rawTermBuff = editorReadKey();
    switch (rawTermBuff) {
        case CTRL_KEY('q'):
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            exit(0);
            break;
    }
}

int getCursorPosition(int* rows, int* cols) {
    char buf[32];
    unsigned int i = 0;

    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) {
        return -1;
    }

    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
        if (buf[i] == 'R') break;
        i++;
    }

    buf[i] = '\0';
    printf("\r\n&buf[1]: '%s'\r\n", &buf[1]);

    printf("\r\n");
    
    editorReadKey();

    return -1;
}

int getWindowSize(int* rows, int* cols) {
    struct winsize ws;

   if (1 || ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
    return getCursorPosition(rows, cols);
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

void initEditor() {
    if (getWindowSize(&E.screenrows, &E.screencols) == -1) {
        die("getWindowSize");
    }
}

int main(int argc, char* argv[]) {
    enableRawMode();
    initEditor();
    while(1) {
        editorRefreshScreen();
        editorProcessKeypress();
    }

    return 0;
}