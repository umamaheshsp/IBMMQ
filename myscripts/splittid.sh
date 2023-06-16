#!/bin/sh
# Illustrates use of a while loop to read a file
file=$1
cat $file|   \
while read line
do
        tid=`echo "$line" | awk -F" " '{print $2}'`
        #tid=`echo $line | cut -c21-29`
        #tid=`echo "$line"| cut -f2`
        #echo $tid
        #echo "$line"
        echo "$line" >> $file.$tid
done

