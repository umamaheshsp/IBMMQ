#!/bin/ksh
egrep "ApplPid|ApplTid|AgentPid|AgentTid|ApplName" *.txt | awk -F ':' '{/ApplPid/{ ApplPid=$1 }
/ApplTid/{ApplTid=$1}
/AgentPid/{AgentPid=$5}
/AgentTid/{AgentTid=$4}
/ApplName/{ print ApplPid "|" ApplTid }'


#egrep "ApplPid|ApplTid|AgentPid|AgentTid|ApplName" *.txt | awk -F":" '{ parm=&1; val=$2 }
