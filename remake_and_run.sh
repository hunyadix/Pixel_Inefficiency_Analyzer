#!/usr/bin/env tcsh

clear
make clean
make
./multiplicity_analysis
root -l stats.root
new TBrowser
