import os
import sys
import json
import numpy as np
import pandas as pd
import itertools
import matplotlib as mpl
import matplotlib.pyplot as plt
from pathlib import Path
from collections import OrderedDict
from scipy.optimize import curve_fit

MPL_CUSTOM = {
    'font.family': 'serif',
    'mathtext.fontset': 'cm',
    'figure.dpi': 120,
    'figure.figsize': (4, 3.5),
    'figure.facecolor': 'white',
    'xtick.top': True,
    'xtick.direction': 'in',
    'xtick.minor.visible': True,
    'ytick.right': True,
    'ytick.direction': 'in',
    'ytick.minor.visible': True,
}

for k, v in MPL_CUSTOM.items():
    mpl.rcParams[k] = v


def main():

    n_layers = 3
    n_modules = 72
    bar_width = 0.04
    bar_gap = 0.004
    n_modules_per_layer = n_modules // n_layers


    # first layer
    x = []
    xtop = n_modules_per_layer // 2 * bar_width + (n_modules_per_layer // 2 - 0.5) * bar_gap
    for i in range(n_modules_per_layer):
        x.append(
            xtop - i * (bar_width + bar_gap) - bar_width / 2
        )

    # second layer
    xright = xtop
    for i in range(n_modules_per_layer):
        x.append(
            xright - i * (bar_width + bar_gap) - bar_width / 2
        )

    # third layer
    for i in range(n_modules_per_layer):
        x.append(
            xtop - i * (bar_width + bar_gap) - bar_width / 2
        )

    
    df = pd.DataFrame({
        'id' : np.arange(n_modules),
        'layer_id' : np.concatenate([[i] * n_modules_per_layer for i in range(n_layers)]),
        'x' : np.array(x),
    })            

    # add a title in the first row, 
    df.to_csv('positions.dat', index=False, sep=' ', float_format='%.3f')

if __name__ == '__main__':
    main()
        





