#include "minunit.h"
#include "../src/parsers/simple-fasta-parser.h"
#include "../src/sa.h"
#include "testHelper.h"


void test_setup(void) {
    /* Nothing */
}

void test_teardown(void) {
    /* Nothing */
}

//int (*test_fun)(char*, char*, int, int);

void assertSameListInt(int * list, int * answers, int len) {
    for(int i=0; i<len; i++) mu_assert_int_eq(list[i], answers[i]);
}

MU_TEST(test_search_mis) {
    char *fasta_str = read_file("../test-data/mis.fa");
    char *reads_str = read_file("../test-data/mis.fastq");
}


MU_TEST(test_search_abc) {
    char * seq = malloc(sizeof(* seq)*6);
    char * sequence = "ABCABC";
    strcpy(seq, sequence);
    char * pattern = malloc(sizeof(* pattern)*3);
    char * pat = "ABC";
    strcpy(pattern, pat);
    int * sa = malloc(sizeof(* sa)*7);
    sa[0] = 6;
    sa[1] = 3;
    sa[2] = 0;
    sa[3] = 4;
    sa[4] = 1;
    sa[5] = 5;
    sa[6] = 2;
    struct Fasta * fasta = malloc(sizeof *fasta);
    update_fasta_by_sequence(&seq, fasta);

    struct Interval inter = searchPattenInSA(*fasta, pattern, sa, 3);
    mu_assert_int_eq(3, inter.end);
    mu_assert_int_eq(1, inter.start);
}


void run_all_fasta_parser_tests() {
    MU_RUN_TEST(test_search_mis);
    MU_RUN_TEST(test_search_abc);
}

MU_TEST_SUITE(fasta_parser_test_suite) {
    run_all_fasta_parser_tests();
}

int main(int argc, char *argv[]) {
    MU_RUN_SUITE(fasta_parser_test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}