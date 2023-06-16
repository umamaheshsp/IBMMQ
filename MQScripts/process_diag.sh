export MQTMPDIR=/tmp
export PROCNAME='amqrmppa|amqzlaa0'
export QMGRNAME=INPFRAPRC1
trap progbreak INT
function progbreak() {
  # UNCOMMENT THIS SECTION IF TRACE IS REQUIRED 
  # echo Exiting, stopping trace
  # endmqtrc -a
  exit
}

# ensure that we start with no trace running

# UNCOMMENT THIS SECTION IF TRACE IS REQUIRED 
# endmqtrc -a

traceduration=10
monitorduration=600

pgrep "$PROCNAME" > $MQTMPDIR/proclist.txt
while [ 1 ]
do
  cat /dev/null > $MQTMPDIR/pidstotrace.txt
  amqldmpa -m $QMGRNAME -c K -d 8 -f $MQTMPDIR/kern$(date -d "today" +"%Y%m%d%H%M%S").txt
  ps -elf | grep 'amqzlaa0\|amqrmppa' > $MQTMPDIR/ps$(date -d "today" +"%Y%m%d%H%M%S").txt
  # build a list of processes matching PROCNAME which
  # have been created since the last time we checked

  pgrep "$PROCNAME" > $MQTMPDIR/proclist_tmp.txt
  while read process;
  do
    if grep -q "^$process$" $MQTMPDIR/proclist.txt
    then
      : #do nothing
    else
      echo $process >> $MQTMPDIR/pidstotrace.txt
      echo Taking SIGUSR2 for $process
      kill -USR2 $process
    fi
  done < $MQTMPDIR/proclist_tmp.txt

  # strmqtrc only allows one PID to be traced at once
  # so iterate through the list of PIDs we found and
  # trace each for an amount of time

  # UNCOMMENT THIS SECTION IF TRACE IS REQUIRED

  # while read process;
  # do
  # if [ -n "$process" ]; then
  #   echo Starting $traceduration seconds of trace: strmqtrc -tdetail -tall -i$process -m$QMGRNAME
  #   strmqtrc -tdetail -tall -i$process -m$QMGRNAME
  #   sleep $traceduration
  #   echo Ending trace for $process
  #   endmqtrc -a
  # fi
  # done < $MQTMPDIR/pidstotrace.txt

  # overwrite the old process list as all new PIDs have
  # now been traced

  mv $MQTMPDIR/proclist_tmp.txt $MQTMPDIR/proclist.txt
  sleep $monitorduration
done

