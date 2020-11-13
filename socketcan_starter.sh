#!/bin/bash

if [ $# -lt 1 ]
        then
                echo "Socket CAN helper"
                echo "Usage: ./socketcan_starter.sh port"
                echo ""
                echo "Before usage install can-utils: "
                echo "$ git clone https://github.com/linux-can/can-utils.git"
                echo "$ cd can-utils"
                echo "$ make"

                sudo ifconfig slcan0 down
                sudo killall -9 slcand

                exit
fi

sudo slcan_attach -f -s5 -o ${1}
#attached tty /dev/ttyACM0 to netdevice slcan0
sudo slcand ${1} slcan0
sudo ifconfig slcan0 up
