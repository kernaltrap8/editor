#!/bin/bash

SRC_FILES="src/editor.c src/terminal.c"

meson init -l c $SRC_FILES --builddir build/
