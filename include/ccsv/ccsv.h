//
//  ccsv.h
//  CCSV
//
//  Created by Mihai Costea on 30/01/2018.
//  Copyright © 2018 Mihai Costea. All rights reserved.
//

#ifndef ccsv_h
#define ccsv_h

#include <stdio.h>
#include <stdbool.h>

typedef struct CSVParser CSVParser;

CSVParser *CSVParserCreate(const char *filePath, const char *delimiter, const bool hasHeader);
void CSVParserDestroy(CSVParser *parser);

void CSVParserParse(CSVParser *parser);
void CSVParserGetHeader(CSVParser *parser, char ***headerTokens, size_t *headerLen);
void CSVParserNextLine(CSVParser *parser, char ***lineTokens, size_t *lineLen);

#endif /* ccsv_h */
