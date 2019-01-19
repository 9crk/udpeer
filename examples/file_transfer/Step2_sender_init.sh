echo yyy |nc -u $1 $2 & sleep 1;killall nc
