QMGR="EAIGPPQM1"
CHLNAMES="TO.EAIMBPP2QM TO.EAIMBPP1QM TO.EAIGPPQM2"
MONINTERVAL=60
CONTINUE=1
LOGFILE=/tmp/mylog.out

strmqtrc -m EAIGPPQM1 -t all -t detail -l 50

while [ $CONTINUE -eq 1 ]
do

  date >> ${LOGFILE}
  ps -elf >> ${LOGFILE}
  for chl in ${CHLNAMES}
  do
     echo "DIS CHS(${chl}) ALL WHERE (STATUS EQ RUNNING)" | /usr/mqm/bin/runmqsc ${QMGR} | tee -a ${LOGFILE} | grep AMQ8420 2>&1 > /dev/null
     if [ $? -eq 0 ]
     then
       echo "$chl NOT FUNNING !!!!!!!!!!!!!!!!!" >> ${LOGFILE}
       CONTINUE=0
       echo "Ending MQ trace ..." >> ${LOGFILE}
       /usr/mqm/bin/endmqtrc -a
       echo "DIS CHS(*) ALL" | /usr/mqm/bin/runmqsc ${QMGR} >> ${LOGFILE}
       #exit
       break
     fi
  done
  
  echo "----------------------------------------------------------------------------------------------------" >> ${LOGFILE}
  if [ $CONTINUE -eq 1 ]
  then
    sleep ${MONINTERVAL}
  fi
done
echo "... EXIT  ..." >> ${LOGFILE}
