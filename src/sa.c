#include "sa.h"

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
    char *start_of_reads_str = reads_str;
    char *start_of_fastas_str = fasta_str;

    struct FastaContainer *fastaCont = parse_fasta(fasta_str);
    int **SAs = constructMultipleSARadix(fastaCont);
    struct Fasta **fastas = fastaCont->fastas;

    while (reads_str[0] != '\0') {
        char *fastq_header = read_fastq_head(&reads_str);
        char *pattern = read_fastq_pattern(&reads_str);
        int pattern_len = (int) strlen(pattern);

        struct Fasta **start_of_fastas = fastas;
        int ** startOfSAs = SAs;

        for (int i = 0; i < fastaCont->numberOfFastas; ++i) {
            if ((*fastas)->fasta_len == 0) {
                fastas++;
                SAs++;
                continue;
            }
            struct Interval interval = searchPatternInSA(**fastas, pattern, *SAs, pattern_len);
            for (int j = interval.start; j < interval.end; j++) {
                printf("%s\t%s\t%d\t%dM\t%s\n", fastq_header, (*fastas)->fasta_head, (*SAs)[j]+1, pattern_len, pattern);
            }

            fastas++;
            SAs++;
        }

        fastas = start_of_fastas;
        SAs = startOfSAs;
    }

    // Free the strings
    free(start_of_reads_str);
    free(start_of_fastas_str);
    // Free everything else
    free(SAs);
    free_fasta_container(fastaCont);

    return 0;
}



