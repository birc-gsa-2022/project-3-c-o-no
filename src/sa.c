#include "sa.h"

//Question: When does it make sense to use array[n] vs. malloc?


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

    char *fastq_header = read_fastq_head(&reads_str);
    char *pattern = read_fastq_pattern(&reads_str);
    int pattern_len = (int) strlen(pattern);


    struct Fasta *start_of_fastas = fastas;
    while (fastas != NULL) {
        if(fastas->fasta_len == 0) {
            fastas += sizeof *fastas; //TODO change
            continue;
        }
        int * sa = constructSARadix(*fastas);
        struct Interval interval = searchPattenInSA((*fastas).fasta_sequence, pattern, sa, (*fastas).fasta_len, pattern_len);
        printf("start: %d\nend: %d", interval.start, interval.end);

        fastas += sizeof *fastas; //TODO change
        break; //TODO REMOVE. Just for now
    }


    //printf("Search in %s for reads from %s\n", genome_fname, reads_fname);

    return 0;

}



