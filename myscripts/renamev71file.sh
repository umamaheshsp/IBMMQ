#AMQ18252.0.FMT: 11:09:51.000602    18252.3           :           PID : 18252 Process : amqzlaa0 (64-bit)

#cat process.out | while read fname tm tidpid colon1 PID colon2 pid str1 colon3 process other
cat process.out|while read fname f2 f3 f4 process other
do
        echo "$fname mv AMQ${pid}.0.FMT ${process}_AMQ${pid}.0.FMT"
        mv AMQ${pid}.0.FMT ${process}_AMQ${pid}.0.FMT
done
