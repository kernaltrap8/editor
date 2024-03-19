#include <stdio.h>
#include <unistd.h>
#include "editor.h"

int main(int argc, char* argv[]) {
    enableRawMode();
    while (read(STDIN_FILENO, &rawTermBuff, 1) == 1 && (rawTermBuff != quitKey)); {
        printChar();
    }
    return 0;
}