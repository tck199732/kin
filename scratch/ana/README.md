# SamuraiDataAnalysis

Generate ROOT file containing Physcical data from unpacked data (Riken Daq). 

- Input : unpacked root file containing raw information such as values in tdc, qdc, trigger, time stamps, etc... 
- Structures of unpacked root file should be fixed by now (2024.06.23).
- Detector : so far only Kyoto, Sbt and HimeVeto are analyzed here. Write new classes to implement BDC. 

# usage
--------
- Just `make` to compile
- create symbolic link to unpacked data `ln -s ${path_to_data} unpacked`
- if ROOT is not sourced, modified `config.sh` and source it every time you start working.
- run `./ana.exe -i $input -o $output` to get analyzed root file. For multi-run, see `a.sh`.

