#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <termios.h>
#include <string.h>
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

void editorDrawRows(struct abuf* ab) {
    int y;
    for (y = 0; y < E.screenrows; y++) {
        if (y == E.screenrows / 3) {
            char version[80];
            int versionlen = snprintf(version, sizeof(version),
                "editor // version %s", EDITOR_VERSION);
            if (versionlen > E.screencols) {
                versionlen = E.screencols;
            }

            int padding = (E.screencols - versionlen) / 2;
            if (padding) {
                abAppend(ab, "/", 1);
                padding--;
            }
            while(padding--) {
                abAppend(ab, " ", 1);
            }

            abAppend(ab, version, versionlen);
        } else {
            abAppend(ab, "/", 1);
        }

        abAppend(ab, "\x1b[K", 3);
        if (y < E.screenrows - 1) {
            abAppend(ab, "\r\n", 2);
        }
    }
}

void editorRefreshScreen() {
    struct abuf ab = ABUF_INIT;

    abAppend(&ab, "\x1b[?25l", 6);
    abAppend(&ab, "\x1b[H", 3);

    editorDrawRows(&ab);

    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy, + 1, E.cx + 1);
    abAppend(&ab, buf, strlen(buf));

    abAppend(&ab, "\x1b[H", 3);
    abAppend(&ab, "\x1b[?25h", 6);

    write(STDOUT_FILENO, ab.b, ab.len);
    abFree(&ab);
}

void editorMoveCursor(int key) {
    switch (key) {
        case ARROW_LEFT:
            E.cx--;
            break;
        case ARROW_RIGHT:
            E.cx++;
            break;
        case ARROW_UP:
            E.cy--;
            break;
        case ARROW_DOWN:
            E.cy++;
            break;
    }
}

void editorProcessKeypress() {
    int rawTermBuff = editorReadKey();
    switch (rawTermBuff) {
        case CTRL_KEY('q'):
            write(STDOUT_FILENO, "\x1b[2J", 4);
            write(STDOUT_FILENO, "\x1b[H", 3);
            exit(0);
            break;
        case 'w':
        case 's':
        case 'a':
        case 'd':
            editorMoveCursor(rawTermBuff);
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
   
    if (buf[0] != '\x1b' || buf[1] != '[') {
        return -1;
    }
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) {
        return -1;
    }

    return 0;
}

int getWindowSize(int* rows, int* cols) {
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
                return -1;
            }
        return getCursorPosition(rows, cols);
        } else {
            *cols = ws.ws_col;
            *rows = ws.ws_row;
            return 0;
        }
}

void initEditor() {
    E.cx = 0;
    E.cy = 0;

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