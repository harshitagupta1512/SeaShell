#include "def.h"

void pwd() {
    char currDir[max_size];
    getcwd(currDir, max_size);
    print_green();
    printf("%s\n", currDir);
    print_reset();
}