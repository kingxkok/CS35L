#!/bin/bash
COUNTER=1
let COUNTER="COUNTER + 1"
while true; do
  echo "$COUNTER"
#  let COUNTER="COUNTER + 1" 
#  let COUNTER="COUNTER % 10"
done >> ./big.txt
