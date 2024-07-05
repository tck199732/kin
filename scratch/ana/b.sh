#!/bin/bash

INPUT_DIR="unpacked"
OUTPUT_DIR="analyzed"

for input in $INPUT_DIR/*.root; do
    run=$(basename $input .root | sed 's/data//')
    output=$OUTPUT_DIR/data$run.root
    echo "Processing data $run"
    ./ana.exe -i $input -o $output
done
