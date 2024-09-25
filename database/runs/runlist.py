#!/usr/bin/env python
import json

commission = [idx for idx in range(1036, 1048 + 1)]

june_sn112 = [idx for idx in range(1047, 1369 + 1)]

runlist = {
    'commission': commission,
    'june_sn112': june_sn112
}

with open('runlist.json', 'w') as f:
    json.dump(runlist, f, indent=4)
    



