//
//  main.c
//  CCSV
//
//  Created by Mihai Costea on 30/01/2018.
//  Copyright © 2018 Mihai Costea. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "ccsv/ccsv.h"

int main(int argc, const char * argv[]) {
    CSVParser *parser = CSVParserCreate("csv_test_file.csv", ",", true);
    
    CSVParserParse(parser);
    char **header = NULL;
    size_t headerLen = 0;
    CSVParserGetHeader(parser, &header, &headerLen);
    for (int i = 0; i < headerLen; i++) {
        printf("%s,", header[i]);
        free(header[i]);
    }
    printf("\n");
    free(header);
    
    char **line = NULL;
    size_t lineLen = 0;
    
    CSVParserNextLine(parser, &line, &lineLen);
    while (lineLen != -1) {
        for (int i = 0; i < lineLen; i++) {
            printf("%s,", line[i]);
            free(line[i]);
        }
        printf("\n");
        free(line);
        
        CSVParserNextLine(parser, &line, &lineLen);
    }
    
    CSVParserDestroy(parser);
    
    return 0;
}
