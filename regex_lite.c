#include <stdio.h>
#include <stdlib.h>
#include "match.h"

void search_file(const char *filename, const char *pattern);
// how to: go to folder and type in terminal ./regex_lite test.txt "h[ae]llo"
int main(int arguments_count, char *arguments[]) {
    // expects program name, file name, and pattern
    if(arguments_count != 3){
        printf("%s <file> <pattern>\n", arguments[0]);
        return 1;
    }

    const char *filename = arguments[1];
    const char *pattern = arguments[2];

    search_file(filename, pattern);

    return 0;
}