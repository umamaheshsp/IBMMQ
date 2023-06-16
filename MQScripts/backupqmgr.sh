#!/bin/ksh
endmqm -i QM1
dltmqm QM1
crtmqm -ll -lp 3 -lf 8096 QM1
cd /var/mqm
tar -cvf QM1.tar qmgrs/QM1 log/QM1
strmqm QM1
runmqsc QM1 <<EOF
def ql(testq1)
EOF

runmqsc QM1 <<EOF
reset qmgr type(advancelog)
dis qmstatus all
EOF

cp /var/mqm/log/QM1/active/S0000000.LOG /tmp

runmqsc QM1 <<EOF
reset qmgr type(advancelog)
dis qmstatus all
EOF

cp /var/mqm/log/QM1/active/S0000001.LOG /tmp

runmqsc QM1 <<EOF
def ql(testq2)
EOF

endmqm -i QM1

dltmqm QM1


cd /var/mqm
tar -xvf QM1.tar


echo "QueueManager:" >> /var/mqm/mqs.ini
echo "   Name=QM1" >> /var/mqm/mqs.ini
echo "   Prefix=/var/mqm" >> /var/mqm/mqs.ini
echo "   Directory=QM1" >> /var/mqm/mqs.ini
#echo "   InstallationName=MQNI08000002" >> /var/mqm/mqs.ini

cp /tmp/S0000000.LOG /var/mqm/log/QM1/active
strmqm -r QM1
cp /tmp/S0000001.LOG /var/mqm/log/QM1/active
strmqm -r QM1

strmqm -a QM1
strmqm QM1

runmqsc QM1 <<EOF
dis ql(testq*)
EOF  

