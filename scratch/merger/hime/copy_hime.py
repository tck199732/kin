#!/usr/bin/env python3

import re
import subprocess
from pathlib import Path

CurrentDir = Path(__file__).parent.resolve()
HimeDesDir = CurrentDir / 'taras-marco'
HimeDesDir.mkdir(exist_ok=True)

# do it manually to add subfolders 
# HimeSrcDir = Path('/home/s063/HIME/himeAna/data/tDiff') / '2024-06-17_shadow'
# HimeSrcDir = Path('/home/s063/HIME/himeAna/data/tDiff') / '2024-06-17_sbt_shadow'
# HimeSrcDir = Path('/home/s063/HIME/himeAna/data/tDiff' / '2024-06-28'
HimeSrcDir = Path('/home/s063/HIME/himeAna/data/tDiff') / '2024-06-30'

def main():
    hime_files = sorted([f for f in HimeSrcDir.rglob("**/*.root")])
    # print(hime_files)
    for hime_file in hime_files:
        try:
            runID = int(re.findall(r'\d+', hime_file.name)[0])
        except:
            print('Error in parsing runID')
            continue
        print(hime_file.name, runID)
        des = HimeDesDir / f'data{runID:04d}.root'
        cmd = f'cp {str(hime_file)} {str(des)}'
        subprocess.run(cmd, shell=True)

if __name__ == '__main__':
    main()
