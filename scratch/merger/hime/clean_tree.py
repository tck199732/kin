#!/usr/bin/env python3

import subprocess
from pathlib import Path

current_dir = Path(__file__).resolve().parent.absolute()
data_dir = current_dir / 'taras-marco'
out_dir = current_dir

root_files = sorted([f for f in data_dir.glob('*.root')])
# if '1148' in f.name
for fname in root_files:
    outname = out_dir / fname.relative_to(data_dir)
    print(f'Processing {fname}')
    cmd = f'root -l -b -q \'cleanTree.C(\"{str(fname)}\", \"{str(outname)}\")\''
    subprocess.run(cmd, shell=True)

