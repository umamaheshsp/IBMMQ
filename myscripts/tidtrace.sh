cat get_mca_tids.out|while read pid tid
do
   echo "grep ${pid}.${tid} amqrmppa_AMQ${pid}.0.FMT > ${pid}.${tid}.gtfmt"
   grep ${pid}.${tid} amqrmppa_AMQ${pid}.0.FMT > ${pid}.${tid}.gtfmt
done
