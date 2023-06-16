#!/bin/ksh
grep '^|' *.FDC | awk '/Date\/Time/{ month=substr($5,1,3); date=$6; time=$7; file=$1 }
/Probe Id/{ probe=$5 }
/QueueManager/{qmgr=$4}
/Program Name/{prgname=$5}
/Component/{component=$4}
/Major Errorcode/{major=$5}
/Process/{process=$4}
/Thread/{thread=$4}
/Major Errorcode/{ print month " " date " " time " " file " "  qmgr " |" prgname "      |" process "." thread "|" component "   |" probe "     |"$5}'

# /Minor Errorcode/{ print month " " date " " time " " file " "  qmgr " |" prgname "      |" process.thread "|" component "   |" probe "     |"major "|" $5}'
