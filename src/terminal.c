#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include "editor.h"

void die(const char* s) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    perror(s);
    exit(1);
}

// Struct with original term atribs
//struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1) {
        die("tcsetattr");
    }
}

void enableRawMode() {
    // make sure terminal atributes are restored on exit
    if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) {
        die("tcsetattr");
    }

    atexit(disableRawMode);

    struct termios raw = E.orig_termios;
    raw.c_lflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_lflag &= ~(OPOST);
    raw.c_lflag |= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        die("tcsetattr");
        }
    }

char editorReadKey() {
    int nread;
    char rawTermBuff;
    while((nread = read(STDIN_FILENO, &rawTermBuff, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) {
            die("read");
        }
    }
    return rawTermBuff;
}