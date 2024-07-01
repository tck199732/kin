#!/bin/bash

INPUT_DIR="analyzed"
# OUTPUT_DIR="result/sbtCharge"
OUTPUT_DIR="result/sbtTime"
mkdir -p $OUTPUT_DIR
# for idx in $(seq 1036 1048); do
for idx in $(seq 1147 1159); do
    input=$INPUT_DIR/data$idx.root
    output=$OUTPUT_DIR/data$idx.txt
    echo "Processing data $idx"
    # root -l -b -q "macros/WriteSbtCharge.C(\"$input\", \"$output\")"
    root -l -b -q "macros/WriteSbtTime.C(\"$input\", \"$output\")"
done