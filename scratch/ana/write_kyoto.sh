#!/bin/bash

#for idx in $(seq 1148 1153); do 
for idx in $(seq 1263 1265); do 
    root -l -b -q "macros/WriteKyotoHits.C(\"analyzed/data$idx.root\", \"result/kyotoHits/data$idx.txt\")"
done
