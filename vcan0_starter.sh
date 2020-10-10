sudo modprobe vcan echo=1
sudo ip link add dev vcan0 type vcan
sudo ip link set vcan0 up txqueuelen 100
sudo ip link set vcan0 up mtu 72
sudo tc qdisc add dev vcan0 root tbf rate 1mbit burst 24b limit 10000

