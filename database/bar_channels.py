#!/usr/bin/env python3
import json 

# viewed from down stream
# beam right
# bar index    = 31, 30, 29, ..., 17, 16, 15, ..., 0
# bar channels = 0, 2, 4, ..., 28, 1, 3, 5, ..., 29
# beam left
# bar index    = 31, 30, 29, ..., 17, 16, 15, ..., 0
# bar channels = 32, 34, 36, ..., 62, 33, 35, 37, ..., 63

right_ch = [bar * 2 for bar in range(15)] + [bar * 2 + 1 for bar in range(15)]
left_ch = [bar * 2 + 32 for bar in range(15)] + [bar * 2 + 33 for bar in range(15)]

channels = {
    'right' : {bar : ch for bar, ch in enumerate(right_ch[::-1])},
    'left' : {bar : ch for bar, ch in enumerate(left_ch[::-1])}
}

for key, value in channels.items():
    print(key)
    for bar, channel in value.items():
        print(f'{bar:02d} : {channel:02d}')

with open('bar_channels.json', 'w') as f:
    json.dump(channels, f, indent=4)

with open('bar_channels.dat', 'w') as f:
    for bar, ch in channels['right'].items():
        f.write(f'{bar} {ch}\n')
    for bar, ch in channels['left'].items():
        f.write(f'{bar+30} {ch}\n')