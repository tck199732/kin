#!/usr/bin/env python3
import json
import pathlib
import subprocess

current_dir = pathlib.Path(__file__).parent.absolute()
run_list_pth = current_dir / 'database' / 'runId.json'

unpacked_dir = current_dir / 'unpacked'
output_dir = current_dir / 'analyzed'

with open(run_list_pth, 'r') as f:
    run_list = json.load(f)['runId']


for run in run_list:
    infile = unpacked_dir / f'data{run}.root'
    outfile = output_dir / f'data{run}.root'
    print(f"Processing data {run}")
    subprocess.run(f"./ana.exe -i {str(infile)} -o {str(outfile)} --hime_veto_calib ./database/veto/calib.dat", shell=True)


