#!/bin/bash
for i in $(seq 1 10000)
do
  echo $i >  /sys/bus/platform/devices/b000000.adder/dataA 
done

