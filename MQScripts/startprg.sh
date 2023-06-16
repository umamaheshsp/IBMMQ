count=0
while [ $count -le 50 ]
do
count=`expr $count + 1`
echo "---> $count"
./amqscnxc_thr &
done
