#!/bin/bash

MINSIZE=512
MINAGE=300
MAXSIZE=8192
MAXAGE=7200
SSTEP=64
TSTEP=300

function usage() {
	cat << EOF
Usage: $0 <OPTIONS>
Checks DNS zone serial numbers 
Options:
-f log file name
-s minimal tested cache size (entries, defaults to 512)
-t minimal tested entry age (seconds, defaults to 300)
-S maximal tested cache size (entries, defaults to 8192)
-T maximal tested entry age (seconds, defaults to 7200)
-d size step (defaults to 64)
-D time step (defaults to 300)
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

declare -a PERCENTAGES

i=0
while [ "$MINSIZE" -lt "$MAXSIZE" ]; do
	while [ "$MINAGE" -lt "MAXAGE" ]; do
		i=$[$i +1];
		${PERCENTAGES[$i]}="${./cache_sim -f $FILENAME -s $MINSIZE -t $MINAGE}"
		MINAGE=$[$MINAGE + $TSTEP];
	done
	MINSIZE=$[$MINSIZE + $SSTEP];
done

echo "echo ${PERCENTAGES[@]} | sort -n | tail -1"