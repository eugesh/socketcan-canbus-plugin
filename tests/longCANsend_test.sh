#!/bin/bash

for i in $(seq 1 1 50000)
do
        #echo "Welcome $i times"
        cansend vcan0 022#FEFF01AB0804
        cansend vcan0 422#0000080800040000
        cansend vcan0 422#0220411501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108000000000000
        cansend vcan0 422#0000000000000000
        cansend vcan0 422#0000851501088515
        cansend vcan0 422#0108000000008515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088993
        cansend vcan0 422#0108A5930108552F
        cansend vcan0 422#0108852F01088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#01088DC00108B1C4
        cansend vcan0 422#010885150108B9C0
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#010819D201088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#01087D9B01080588
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#010885150108C193
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#010865B401088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501088515
        cansend vcan0 422#0108851501080000
        cansend vcan0 422#0000000000000000
        cansend vcan0 422#0000000000000000
        cansend vcan0 422#0000000000000000
        cansend vcan0 422#0000000000000000
        cansend vcan0 422#0000000000000000
        cansend vcan0 422#0000000000000000
        cansend vcan0 422#00000000000010B5
        cansend vcan0 422#054C237833B9044B
        cansend vcan0 422#13B10448AFF30080
        cansend vcan0 422#0123237010BD900E
        cansend vcan0 422#0020000000000CAF
        cansend vcan0 422#020808B5034B1BB1
        cansend vcan0 422#03490348AFF30080
        cansend vcan0 422#08BD00000000940E
        cansend vcan0 422#00200CAF020881F0
        cansend vcan0 422#004102E000BF83F0
        cansend vcan0 422#004330B54FEA4104
        cansend vcan0 422#4FEA430594EA050F
        cansend vcan0 422#08BF90EA020F1FBF
        cansend vcan0 422#54EA000C55EA020C
        cansend vcan0 422#7FEA645C7FEA655C
        cansend vcan0 422#00F0E2804FEA5454
        cansend vcan0 422#D4EB5555B8BF6D42
        cansend vcan0 422#0CDD2C4480EA0202
        cansend vcan0 422#81EA030382EA0000
        cansend vcan0 422#83EA010180EA0202
        cansend vcan0 422#81EA0303362D88BF
        cansend vcan0 422#30BD11F0004F4FEA
        cansend vcan0 422#01314FF4801C4CEA
        cansend vcan0 422#113102D0404261EB
        cansend vcan0 422#410113F0004F4FEA
        cansend vcan0 422#03334CEA133302D0
        cansend vcan0 422#524263EB430394EA
        cansend vcan0 422#050F00F0A780A4F1
        cansend vcan0 422#0104D5F1200E0DDB
        cansend vcan0 422#02FA0EFC22FA05F2
        cansend vcan0 422#801841F1000103FA
        cansend vcan0 422#0EF2801843FA05F3
        cansend vcan0 422#59410EE0A5F12005
        cansend vcan0 422#0EF1200E012A03FA
        cansend vcan0 422#0EFC28BF4CF0020C
        cansend vcan0 422#43FA05F3C01851EB
        cansend vcan0 422#E37101F0004507D5
        cansend vcan0 422#4FF0000EDCF1000C
        cansend vcan0 422#7EEB00006EEB0101
        cansend vcan0 422#B1F5801F1BD3B1F5
        cansend vcan0 422#001F0CD349085FEA
        cansend vcan0 422#30004FEA3C0C04F1
        cansend vcan0 422#01044FEA445212F5
        cansend vcan0 422#800F80F09A80BCF1
        cansend vcan0 422#004F08BF5FEA500C
        cansend vcan0 422#50F1000041EB0451
        cansend vcan0 422#41EA050130BD5FEA
        cansend vcan0 422#4C0C404141EB0101
        cansend vcan0 422#11F4801FA4F10104
        cansend vcan0 422#E9D191F0000F04BF
        cansend vcan0 422#01460020B1FA81F3
        cansend vcan0 422#08BF2033A3F10B03
        cansend vcan0 422#B3F120020CDA0C32
        cansend vcan0 422#08DD02F1140CC2F1
        cansend vcan0 422#0C0201FA0CF021FA
        cansend vcan0 422#02F10CE002F11402
        cansend vcan0 422#D8BFC2F1200C01FA
        cansend vcan0 422#02F120FA0CFCDCBF
        cansend vcan0 422#41EA0C019040E41A
        cansend vcan0 422#A2BF01EB04512943
        cansend vcan0 422#30BD6FEA04041F3C
        cansend vcan0 422#1CDA0C340EDC04F1
        cansend vcan0 422#1404C4F1200220FA
        cansend vcan0 422#04F001FA02F340EA
        cansend vcan0 422#030021FA04F345EA
        cansend vcan0 422#030130BDC4F10C04
        cansend vcan0 422#C4F1200220FA02F0
        cansend vcan0 422#01FA04F340EA0300
        cansend vcan0 422#294630BD21FA04F0
        cansend vcan0 422#294630BD94F0000F
        cansend vcan0 422#83F4801306BF81F4
        cansend vcan0 422#80110134013D4EE7
        cansend vcan0 422#7FEA645C18BF9E5E
done
