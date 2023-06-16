cat tids.out |awk -F"."  '{print $1 " " $2}' |while read pid tid
do 
  fname="AMQ$pid.0.FMT"
  echo "grep  $pid.$tid $fname >> $pid.$tid.tfmt" 
  grep  $pid.$tid *$fname >> $pid.$tid.tfmt
done
