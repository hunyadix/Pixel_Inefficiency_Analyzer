#!/bin/sh

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
FILE_TO_RUN="./Pixel_Inefficiency_Analyzer"

clear

if [ -e "$FILE_TO_RUN" ]
then
	echo "Removing earlier executables..."
	pwd
	rm "$FILE_TO_RUN"
fi

git fetch --all && git reset --hard origin/master
make clean
make |& less

# if [ -e $FILE_TO_RUN ]
# then
# 	echo "Build succesful, preparing to run..."
# 	exec $FILE_TO_RUN 
# fi