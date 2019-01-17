/*
 * strdup() is not part of POSIX and is causing crashes on my CentOS systems.
 *
 * This is a simple implementation of strdup() but we can't call it that!
 *
 * For more information see http://stackoverflow.com/questions/8359966/strdup-returning-address-out-of-bounds
 */

#include "robstr.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define STRING_ARRAY_BLOCK_LENGTH       1048576         // 1 MByte - must be greater than the length of any sting we are duplicating (+1)

char *dupstr(const char *s) {
    static int stringArrayLength = 0;
    static int numberOfAllocatedBlocks = 0;
    static int newStringIndex = 0;
    static char *pStringArray;
    char * result;

	if (s == NULL) {
		fprintf(stderr, "Cannot duplicate a NULL string\n");
		return NULL;
	}

    int stringLength = strlen(s);

if (stringLength > STRING_ARRAY_BLOCK_LENGTH) {printf("string too long\n");}

    if (stringArrayLength == 0) {
        pStringArray = malloc (STRING_ARRAY_BLOCK_LENGTH);
        if (pStringArray == NULL) {
            fprintf(stderr, "Cannot allocate memory to duplicate %s\n", s);
            return NULL;
        }
        stringArrayLength = STRING_ARRAY_BLOCK_LENGTH;
        numberOfAllocatedBlocks = 1;
    }
    else if ((newStringIndex + stringLength + 1) > stringArrayLength) {
static int reallocCount = 0;
printf("Realloc %d\n", reallocCount++);
        numberOfAllocatedBlocks++;
        stringArrayLength = numberOfAllocatedBlocks*STRING_ARRAY_BLOCK_LENGTH;
        pStringArray = realloc (pStringArray, stringArrayLength);
        if (pStringArray == NULL) {
            fprintf(stderr, "Cannot re-allocate memory to duplicate %s\n", s);
            return NULL;
        }
printf("ReallocDone %d\n", reallocCount-1);
    }

    result = pStringArray+newStringIndex;
    newStringIndex += stringLength + 1;

	strcpy(result, s);
	return result;
}

char *catstr(const char *s, const char *t) {
	char *const result = malloc(strlen(s) + strlen(t) + 1);
	if (result == NULL) {
		fprintf(stderr, "Cannot allocate memory for catstr of %s and %s\n", s, t);
		exit(1);
	}
	int i;
	for (i=0; i<strlen(s); i++)
		result[i] = s[i];
	for (int j=0; j<strlen(t); j++)
		result[i++] = t[j];
	result[i]='\0';
	return result;
}
