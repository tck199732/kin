import os
import re
import json
import itertools
import numpy as np
import pandas as pd
from pathlib import Path
from functools import cached_property

project_dir = Path(os.environ['CONDA_PREFIX']).parent.absolute()
data_path = project_dir / 'scratch/ana/result/kyotoRate' / 'data1045.csv'

class RateConverter:
    def __init__(self, data_path):
        self.data_path = data_path
        self.df = pd.read_csv(str(f), sep=r'\s+', header=None, names=['Mch', 'count', 'P(Mch>=x)'])
        # self.df.query('Mch >= 1', inplace=True)
        self.df['Mch'] = self.df['Mch'] - 0.5
        print(self.df)

    @cached_property    
    def multiplicities(self):
        return self.df['Mch'].values

    @cached_property
    def counts(self):
        return self.df['count'].values
    
    @cached_property
    def probs(self):
        return self.df['P(Mch>=x)'].values
    
    @cached_property
    def rates(self):
        rates = np.zeros((len(self.multiplicities), len(self.multiplicities)))
        for mch, mch_0 in itertools.product(self.multiplicities, self.multiplicities):
            rates[mch, mch_0] = self.__get_rate(mch, mch_0)
        return rates

    def __get_rate(self, mch, mch_0):
        ref = self.df.query('x == @mch_0')['y'].values[0]
        return self.df.query('x == @mch')['y'].values[0] / (ref + 1e-12)


if __name__ == '__main__':
    rc = RateConverter(data_path)
    # print(rc.rates)