#!/bin/bash

# NO NEED TO RUN THIS SCRIPT IF YOU ARE ON RIKEN MACHINE WITH ANAROOTLOGIN COMMAND
# FOR LOCAL USE ONLY

while [ ! -z $CONDA_PREFIX ]; do
    conda deactivate
done

source /home/kin/software/root/bin/thisroot.sh
export TARTSYS=/home/kin/Desktop/spirit2024-scripts/src/anaroot/install
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TARTSYS/lib

#For MINOS
export LD_LIBRARY_PATH=$HOME/exp/minos/install/pentium-linux-release/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$HOME/exp/minos/liboffline:$LD_LIBRARY_PATH
export PKG_CONFIG_PATH=$HOME/exp/minos/install/pentium-linux-release/lib/pkgconfig
export XMLDIPOLESH=$HOME/epics/xmldipole.sh