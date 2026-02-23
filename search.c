#include <stdio.h>
#include <string.h>
#include "match.h"

void search_file(const char *filename, const char *pattern) {
    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        printf("ERROR: could not open %s\n", filename);
        return;
    }
    FILE *out = fopen("output.txt", "w"); // create output file

    char line[1024];
    int line_num = 1;
    
    // reads file line by line
    while(fgets(line, sizeof(line), fp) != NULL){
        // removes \n if any 
        line[strcspn(line, "\n")] = '\0';
        // if pattern matches the line -> print with line num
        if(match(pattern, line)){
            printf("%d: %s\n", line_num, line);
            fprintf(out, "%d: %s\n", line_num, line);
        }
        line_num++;
    }
    fclose(fp);
    fclose(out);

}