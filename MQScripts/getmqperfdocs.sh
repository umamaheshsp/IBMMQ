#!/bin/sh
#WMQ Documents collection script V0.1a
#

if [ $# -ne 7 ]
then
  echo "Usage: getmqperfdocs.sh <qmgr name> <qdepth> <interval1 in seconds> <interval2 in seconds> <interval3 in seconds> <number of dumps> <duration in seconds>"
  echo "Example: getmqperfdocs.sh testqmgr 5000 60 15 300 3 7200"
  echo "qmgr name: Name of the queue manager" 
  echo "qdepth: Queue depth of the SCTQ at which the interval changes from interval1 to interval2"
  echo "interval1: frequency of data colleciton during normal run"
  echo "interval2: frequency of data colleciton during slowdown"
  echo "interval3 to capture amqldmpa dumps during slowdown"
  echo "number of amqldmpa dumps"
  echo "duration of data collection"
  echo "All interval values should be in multiples of same number(e.g. 60 15 300 or 66 11 333 ...)"
  exit 1
fi

qmgr="$1"
msg=`dspmq -m ${qmgr} 2>&1`
if [ $? -eq 0 ]
then
  echo "$msg" | grep "Running" > /dev/null
  if [ $? -ne 0 ]
  then
    echo "Not able to connect to queue manager $qmgr"
    echo "$msg"
    exit 2
  fi
else
  echo "Not able to connect to queue manager $qmgr"
  echo "$msg"
  exit 3
fi

# To capture the documents set the value to 1

PS=0                 # Process status
ST=0                 # Stack trace
LOG=1                # amqldmpa logger dump
KERN=1               # amqldmpa kernel dump
DAP=1                # amqldmpa transaction dump
QSTATE=1             # amqldmpa queue state dump


qdepth=$2
ndelay="$3"
sddelay="$4"
curdelay=$ndelay
doccount=0
logdir="/tmp"
stamqlfreq=$5
ndumps=$6
duration=$7
timeelapsed=0

while true
do
  mydate=`date +%Y%m%d`
  qsoutfile="$logdir/qs_$mydate.out"
  nqschsoutfile="$logdir/nqschs_$mydate.out"
  sdqschsoutfile="$logdir/sdqschs_$mydate.out"
  stoutfile="$logdir/st_$mydate.out"
  psoutfile="$logdir/ps_$mydate.out"
  amqloutdir="/var/mqm/trace"

  if [ $PS -eq 1 ]
  then
    echo "DATE TIME : \c" >> $psoutfile
    date >> $psoutfile
    ps -ef -o user,pid,ppid,nice,stime,time,etime,vsz,pcpu,pmem,args >> $psoutfile
  fi

 
  echo "DATETIME START: \c" >> $qsoutfile
  date >> $qsoutfile

  curqdepth=`echo "dis qs(SYSTEM.CLUSTER.TRANSMIT.QUEUE) all" | runmqsc $qmgr | tee -a $qsoutfile | grep CURDEPTH | \
       awk '{i1=index($1,"("); i2=index($1,")")-1; curdepth=substr($1,i1+1,i2-i1); print curdepth}'`

  if [ $curqdepth -ge $qdepth ]
  then
    echo "DATE TIME : \c" >> $sdqschsoutfile
    date >> $sdqschsoutfile
    echo "dis qs(SYSTEM.CLUSTER.TRANSMIT.QUEUE) all 
          dis qs(TESTQ) all 
          dis chs(MYCHL*) all 
          dis chs(TESTCHL) all 
         " | runmqsc $qmgr >> $sdqschsoutfile

    #tval=`expr $doccount % $stamqlfreq`
    tval=`expr $timeelapsed % $stamqlfreq`
    if [ $tval -eq 0 -a $doccount -le $ndumps ]
    then
      count=0
      if [ $ST -eq 1 ]
      then
        while [ $count -lt 3 ]
        do
          echo "DATE TIME : \c" >> $stoutfile
          date >> $stoutfile
          stackit -o stack -n amqzmuc0 -n amqzlaa0 -n runmqchl -n amqrmppa >> $stoutfile
          count=`expr $count + 1`
        done
      fi

      if [ $QSTATE -eq 1 ]
      then
        echo "amqldmpa..."
        tdate=`date '+%y%m%d%H%M%S'`;
        amqldmpa -m $qmgr -c H -d 4 -f $amqloutdir/${qmgr}_logger${tdate}.out &
        amqldmpa -m $qmgr -c A -u 1 -q SYSTEM.CLUSTER.TRANSMIT.QUEUE -d 3 -n 6 -s 10 -f $amqloutdir/${qmgr}_dapSCTQ${tdate}.out &
        amqldmpa -m $qmgr -c K -d 8 -f $amqloutdir/${qmgr}_kern${tdate}.out &
        amqldmpa -m $qmgr -c A -d 15 -o 2 -f $amqloutdir/${qmgr}_dap${tdate}.out &
      fi
    fi

    doccount=`expr $doccount + 1`
    curdelay=$sddelay
  else
    doccount=0
    curdelay=$ndelay
  fi

  timeelapsed=`expr $timeelapsed + $curdelay`

  #tval=`expr $doccount % $nfreq`
  tval=`expr $timeelapsed % $ndelay`
  if [ $tval -eq 0 ]
  then
    echo "DATE TIME : \c" >> $nqschsoutfile
    date >> $nqschsoutfile
    echo "dis qs(*) all 
          dis chs(*) all 
         " | runmqsc $qmgr >> $nqschsoutfile
  fi

  if [ $timeelapsed -ge $duration ]
  then
    echo "Ending..."
    exit 0
  fi

  sleep $curdelay
done

