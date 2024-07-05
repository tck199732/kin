#!/bin/bash

INPUT_DIR="unpacked"
OUTPUT_DIR="analyzed"
run1=$1
run2=$2

for idx in $(seq $run1 $run2); do
    input=$INPUT_DIR/data$idx.root

    if [ ! -f $input ]; then
        echo "File $input does not exist"
        continue
    fi
    output=$OUTPUT_DIR/data$idx.root
    echo "Processing data $idx"
    ./ana.exe -i $input -o $output
done
