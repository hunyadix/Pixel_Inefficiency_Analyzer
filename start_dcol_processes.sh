#!/bin/bash

MAIN="Resources/Sources/Programs/main.cxx"

sed -i "113s;input_file_paths_file_name  =.*;input_file_paths_file_name  = \"Data/Dcolscans/dcol100_paths.txt\"\;;" $MAIN
sed -i "114s;firts_loop_output_file_name =.*;firts_loop_output_file_name = \"Results/Dcolscans/dcol100.root\"\;;" $MAIN
make clean
make
mv Pixel_Inefficiency_Analyzer Dcolscan_processes/dcol100_process
nohup ./Dcolscan_processes/dcol100_process > ./Dcolscan_processes/dcol100_results.log 2>&1 & 
DCOL100_PROCESS_PID=$(echo $!)

sed -i "113s;input_file_paths_file_name  =.*;input_file_paths_file_name  = \"Data/Dcolscans/dcol80_paths.txt\"\;;" $MAIN
sed -i "114s;firts_loop_output_file_name =.*;firts_loop_output_file_name = \"Results/Dcolscans/dcol80.root\"\;;" $MAIN
make clean
make
mv Pixel_Inefficiency_Analyzer Dcolscan_processes/dcol80_process
nohup ./Dcolscan_processes/dcol80_process > ./Dcolscan_processes/dcol80_results.log 2>&1 & 
DCOL80_PROCESS_PID=$(echo $!)

sed -i "113s;input_file_paths_file_name  =.*;input_file_paths_file_name  = \"Data/Dcolscans/dcol90_paths.txt\"\;;" $MAIN
sed -i "114s;firts_loop_output_file_name =.*;firts_loop_output_file_name = \"Results/Dcolscans/dcol90.root\"\;;" $MAIN
make clean
make
mv Pixel_Inefficiency_Analyzer Dcolscan_processes/dcol90_process
nohup ./Dcolscan_processes/dcol90_process > ./Dcolscan_processes/dcol90_results.log 2>&1 & 
DCOL90_PROCESS_PID=$(echo $!)

sed -i "113s;input_file_paths_file_name  =.*;input_file_paths_file_name  = \"Data/Dcolscans/dcol95_paths.txt\"\;;" $MAIN
sed -i "114s;firts_loop_output_file_name =.*;firts_loop_output_file_name = \"Results/Dcolscans/dcol95.root\"\;;" $MAIN
make clean
make
mv Pixel_Inefficiency_Analyzer Dcolscan_processes/dcol95_process
nohup ./Dcolscan_processes/dcol95_process > ./Dcolscan_processes/dcol95_results.log 2>&1 & 
DCOL95_PROCESS_PID=$(echo $!)

sed -i "113s;input_file_paths_file_name  =.*;input_file_paths_file_name  = \"Data/Dcolscans/dcol97_paths.txt\"\;;" $MAIN
sed -i "114s;firts_loop_output_file_name =.*;firts_loop_output_file_name = \"Results/Dcolscans/dcol97.root\"\;;" $MAIN
make clean
make
mv Pixel_Inefficiency_Analyzer Dcolscan_processes/dcol97_process
nohup ./Dcolscan_processes/dcol97_process > ./Dcolscan_processes/dcol97_results.log 2>&1 & 
DCOL97_PROCESS_PID=$(echo $!)

sed -i "113s;input_file_paths_file_name  =.*;input_file_paths_file_name  = \"Data/Dcolscans/dcol98_paths.txt\"\;;" $MAIN
sed -i "114s;firts_loop_output_file_name =.*;firts_loop_output_file_name = \"Results/Dcolscans/dcol98.root\"\;;" $MAIN
make clean
make
mv Pixel_Inefficiency_Analyzer Dcolscan_processes/dcol98_process
nohup ./Dcolscan_processes/dcol98_process > ./Dcolscan_processes/dcol98_results.log 2>&1 & 
DCOL98_PROCESS_PID=$(echo $!)

sed -i "113s;input_file_paths_file_name  =.*;input_file_paths_file_name  = \"Data/Dcolscans/dcol99_paths.txt\"\;;" $MAIN
sed -i "114s;firts_loop_output_file_name =.*;firts_loop_output_file_name = \"Results/Dcolscans/dcol99.root\"\;;" $MAIN
make clean
make
mv Pixel_Inefficiency_Analyzer Dcolscan_processes/dcol99_process
nohup ./Dcolscan_processes/dcol99_process > ./Dcolscan_processes/dcol99_results.log 2>&1 & 
DCOL99_PROCESS_PID=$(echo $!)

sed -i "113s;input_file_paths_file_name  =.*;input_file_paths_file_name  = \"Data/Dcolscans/dcol99p3_paths.txt\"\;;" $MAIN
sed -i "114s;firts_loop_output_file_name =.*;firts_loop_output_file_name = \"Results/Dcolscans/dcol99p3.root\"\;;" $MAIN
make clean
make
mv Pixel_Inefficiency_Analyzer Dcolscan_processes/dcol99p3_process
nohup ./Dcolscan_processes/dcol99p3_process > ./Dcolscan_processes/dcol99p3_results.log 2>&1 & 
DCOL99P3_PROCESS_PID=$(echo $!)

sed -i "113s;input_file_paths_file_name  =.*;input_file_paths_file_name  = \"Data/Dcolscans/dcol99p6_paths.txt\"\;;" $MAIN
sed -i "114s;firts_loop_output_file_name =.*;firts_loop_output_file_name = \"Results/Dcolscans/dcol99p6.root\"\;;" $MAIN
make clean
make
mv Pixel_Inefficiency_Analyzer Dcolscan_processes/dcol99p6_process
nohup ./Dcolscan_processes/dcol99p6_process > ./Dcolscan_processes/dcol99p6_results.log 2>&1 & 
DCOL99P6_PROCESS_PID=$(echo $!)

sed -i "113s;input_file_paths_file_name  =.*;input_file_paths_file_name  = \"Data/Dcolscans/dcol99p9_paths.txt\"\;;" $MAIN
sed -i "114s;firts_loop_output_file_name =.*;firts_loop_output_file_name = \"Results/Dcolscans/dcol99p9.root\"\;;" $MAIN
make clean
make
mv Pixel_Inefficiency_Analyzer Dcolscan_processes/dcol99p9_process
nohup ./Dcolscan_processes/dcol99p9_process > ./Dcolscan_processes/dcol99p9_results.log 2>&1 & 
DCOL99P9_PROCESS_PID=$(echo $!)

sleep 10

kill -STOP $DCOL100_PROCESS_PID $DCOL80_PROCESS_PID $DCOL90_PROCESS_PID $DCOL95_PROCESS_PID $DCOL97_PROCESS_PID $DCOL98_PROCESS_PID $DCOL99_PROCESS_PID $DCOL99P3_PROCESS_PID $DCOL99P6_PROCESS_PID $DCOL99P9_PROCESS_PID

kill -CONT $DCOL100_PROCESS_PID $DCOL80_PROCESS_PID
sleep 3h

kill -CONT $DCOL90_PROCESS_PID $DCOL95_PROCESS_PID
sleep 3h

kill -CONT $DCOL97_PROCESS_PID $DCOL98_PROCESS_PID
sleep 3h

kill -CONT $DCOL99_PROCESS_PID $DCOL99P3_PROCESS_PID
sleep 3h

kill -CONT $DCOL99P6_PROCESS_PID $DCOL99P9_PROCESS_PID
sleep 3h