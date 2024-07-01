#!/bin/bash

if [ ! -z $ANADIR ]; then
    export ANADIR_OLD=$ANADIR
fi

export ANADIR=$(pwd)

# assume root is already sourced
# source ~/software/root/bin/thisroot.sh