#!/bin/bash

MINSIZE=100
MINAGE=1800
MAXSIZE=3072
MAXAGE=36000
SSTEP=256
TSTEP=1800

function usage() {
	cat << EOF
Usage: $0 <OPTIONS>
Script for running HTTP cache simulation 
Options:
-f log file name
-s minimal tested cache size (MB, defaults to 100)
-t minimal tested entry age (seconds, defaults to 1800)
-S maximal tested cache size (MB, defaults to 3072)
-T maximal tested entry age (seconds, defaults to 36000)
-d size step (defaults to 256)
-D time step (defaults to 1800)
-h Prints this help message
Example: $0 -f example.log -s 1024 -S 19000 -t 60 -T 24000 -d 1 -D 15 
EOF
}

while getopts "hf:s:t:S:T:d:D:" OPTION; do
	case $OPTION in
	h)
		usage
		exit 1
		;;
	f)
		FILENAME="$OPTARG"
		;;
	s)
		MINSIZE="$OPTARG"
		;;
	t)
		MINAGE="$OPTARG"
		;;
	S)
		MAXSIZE="$OPTARG"
		;;
	T)
		MAXAGE="$OPTARG"
		;;
	d)
		SSTEP="$OPTARG"
		;;
	D)
		TSTEP="$OPTARG"
		;;
	*)
		echo "Invalid option"
		usage
		exit 1;
		;;
	?)
		usage
		exit 0
		;;
	esac
done

[ -n "$FILENAME" ] || { echo "No file name specified." ; exit 1; }
[ -n "$MINSIZE" ] || { echo "No minimal size specified." ; exit 1; }
[ -n "$MINAGE" ] || { echo "No minimal age specified." ; exit 1; }
[ -n "$MAXSIZE" ] || { echo "No maximal size specified." ; exit 1; }
[ -n "$MAXAGE" ] || { echo "No maximal age specified." ; exit 1; }
[ -n "$SSTEP" ] || { echo "No size step specified." ; exit 1; }
[ -n "$TSTEP" ] || { echo "No time step specified." ; exit 1; }

INITIAL_MINAGE=$MINAGE

declare -a OUTS

i=0
while [ "$MINSIZE" -le "$MAXSIZE" ]
do
	MINAGE=$INITIAL_MINAGE;
	while [ "$MINAGE" -le "$MAXAGE" ]
	do
		i=$[$i +1];
		OUTS[$i]=$(./cache_sim -f $FILENAME -s $MINSIZE -t $MINAGE)
		MINAGE=$[$MINAGE + $TSTEP];
	done
	MINSIZE=$[$MINSIZE + $SSTEP];
done

echo "size,time,hit,miss,percent_hit"

for p in ${OUTS[@]}
do
	echo $p
done