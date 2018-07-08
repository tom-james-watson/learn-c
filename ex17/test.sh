#!/bin/bash
set -x #echo on

rm test.db

./ex17 test.db c
./ex17 test.db l
./ex17 test.db s 1 tom tom@tomjwatson.com
./ex17 test.db l
./ex17 test.db s 2 bob bob@bob.com
./ex17 test.db l
./ex17 test.db g 1
./ex17 test.db d 1
./ex17 test.db l
