BEGIN {
   diff=0
}
/apiLockExclusive rc=OK/{ stime=$2; stid=$3; stime_1[stid]=$2; split($2, var, "."); split(var[1], var1, ":"); starttime_sec[stid]=var1[3]; starttime_ms[stid]=var[2]}
/apiUnlockExclusive rc=OK/{ etime=$2; etid=$3; etime_1[etid]=$2; split($2, var, "."); split(var[1], var1, ":"); endtime_sec[etid]=var1[3]; endtime_ms[etid]=var[2]; 
                           
                          printf "etid =" etid " " stime_1[etid] " " etime_1[etid] " "
                          #if( starttime_ms[etid] == 0 )
                          if( etid in starttime_ms )
                           {
                            diff_ms = endtime_ms[etid]-starttime_ms[etid];
                            diff_sec = endtime_sec[etid]-starttime_sec[etid];
                            printf endtime_ms[etid] " " starttime_ms[etid] " " diff_sec ":" diff_ms " "
                            printf endtime_sec[etid] " " starttime_sec[etid] " " diff_sec ":" diff_ms " "
                            print "diff=" diff " --- "  "diff_sec=" diff_sec " "
                            #print "-> "  stime ":" etime
                            if(diff_sec >  diff )
                            {
                              diff=diff_sec
                            }
                           }
                          }
END {
  print "Highest Delay = " diff
}
