#!/bin/bash

taras_dir=/home/s063/exp/exp2406_s063/anaroot/users/taras/macros/chkveto/root_files
my_dir=/home/s063/exp/exp2406_s063/anaroot/users/kin/unpacked

# # for idx in $(seq 1036 1048); do
# for idx in $(seq 1147 1155); do
#     taras_file=$taras_dir/veto_kyoto_$idx.root
#     my_file=$my_dir/data$idx.root
#     root -l -b -q "CheckUnpackedFileEntries.C(\"$taras_file\", \"$my_file\")"
# done

ana_dir=/home/s063/exp/exp2406_s063/anaroot/users/kin/scratch/ana/analyzed

for idx in $(seq 1036 1048); do
    my_file=$my_dir/data$idx.root
    ana_file=$ana_dir/data$idx.root
    root -l -b -q "CheckUnpackedFileEntries.C(\"$my_file\", \"$ana_file\")"
done


