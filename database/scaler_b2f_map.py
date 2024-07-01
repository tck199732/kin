#!/usr/bin/env python3
import re
import json
import requests

# mapping, as updated on 2024-06-16
mapping = {    
    0: 'gated trig',
    1: 'ungated trig',
    2: 'AC up',
    3: 'AC down',
    4: 'AC left',
    5: 'AC right',
    6: 'AC OR',
    7: 'KatanaV left',
    8: 'KatanaV middle',
    9: 'KatanaV right',
    10: 'KatanaV spare',
    11: 'KatanaV OR',
    12: 'SBT',
    13: 'SBT and no AC OR',
    14: 'SBT and no Katana OR',
    15: 'SBT and no Busy',
    16: 'Kyoto mult',
    17: '1k clk',
    18: '',
    19: '',
    20: '',
    21: '',
    22: '',
    23: '',
    24: '',
    25: '',
    26: '',
    27: '',
    28: '',
    29: '',
    30: '',
    31: '',
}

with open('scaler_b2f.json', 'w') as f:
    json.dump(mapping, f, indent=4)

def main():
    r = requests.get("http://ribfdaq.rarfadv.riken.go.jp/~samurai/babiscr.php", auth=('samurai', 'samurai'))
    if r.status_code == 200:
        print("Successfully connected to the server.")
    else:
        print("Failed to connect to the server.")
        return
    
    # HARD-CODED to get scaler monitor for samurai
    data = r.text.split('Scaler Data ID=41 (41)')[1]
    # data = data.split('Scaler Data ID=107 ()')[0]
    data = data.split('Scaler Data ID=98 ()')[0]

    data = data.split('\n')[2:-6]
    
    for d in data:
        print(d)
    
    input()
    

    map_from_server = {}
    for l in data:
        try:
            index, monitor = re.findall(r'<td align=right>(\d+)<td>\(([^)]*)\)*', l)[0]
        except:
            print(f"Error: {l}")
            continue

        index = int(index)
        map_from_server[index] = monitor
    
    # check if the mapping is correct
    correct = True
    for k, v in map_from_server.items():
        # assert mapping[k] == v, f"Error: {k} -> {v} != {mapping[k]}"
        if mapping[k] != v:
            print(f"mapping has changed: {k} -> {v} != {mapping[k]}")
            correct = False
    
    if correct:
        print("Mapping is correct, no need to update.")
    else:
        print("Mapping has changed. Press 'y' to update the mapping.")
        if input() == 'y':
            with open('scaler_b2f.json', 'w') as f:
                json.dump(map_from_server, f, indent=4)
        else:
            print("Mapping has not been updated.")
    

if __name__ == '__main__':
    main()