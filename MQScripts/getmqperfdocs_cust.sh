#!/bin/sh
#WMQ Documents collection script V0.1a
#

if [ $# -ne 4 ]
then
  echo "Usage: getmqperfdocs.sh <qmgr name> <qdepth> <interval1> <interval2>"
  echo "Example: getmqperfdocs.sh testqmgr  2000 600 1"
  echo "qmgr name: Name of the queue manager" 
  echo "qdepth: Queue depth of the SCTQ at which the interval changes from interval1 to interval2"
  exit 1
fi

qmgr="$1"
qdepth="$2"
ndelay="$3"
sddelay="$4"
curdelay=$ndelay
doccount=0
logdir="/tmp"
nfreq=90
stamqlfreq=600

while true
do
  mydate=`date +%Y%m%d`
  qsoutfile="$logdir/qs_$mydate.out"
  nqschsoutfile="$logdir/nqschs_$mydate.out"
  sdqschsoutfile="$logdir/sdqschs_$mydate.out"
  stoutfile="$logdir/st_$mydate.out"
  psoutfile="$logdir/ps_$mydate.out"
  amqloutfile="/tmp/amqldmpasctq_$mydate.out"

  echo "DATE TIME : \c" >> $psoutfile
  date >> $psoutfile
 
 #ps -ef -o user,pid,ppid,nice,stime,time,etime,vsz,rss,osz,pcpu,pmem,args >> $psoutfile
  ps -ef -o user,pid,ppid,nice,stime,time,etime,vsz,pcpu,pmem,args >> $psoutfile
 
  echo "DATE TIME : \c" >> $qsoutfile
  date >> $qsoutfile

  curqdepth=`echo "dis qs(SYSTEM.CLUSTER.TRANSMIT.QUEUE) all" | runmqsc $qmgr | tee -a $qsoutfile | grep CURDEPTH | \
       awk '{i1=index($1,"("); i2=index($1,")")-1; curdepth=substr($1,i1+1,i2-i1); print curdepth}'`

   echo "DATE TIME : \c" >> $nqschsoutfile
    date >> $nqschsoutfile
    echo "dis qs(*) all 
          dis chs(*) all 
         " | runmqsc $qmgr >> $nqschsoutfile
         
         
  if [ $curqdepth -ge $qdepth ]
  then
    echo "DATE TIME : \c" >> $sdqschsoutfile
    date >> $sdqschsoutfile
    echo "dis qs(SYSTEM.CLUSTER.TRANSMIT.QUEUE) all 
          dis chs(TO.HKIB01P1.HK6.DF) all 
          dis chs(TO.HKIB02P1.HK6.DF) all 
         " | runmqsc $qmgr >> $sdqschsoutfile

    tval=`expr $doccount % $stamqlfreq`
    if [ $tval -eq 0 ]
    then
      count=0
      while [ $count -lt 3 ]
      do
        echo "DATE TIME : \c" >> $stoutfile
        date >> $stoutfile
        stackit -o stack -n amqzmuc0 -n amqzlaa0 -n runmqchl -n amqrmppa >> $stoutfile
        count=`expr $count + 1`
      done

 #     echo "DATE TIME : \c" >> /tmp/amqldmpa_`date '+%y%m%d%s'`.out
 #     date >> /tmp/amqldmpa_`date '+%y%m%d%s'`.out
      amqldmpa -m $qmgr -q SYSTEM.CLUSTER.TRANSMIT.QUEUE -u 1 -c A -d 3 -f /tmp/amqldmpa_`date '+%y%m%d%s'`.out
    fi

    doccount=`expr $doccount + 1`
    curdelay=$sddelay
  else
    doccount=0
    curdelay=$ndelay
  fi

  #tval=`expr $doccount % $nfreq`
  #if [ $tval -eq 0 ]
  #then
  #  echo "DATE TIME : \c" >> $nqschsoutfile
  #  date >> $nqschsoutfile
  #  echo "dis qs(*) all 
  #        dis chs(*) all 
  #       " | runmqsc $qmgr >> $nqschsoutfile
  #fi

  sleep $curdelay
done

