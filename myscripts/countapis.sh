if [ $# -eq 1 ]
then
  fname="$1"
else
  fname="mqapis.out"
fi

tfname="${fname}_data.out"

grep "zlaMQGET rc=" $fname > zlaMQGET.out
grep "zlaMQGETM rc=" $fname > zlaMQGETM.out
grep "zlaMQPUT rc=" $fname > zlaMQPUT.out
grep "! zlaMQPUT rc=" $fname > zlaMQPUT_fail.out

echo "Number of MQGETs               : \c" >> $tfname
wc -l zlaMQGET.out |awk '{print $1}' >> $tfname

echo "Number of MQGETMs              : \c" >> $tfname
wc -l zlaMQGETM.out |awk '{print $1}' >> $tfname

echo "Number of MQPUTs               : \c" >> $tfname
wc -l zlaMQPUT.out |awk '{print $1}' >> $tfname

echo "Number of Successful MQPUTs     : \c" >> $tfname
grep "rc=OK" zlaMQPUT.out | wc -l >> $tfname

echo "Number of failed MQPUTs         : \c" >> $tfname
grep "}! zlaMQPUT" zlaMQPUT.out | wc -l >> $tfname

echo "Number of Successful MQGETs    : \c" >> $tfname
grep "rc=OK" zlaMQGET.out | wc -l >> $tfname

echo "MQRC_NO_MSG_AVAILABLE          : \c" >> $tfname
grep NO_MSG_AVAIL zlaMQGET.out | wc -l >> $tfname

echo "MQRC_TRUNCATED_MSG_FAILED      : \c" >> $tfname
grep MQRC_TRUNCATED_MSG_FAILED zlaMQGET.out | wc -l >> $tfname

echo "MQRC_TRUNCATED_MSG_ACCEPTED    : \c" >> $tfname
grep MQRC_TRUNCATED_MSG_ACCEPTED zlaMQGET.out | wc -l >> $tfname

echo "-----------------------------------------------------------------------------" >> $tfname
 
