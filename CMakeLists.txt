cmake_minimum_required (VERSION 3.21)
project (sa)
enable_testing()

add_executable(sa src/sa.c)
add_executable(testing1 test/tests.c)
add_executable(testing2 test/testSearch.c)
add_executable(time test/time.c)
add_test(tester testing1 "./")
add_test(tester2 testing2 "./")

