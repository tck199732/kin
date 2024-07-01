#!/usr/bin/env python3
import subprocess
from pathlib import Path

CurrentDir = Path(__file__).parent.resolve()
SamuraiDir = CurrentDir / 'samurai'
HimeDir = CurrentDir / 'hime'
MergeDir = CurrentDir / 'merged'
LogDir = CurrentDir / 'logs'
ExePath = CurrentDir / 'merge.exe'

# commissioning
# start_id = 1036
# end_id = 1048

start_id = 1301
end_id = 1310

def main():
    # hime_files = sorted([f for f in HimeDir.rglob("**/*.root")])
    # samurai_files = sorted([f for f in SamuraiDir.rglob("**/*.root")])

    hime_files = sorted([HimeDir / f'data{idx:04d}.root' for idx in range(start_id,end_id+1)])
    samurai_files = sorted([SamuraiDir / f'data{idx:04d}.root' for idx in range(start_id,end_id+1)])

    for hime_file, samurai_file in zip(hime_files, samurai_files):
        if (not hime_file.exists()) or (not samurai_file.exists()):
            print(f'File not found: {hime_file} or {samurai_file}')
            continue
        
        print(f'Merging {hime_file} and {samurai_file}')
        cmd = [
            ExePath,
            '--hime-filename', hime_file,
            '--samurai-filename', samurai_file,
            '--output-filename', MergeDir / f'{samurai_file.stem}.root',
            '>&', LogDir / f'{samurai_file.stem}.log'
        ]
        cmd = ' '.join(list(map(str, cmd)))
        subprocess.run(cmd, shell=True)

if __name__ == '__main__':
    main()
