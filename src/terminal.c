#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include "editor.h"

void die(const char * s) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    perror(s);
    exit(1);
}

// Struct with original term atribs
struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, & E.orig_termios) == -1) {
        die("tcsetattr");
    }
}

void enableRawMode() {
    // make sure terminal atributes are restored on exit
    if (tcgetattr(STDIN_FILENO, & E.orig_termios) == -1) {
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

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, & raw) == -1) {
        die("tcsetattr");
    }
}

char editorReadKey() {
    int nread;
    char rawTermBuff;
    while ((nread = read(STDIN_FILENO, &rawTermBuff, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) die("read");
    }
    if (rawTermBuff == '\x1b') {
        char seq[3];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';
        if (seq[0] == '[') {
        if (seq[1] >= '0' && seq[1] <= '9') {
            if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
            if (seq[2] == '/') {
            switch (seq[1]) {
                case '1': return HOME_KEY;
                case '3': return DEL_KEY;
                case '4': return END_KEY;
                case '5': return PAGE_UP;
                case '6': return PAGE_DOWN;
                case '7': return HOME_KEY;
                case '8': return END_KEY;
            }
            }
        } else {
            switch (seq[1]) {
            case 'A': return ARROW_UP;
            case 'B': return ARROW_DOWN;
            case 'C': return ARROW_RIGHT;
            case 'D': return ARROW_LEFT;
            case 'H': return HOME_KEY;
            case 'F': return END_KEY;
            }
        }
        } else if (seq[0] == 'O') {
        switch (seq[1]) {
            case 'H': return HOME_KEY;
            case 'F': return END_KEY;
        }
        }
        return '\x1b';
    } else {
        return rawTermBuff;
    }
}