 09:42:29.921535  1032984.4          PID : 1032984 Process : amqzmuc0
 09:42:34.087683  1040692.1          PID : 1040692 Process : amqzdmaa
 09:42:29.866314  1057272.117259     PID : 1057272 Process : amqrmppa
 09:42:44.994230  1057758.1          PID : 1057758 Process : MMA_Client
 09:42:34.459773  1135430.1          PID : 1135430 Process : runmqsc
 09:43:21.181920  1233552.5          PID : 1233552 Process : runmqchl_nd
 09:42:30.012809  1237428.48         PID : 1237428 Process : amqzlaa0_nd
 09:42:29.863902  1286704.1          PID : 1286704 Process : runmqlsr_nd
 09:42:30.014206  1327256.1          PID : 1327256 Process : amqzlaa0_nd
 09:42:29.636438  1352152.5515       PID : 1352152 Process : amqzlaa0_nd

cat process.out | while read tm tidpid str1 colon1 pid str2 colon2 process
do
        echo "mv AMQ${pid}.0.FMT ${process}_AMQ${pid}.0.FMT"
        mv AMQ${pid}.0.FMT ${process}_AMQ${pid}.0.FMT
done
