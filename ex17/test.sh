#!/bin/bash
set -x #echo on

rm test.db

./ex17 test.db c
./ex17 test.db l
./ex17 test.db s 0 tom tom@tomjwatson.com
./ex17 test.db l
./ex17 test.db s 1 bob bob@bob.com
./ex17 test.db l
./ex17 test.db g 0
./ex17 test.db f eric
./ex17 test.db f tom
./ex17 test.db d 0
./ex17 test.db l
