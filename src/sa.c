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

    struct FastaContainer *fastaCont = parse_fasta(fasta_str);

    int ** SAs = constructMultipleSARadix(fastaCont);
    struct Fasta **fastas = fastaCont->fastas;


    while (reads_str[0] != '\0') {
        char *fastq_header = read_fastq_head(&reads_str);
        char *pattern = read_fastq_pattern(&reads_str);
        int pattern_len = (int) strlen(pattern);


        struct Fasta **start_of_fastas = fastas;
        int ** startOfSAs = SAs;

        while (*fastas != NULL) {
            if ((*fastas)->fasta_len == 0) {
                fastas++;
                SAs++;
                continue;
            }
            struct Interval interval = searchPatternInSA(**fastas, pattern, *SAs, pattern_len);
            for (int i = interval.start; i < interval.end; i++) {
                printf("%s\t%s\t%d\t%dM\t%s\n", fastq_header, (*fastas)->fasta_head, (*SAs)[i], pattern_len, pattern);
            }

            fastas++;
            SAs++;
        }
        fastas = start_of_fastas;
        SAs = startOfSAs;


        //printf("Search in %s for reads from %s\n", genome_fname, reads_fname);
    }
    return 0;

}



