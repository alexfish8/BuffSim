#!/bin/sh
#
# monitor_resource.sh
#
# sh monitor_resource.sh <pid>
#
# Monitor and log the resource usage of the given process until
# it terminates
#

NUM_EXPECTED=1
USAGE="Usage: sh monitor_resource.sh <sim> <input>"


if [ $# -lt $NUM_EXPECTED ]; then
  echo $USAGE
  exit
fi

sim=$1
input=$2

$sim < $input &

pid=$!
kill -0 $pid > /dev/null
alive=$?
while [ $alive -eq 0 ]; do
  top -b -n 1 -p $pid | grep $pid
  sleep 4
  kill -0 $pid > /dev/null
  alive=$?
done

echo $1



