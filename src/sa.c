#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "parsers/simple-fasta-parser.h"
#include "parsers/simple-fastq-parser.h"
#include <stdint-gcc.h>
#include "helper.h"

//Question: When does it make sense to use array[n] vs. malloc?

int * constructSARadix(struct Fasta fasta)
{
    char* x = fasta.fasta_sequence;
    int n = fasta.fasta_len;
    int* sa = malloc(n * sizeof *sa);
    int* saCopy = malloc(n * sizeof *sa);
    int** currentSa = &sa;
    int** otherSa = &saCopy;
    int** temp;

    for (int i = 0; i<n; i++) {
        sa[i] = i;
    }
    int* bucketsIndecies = fasta.alphabet.sightings;
    int accumSum = 0;
    for(int i=0; i<fasta.alphabet.size; i++) { //TODO invariant will break when multiple fastas. Make into multiple functions
        int sighting = bucketsIndecies[i];
        bucketsIndecies[i] = accumSum;
        accumSum += sighting;
    }
    int* buckets = malloc(fasta.alphabet.size*sizeof *buckets);

    for(int i=n-1; i>=0; i--) {
        for(int j=0; j<fasta.alphabet.size; j++) buckets[j] = 0;
        for(int j=0; j<n; j++) {
            //TODO move initilization outside loop
            int charIndex = ((*currentSa)[j] + i) % n;
            char c = x[charIndex];
            int elemInBucket = buckets[c]++;
            int saIndex = bucketsIndecies[c] + elemInBucket;
            (*otherSa)[saIndex] = (*currentSa)[j];
        }
        temp = currentSa;
        currentSa = otherSa;
        otherSa = temp;
    }

    return sa;
}

int * searchPattenInSA(char* x, char* pattern, int* sa, int n, int m) {


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


    struct Fasta *start_of_fastas = fastas;
    while (fastas != NULL) {
        if(fastas->fasta_len == 0) {
            fastas += sizeof *fastas; //TODO change
            continue;
        }
        printIntArray(constructSARadix(*fastas), fastas->fasta_len); //TODO Change to real one. This is just debug
        fastas += sizeof *fastas; //TODO change
        break; //TODO REMOVE. Just for now
    }


    //printf("Search in %s for reads from %s\n", genome_fname, reads_fname);

    return 0;

}



