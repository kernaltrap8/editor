#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include "editor.h"

void printChar() {
    if (iscntrl(rawTermBuff)) {
        printf("%d\n", rawTermBuff);
    } else {
        printf("%d ('%c')\n", rawTermBuff, rawTermBuff);
    }
}

int main(int argc, char* argv[]) {
    enableRawMode();
    while (read(STDIN_FILENO, &rawTermBuff, 1) == 1 && (rawTermBuff != quitKey)); {
        printChar();
    }
    return 0;
}