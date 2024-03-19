#include <stdio.h>
#include <ctype.h>
#include "editor.h"

void printChar() {
    if (iscntrl(rawTermBuff)) {
        printf("%d\n", rawTermBuff);
    } else {
        printf("%d ('%c')\n", rawTermBuff, rawTermBuff);
    }
}