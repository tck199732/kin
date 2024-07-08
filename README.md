# Scripts for Spirit 2024 experiment at RIKEN

This repository stores useful scripts and ROOT macros for unpacking and analyzing data. 

## directories
- [macros](./) : stores ROOT macros for unpacking raw data in `ridf` format and perform simple analysis. `Anaroot` source files are not uploaded here. 

- [database](./database/) : stores useful data files for the experiment.

- [logs](./logs/) : stores log files generated when unpacking `ridf` files.

- [scratch](./scratch/) : further analysis happens here
    - [ana](./scratch/ana) : converts unpacked data to ROOT files with observables such as `tof, tdiff, multi, ...`
    - [merger](./scratch/merger/) : merging analyzed data from [ana](./scratch/ana) and data from [`tDiff`](https://github.com/MarcoKnoesel/himeAna) from Hime detector. 
    - [rateCheck](./scratch/rateCheck/) : a simple calculation for downscaler factor based on kyoto multiplicities in run `1045`.
    - [workdir](./scratch/workdir/) : working directory for `Hime` analysis. 
    - [neutronAna](./scratch/neutronAna/) : similar to [workdir](./scratch/workdir/) but for my own study.

## Instruction

### 0. Start
---------
Copy the required files or clone the repository `git clone https://github.com/tck199732/spirit2024-scripts.git`. Assume you are running the scripts on `ridfana04`, run `anarootlogin ${your_account_name}` to source everything you need. 

### 1. Unpacker
-----------
The first step is to unpack the `ridf` files using the macro located at [`macros/UnpackSamuraiData.C`](./macros/UnpackSamuraiData.C). To unpack multiple files, use the script [`unpacker.py`](./unpack.py). 

``` {bash}
# run `./unpack.py --help` to see options
./unpack.py --start 1036 --end 1048
```

By default, log files are produced in `./log/unpacked/data${idx}.log`. If there is any issue, there will be error message such as `RIDF:FrameError`. In such case, the data should not be analyzed. In commissioning run, there is error from `babieeasiroc`

```
RIBFROOT-Info : [/home/s063/svn/2024SAMURAI.s063/sources/Core/src/TArtDecoderVmeEasyroc.cc] Unknown data 0
```

Such error occurs in only a few events and those events are skipped in the unpacker. To check error in all log files, see [./check.py](./logs/unpacker/check.py) 

### 2. Ana
---------
Nagvigate to [`scratch/ana`](./scratch/ana), compile the program with `make`. This creates the executable `ana.exe`. To analyze multiple files generated in `step 1`, run 

``` {bash}
./a.sh ${start-run-idx} ${end-run-idx}
```

This produces ROOT files with observables such as `tdiff, tof, multi, detectorId, scaler ...` stored in [`analyzed`](./scratch/ana/analyzed/).

### 3. Merging 
-----------------
Now, we need to merge the data from samurai daq and hime daq. To do this, first we copy files from the Hime side. This is done in [`scratch/merger/hime`](./scratch/merger/hime/). If you are on shift, you can run `link_himefiles.py` to create sym link to the hime data files. 

Next, compile the merger program in the merger directory, i.e. `make`. Run the `a.py` script to merge files according to time stamp. Modify the `runId` in the script and simply `./a.py`. 

Users are suggested to check the merging by running the executable on single run number with verbose mode `./merge.exe -i ${hime-file} -s ${samurai-file} --verbose`. In run `1150`, for example, the merging is not always complete, below shows part of the output 

```
...
...
...

skipping event.
diff > window : 18446744041425467180 at hime 35294 , samurai 23480 , skipping event.
diff > window : 18446744041424106714 at hime 35295 , samurai 23480 , skipping event.
diff > window : 18446744041419285633 at hime 35296 , samurai 23480 , skipping event.
entriesHime = 35297     entriesSamurai = 33876  mergedEvents = 23480
```

### 4. analysis
- further analysis can be done on the merged files, see [`neutronAna`](./scratch/neutronAna/) and [`workdir/macros`](./scratch/workdir/macros/).

- There are a few python/jupyter files, I generated a conda env using the [`./environement.yml](./environment.yml), i.e. `conda env create -f ./environement.yml --prefix ./env`. However, do not add ROOT in environment since for some reason it conflicts root in the machine. 

## TLDR
For super fast instruction, see [`./scratch/merger/howtomerge.txt`](./scratch/merger/howtomerge.txt)

