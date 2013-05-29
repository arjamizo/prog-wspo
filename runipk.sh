cmd=`find . -type f -executable | grep LAB7 | grep ipk  |  grep Debug` 
# | sed ':a;N;$!ba;s/\n/ \& ;/g'

echo $cmd

apo="'"

for i in $cmd 
do
xterm -e $apo$i$apo &
done


