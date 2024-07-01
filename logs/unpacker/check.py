#!/usr/bin/env python3

from pathlib import Path

words_to_search = [
    'Error!! Channel for EASIROC should be less than 64!!', # from my unpacker
    'Frame Error'  # generated from RIBFROOT
]

def main():
    files_to_check = sorted([f for f in (Path(__file__).parent).rglob('*.log')])
    validities = {
        f.stem : is_valid(f, words_to_search) for f in files_to_check
    }

    for f, v in validities.items():
        if not v:
            print(f'{f}: error detected')

def keyword_present(file, keyword) -> bool :
    with open(file, 'r') as f:
        for line in f.readlines():
            if keyword in line:
                return False
    return True

def is_valid(file, keywords):
    detected = [keyword_present(file, keyword) for keyword in keywords]
    return all(detected)

if __name__ == '__main__':
    main()