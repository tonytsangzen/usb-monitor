#/bin/sh
echo "Select mode(1):"
echo "1.localhost"
echo "2.ssh client"
read select

if [ "$select" == "2" ];then
  echo "Hostname:"
  read host
  echo "Username:"
  read user
  echo "Password:"
  read passwd
  nohup src/legency.py -r $host -u $user -p $passwd &
else
  nohup src/legency.py &
fi
