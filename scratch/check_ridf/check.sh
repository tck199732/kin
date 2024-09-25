#!/bin/bash

unpacked_dir="/home/s063/exp/exp2406_s063/anaroot/users/kin/unpacked"
for f in $unpacked_dir/*.root; do
    root -l -b -n -q "checkRidfEntries.C(\"$f\")"
done