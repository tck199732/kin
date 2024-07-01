#!/bin/bash

# Check if run_number argument is provided
if [ -z "$1" ]; then
	echo "## Error: Missing run_number argument."
	exit 1
fi

run_number=$1
echo "## Run number: $run_number"
echo ""
action=$2

if [ "$action" == "new" ]; then
	echo '======================================================='
	if [ -f "unpacked/data$run_number.root" ]; then
		echo "## Removing unpacked/data$run_number.root ..."
		rm unpacked/data$run_number.root
	fi
	if [ -f "scratch/ana/analyzed/data$run_number.root" ]; then
		echo "## Removing scratch/ana/analyzed/data$run_number.root ..."
		rm scratch/ana/analyzed/data$run_number.root
	fi
	echo ""
fi

#echo '======================================================='
#if [ ! -f spirit_reco/run"$run_number"_s00.reco.2024.root ]; then
#	echo "##" File spirit_reco/run"$run_number"_s00.reco.2024.root does not exist. copying data from spana01 ...
#	scp spirit@spana01:/cache/scr/spirit/curtis/spiritroot/SpiRITROOT/macros/data/run"$run_number"_s00.reco.2024.root spirit_reco/
#fi
#ls spirit_reco/run"$run_number"_s00.reco.2024.root
#echo ""

echo '======================================================='
if [ ! -f "unpacked/data$run_number.root" ]; then
	echo "## File unpacked/data$run_number.root does not exist. Running unpack.py ..."
	./unpack.py --start $run_number --end $run_number
fi
ls unpacked/data$run_number.root
echo ""

echo '======================================================='
if [ ! -f "scratch/ana/analyzed/data$run_number.root" ]; then
	echo "## File scratch/ana/analyzed/data$run_number.root does not exist. Running a.sh ..."
	cd scratch/ana/
	echo "## Running a.sh script in scratch/ana/ ..."
	./a.sh $run_number $run_number
	cd -
fi
ls scratch/ana/analyzed/data$run_number.root
echo ""

echo '======================================================='
root -l scratch/ana/draw_kyoto_multiplicity.C\($run_number\)
