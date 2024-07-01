#!/bin/bash

SAVEDIR=logs/modules_in_use
if [ ! -d ${SAVEDIR} ]; then
    echo "Directory ${SAVEDIR} does not exist. Creating it."
    mkdir -p ${SAVEDIR}
else
    echo "Directory ${SAVEDIR} exists."
fi

for i in $(seq 1176 1178); do 
    idx=$(printf "%04d" $i)
    root -b -q -l "macros/CheckDataModule.C(
        \"ridf/data${idx}.ridf\", \
        \"${SAVEDIR}/data${idx}.dat\"
    )"
done

