#!/bin/bash

echo type height,insert_time,search_time,delete_time

NUM="1"
ZZZ="0"
for i in $(seq 1 6); do
    NUM="${NUM}${ZZZ}"
    for j in $(seq 1 10); do
        ./benchmark "$NUM" >> data.txt
    done
done
