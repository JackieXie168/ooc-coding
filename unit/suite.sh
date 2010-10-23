#!/bin/bash
# Running test cases as a test suite with memory leak check


valgrind --leak-check=yes --quiet ./ooctest

valgrind --leak-check=yes --quiet ./exceptiontest

# This test should not be run in Valgrind because it uses unix signals
./testcasetest

valgrind --leak-check=yes --quiet ./refcountedtest

valgrind --leak-check=yes --quiet ./vectortest

valgrind --leak-check=yes --quiet ./listtest
