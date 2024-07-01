#!/usr/bin/env python3
import re
from pathlib import Path
import subprocess

def read_events(filename):
    with open(filename, 'r') as f:
        try:
            line = f.readlines()[-1]
            return [int(s) for s in re.findall(r'(\d+)', line)]
        except:
            return 0, 0, 0

def main():
    files = sorted([f for f in Path('.').glob('./*.log')])
    events = [read_events(f) for f in files]

    # for f, (hime, samurai, merged) in zip(files, events):
    #     print(f'{f.stem}: hime: {hime:7d} \tsamurai: {samurai:7} \tmerged: {merged:7}')

    for f, (hime, samurai, merged) in zip(files, events):
        if merged == samurai - 1 and samurai == hime:
            print(f'{f.stem} fully merged.')
        else:
            print(f'{f.stem}: Hime: {hime}, Samurai: {samurai}, Merged: {merged}')

if __name__ == '__main__':
    main()
        
