//
//  ccsv.c
//  CCSV
//
//  Created by Mihai Costea on 30/01/2018.
//  Copyright © 2018 Mihai Costea. All rights reserved.
//

#include "ccsv/ccsv.h"
#include <stdlib.h>
#include <string.h>

#include "ccsv_config.h"

#ifndef HAVE_GETLINE
#include "ccsv/win_string.h"
#endif

struct CSVParser {
    FILE *csvFile;
    char *filePath;
    char *delimiter;
    bool hasHeader;
    char **header;
    size_t headerLen;
};


CSVParser *CSVParserCreate(const char *filePath, const char *delimiter, const bool hasHeader) {
    CSVParser *parser = (CSVParser *) malloc(sizeof(CSVParser));
    parser->filePath = (char *)filePath;
    parser->delimiter = (char *)delimiter;
    parser->hasHeader = hasHeader;
    parser->csvFile = NULL;
    parser->csvFile = fopen(filePath, "r");
    
    if (!parser->csvFile) {
        free(parser);
        return NULL;
    }
    
    return parser;
}

void CSVParserDestroy(CSVParser *parser) {
    if (parser->csvFile) {
        fclose(parser->csvFile);
        parser->csvFile = NULL;
    }
    
    if (parser->hasHeader && parser->header) {
        for (int i = 0; i < parser->headerLen; i++) {
            free(parser->header[i]);
        }
        
        free(parser->header);
    }
    
    free(parser);
}

void CSVParserParse(CSVParser *parser) {
    if (parser->hasHeader) {
        char **headerTokens = NULL;
        size_t headerLen = 0;
        CSVParserNextLine(parser, &headerTokens, &headerLen);
        
        if (headerTokens) {
            parser->header = headerTokens;
            parser->headerLen = headerLen;
        }
    }
}

void CSVParserGetHeader(CSVParser *parser, char ***headerTokens, size_t *headerLen) {
    if (!parser->hasHeader || !parser->header) {
        *headerTokens = NULL;
        *headerLen = -1;
        return;
    }
    
    char **headerCopy = (char **) malloc(parser->headerLen * sizeof(char *));
    for (int i = 0; i < parser->headerLen; i++) {
        headerCopy[i] = strdup(parser->header[i]);
    }
    
    *headerTokens = headerCopy;
    *headerLen = parser->headerLen;
}

ssize_t extendArray(void *arr, size_t newCapacity) {
	void *extendedArray = realloc(arr, newCapacity);

	if (!extendedArray) {
		// We were unable to allocate more memory so we exit gracefully
		return -1;
	}

	arr = extendedArray;
}

void CSVParserNextLine(CSVParser *parser, char ***lineTokens, size_t *lineLen) {
    if (!parser->csvFile) {
        *lineTokens = NULL;
        *lineLen = -1;
        return;
    }
    
    char *line = NULL;
    size_t lineCap = 0;
    ssize_t charNum = 0;
    
	charNum = getline(&line, &lineCap, parser->csvFile);

    if (charNum != -1) {
        char *toFree = line;
        char *token;
        char **tokens = (char **) malloc(10 * sizeof(char *));
        size_t tokensCap = 10;
        size_t tokensLen = 0;

        while ((token = strsep(&line, parser->delimiter)) != NULL) {
            if (tokensLen == tokensCap) {
                size_t newTokensCap = tokensCap + 10;
				if (extendArray(tokens, newTokensCap) == -1) {
					*lineTokens = NULL;
					*lineLen = -1;
					return;
				}
                tokensCap = newTokensCap;
            }
            
            if (token[strlen(token) - 1] == '\n') {
                // Remove newline character from last token
                token[strlen(token) - 1] = '\0';
                
                // Check if \r\n DOS line ending
                if (token[strlen(token) - 1] == '\r') {
                    token[strlen(token) - 1] = '\0';
                }
            }
            tokens[tokensLen++] = strdup(token);
        }
        
        free(toFree);
        
        *lineTokens = tokens;
        *lineLen = tokensLen;
    } else {
        free(line);
        
        *lineTokens = NULL;
        *lineLen = -1;
    }
}
