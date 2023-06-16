#AMQ10158142.0.FMT:| Program Name      :- runmqchi                                         |

ls -l *.FMT |while read a b c d e f g h fname
do
  process=`head -30 $fname | grep "Program Name" | awk '{print $5}'`
  #echo "$fname \c"
  echo "mv $fname ${process}_${fname}"
  mv $fname ${process}_${fname}
done


#cat progname.out|while read fname f2 f3 f4 process other
#do
#        echo "$fname mv AMQ${pid}.0.FMT ${process}_AMQ${pid}.0.FMT"
#        mv AMQ${pid}.0.FMT ${process}_AMQ${pid}.0.FMT
#done
