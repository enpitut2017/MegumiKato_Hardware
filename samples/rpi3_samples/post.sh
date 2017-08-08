
for f in `seq 1 1 100`
do
     echo "a"
     curl -X POST -H "Content-Type: application/json" -H "Accept: application/json" -d  '{"serial":"raspi", "latitude":38.258595, "longitude":139.6950221}' http://192.168.11.143:3000/positions
     sleep 1s
done
