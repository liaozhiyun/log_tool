min=1
max=50
while [ $min -le $max ]
do
echo $min
./test
sleep 1
min=`expr $min + 1`
done
