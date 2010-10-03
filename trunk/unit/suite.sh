#!/bin/bash
# Running test cases as a test suite with memory leak check

# This test should not be run in Valgrind because it uses signals
./testcasetest

valgrind --leak-check=yes --quiet ./exceptiontest

valgrind --leak-check=yes --quiet ./vectortest
