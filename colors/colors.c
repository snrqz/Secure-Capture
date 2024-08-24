#include <stdio.h>
#include "colors.h"

void display_red() {
    printf("%s", "\033[1;31m");
}


void display_green() {
    printf("%s", "\033[1;32m");
}


void display_yellow() {
    printf("%s", "\033[1;33m");
}


void display_blue() {
    printf("%s", "\033[1;34m");
}


void display_magenta() {
    printf("%s", "\033[1;35m");
}


void display_cyan() {
    printf("%s", "\033[1;36m");
}


void display_white() {
    printf("%s", "\033[1;37m");
}


void display_reset() {
    printf("%s", "\033[0m");
}
