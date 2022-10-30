#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "parsers/simple-fasta-parser.h"
#include "parsers/simple-fastq-parser.h"
#include <stdint-gcc.h>


// make order of alphabet

int * constructSARadix(char const * x)
{
    int n = strlen(x);
    int* sa = malloc(sizeof(int)*n);
    for (int i = 0; i<n; i++) {
        sa[i] = i;
    }


    return sa;
}

void printIntArray(int * a, int len) {
    for(int i=0; i<len; i++) {
        printf("%d", a[i]);
    }
    printf("\n");
}


int main(int argc, char const *argv[])
{
    printIntArray(constructSARadix("abc"), 3);
    /*
    if (argc != 3)
    {
        fprintf(stderr, "%s genome reads", argv[0]);
        return 1;
    }
    const char *genome_fname = argv[1];
    const char *reads_fname = argv[2];

    printf("Search in %s for reads from %s\n", genome_fname, reads_fname);

    return 0;
    */
}



