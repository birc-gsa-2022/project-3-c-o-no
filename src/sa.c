#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "parsers/simple-fasta-parser.h"
#include "parsers/simple-fastq-parser.h"
#include <stdint-gcc.h>
#include "helper.h"


int * constructSARadix(struct Fasta fasta)
{
    char * x = fasta.fasta_sequence;
    int n = strlen(x);
    int* sa = malloc(n * sizeof *sa);
    for (int i = 0; i<n; i++) {
        sa[i] = i;
    }


    return sa;
}

char *read_file(const char *file_name) {
    FILE *fp;
    fp = fopen(file_name, "rb+");

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);  /* same as rewind(f); */

    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, fp);
    fclose(fp);

    string[fsize] = '\0'; // terminate with zero
    return string;
}

int main(int argc, char const *argv[])
{

    if (argc != 3)
    {
        fprintf(stderr, "%s genome reads", argv[0]);
        return 1;
    }
    const char *genome_fname = argv[1];
    const char *reads_fname = argv[2];

    char *fasta_str = read_file(genome_fname);
    char *reads_str = read_file(reads_fname);

    struct Fasta *fastas = *parse_fasta(fasta_str);

    // As we parse the reads, we pattern match the read against our above-parsed FASTA sequences
    while (reads_str[0] != '\0') {
        char *fastq_header = read_fastq_head(&reads_str);
        char *pattern = read_fastq_pattern(&reads_str);
        int pattern_len = (int) strlen(pattern);

        struct Fasta *start_of_fastas = fastas;
        while (fastas != NULL) {
            printIntArray(constructSARadix(*fastas), 3); //TODO Change to real one. This is just debug
            fastas++;
        }
        fastas = start_of_fastas;
    }


    //printf("Search in %s for reads from %s\n", genome_fname, reads_fname);

    return 0;

}



