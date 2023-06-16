#!/bin/ksh
#Documents collection script V0.1a
#

PS=1
PS_ENV=0
IPCS=1 
SVMON_GLOBAL=0
SVMON_PER_PROC=0
INSTFIX=0
LSLPP=0
UNAME=0
LS_ltrRi=1
AMQICLEN=1
IPCDIRS=1

if [ $# -ne 3 ]
then
  echo "Usage: getdocs.sh <qmgr name> <qmgr dir> <delay>"
  echo "Example: getdocs.sh testqmgr /var/mqm/qmgrs/testqmgr 600"
  exit 1
fi
QMGR_NAME="$1"
QMGR_DIR="$2"
delay="$3"

outfile="/var/mqm/errors/mqdocs.out"
echo "************MQ DOCS***************" > $outfile
echo "Using outfile $outfile" >> $outfile
date >> $outfile
dspmqver >> $outfile

while true
do

echo "DATE TIME : \c" >> $outfile
date >> $outfile
echo "-------------------------------------------------------------------------------" >> $outfile

if [ $PS -eq 1 ]
then
  echo "-----process status-----" >> $outfile
  ps -ef >> $outfile
  echo "-------------------------------------------------------------------------------" >> $outfile
fi

if [ $PS_ENV -eq 1 ]
then
  echo "-----env setup for processes-----" >> $outfile
  ps gleww >> $outfile
  echo "-------------------------------------------------------------------------------" >> $outfile
fi
#

if [ $IPCS -eq 1 ]
then
  echo "-----IPC status-----" >> $outfile
  ipcs -a >> $outfile
  echo "-------------------------------------------------------------------------------" >> $outfile
fi

if [ $SVMON_GLOBAL -eq 1 ]
then
  echo "-----global memory status-----" >> $outfile
  svmon -G  >> $outfile
  echo "-------------------------------------------------------------------------------" >> $outfile
fi

if [ $SVMON_PER_PROC -eq 1 ]
then
  echo "-----per process memory status-----" >> $outfile
  #for pid in `ps -ef |grep -v "UID    PID   PPID   C    STIME    TTY  TIME CMD" |awk '{print $2}'`
  #do
  #  echo "---PID : $pid---" >> $outfile
  #  svmon -nrP  $pid  >> $outfile 2>> $outfile
  #  echo "-----------------------------------------------------------" >> $outfile
  #done
  svmon -nrP >> $outfile
  echo "-------------------------------------------------------------------------------" >> $outfile
fi

if [ $INSTFIX -eq 1 ]
then
  echo "-----instfix output-----" >> $outfile
  instfix -aiv >> $outfile
  echo "-------------------------------------------------------------------------------" >> $outfile
fi

if [ $LSLPP -eq 1 ]
then
  echo "-----lslpp output-----" >> $outfile
  lslpp -l >> $outfile
  echo "-------------------------------------------------------------------------------" >> $outfile
fi

if [ $UNAME -eq 1 ]
then
  echo "-----uname -a-----" >> $outfile
  uname -a >> $outfile
  echo "-------------------------------------------------------------------------------" >> $outfile
fi

if [ $LS_ltrRi -eq 1 ]
then
  if [ -d "$QMGR_DIR" ]
  then
    echo "-----ls -ltrRi output-----" >> $outfile
    ls -ltrRi $QMGR_DIR/shmem $QMGR_DIR/?sem $QMGR_DIR/spipe $QMGR_DIR/@ipcc $QMGR_DIR/@app $QMGR_DIR/@qmpersist >> $outfile
    #ls -ltrRi $QMGR_DIR >> $outfile
    echo "-------------------------------------------------------------------------------" >> $outfile
  fi
fi

if [ $AMQICLEN -eq 1 ]
then
  if [ -n "$QMGR_NAME" ]
  then
    echo "-----amqiclen -c -v -i -m output-----" >> $outfile
    /usr/mqm/bin/amqiclen -c -v -i -m $QMGR_NAME >> $outfile 2>> $outfile
    echo "-------------------------------------------------------------------------------" >> $outfile
  fi
fi

if [ $IPCDIRS -eq 1 ]
then
  if [ -n "$QMGR_DIR" ]
  then
    echo "-----copying IPC directories-----" >> $outfile
    DATETIME=`date +%Y%m%d%H%M%S`
    FILENAME="/var/mqm/errors/"$DATETIME"IPC.tar"
    echo "IPC File Name : $FILENAME" >> $outfile
    tar -cf $FILENAME $QMGR_DIR/shmem $QMGR_DIR/?sem $QMGR_DIR/@ipcc/shmem $QMGR_DIR/@ipcc/?sem \
       $QMGR_DIR/@app/shmem $QMGR_DIR/@app/?sem $QMGR_DIR/@qmpersist/shmem $QMGR_DIR/@qmpersist/?sem >> $outfile

    echo "-------------------------------------------------------------------------------" >> $outfile
  fi
fi

sleep $delay

done #while true

exit 0
