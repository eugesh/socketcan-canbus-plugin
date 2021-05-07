sudo modprobe vcan echo=1
sudo ip link add dev vcan0 type vcan
sudo ip link set vcan0 txqueuelen 1000
sudo ip link set vcan0 mtu 72
sudo ip link set up vcan0
sudo tc qdisc add dev vcan0 root tbf rate 1mbit burst 240b limit 10000

