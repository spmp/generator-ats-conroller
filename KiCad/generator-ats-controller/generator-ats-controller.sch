EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_Nano_v3.x A?
U 1 1 5F25F1ED
P 2550 2450
F 0 "A?" H 2550 1361 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 2550 1270 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 2550 2450 50  0001 C CIN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 2550 2450 50  0001 C CNN
	1    2550 2450
	1    0    0    -1  
$EndComp
$Comp
L Interface_UART:MAX232 U?
U 1 1 5F261638
P 2550 5850
F 0 "U?" H 2550 7231 50  0000 C CNN
F 1 "MAX232" H 2550 7140 50  0000 C CNN
F 2 "" H 2600 4800 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/max232.pdf" H 2550 5950 50  0001 C CNN
	1    2550 5850
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F2662F8
P 1550 5100
F 0 "C?" H 1665 5146 50  0000 L CNN
F 1 "0.1uF" H 1665 5055 50  0000 L CNN
F 2 "" H 1588 4950 50  0001 C CNN
F 3 "~" H 1550 5100 50  0001 C CNN
	1    1550 5100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F2691ED
P 3550 5100
F 0 "C?" H 3665 5146 50  0000 L CNN
F 1 "0.1uF" H 3665 5055 50  0000 L CNN
F 2 "" H 3588 4950 50  0001 C CNN
F 3 "~" H 3550 5100 50  0001 C CNN
	1    3550 5100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F269678
P 4000 5300
F 0 "C?" H 4115 5346 50  0000 L CNN
F 1 "0.1uF" H 4115 5255 50  0000 L CNN
F 2 "" H 4038 5150 50  0001 C CNN
F 3 "~" H 4000 5300 50  0001 C CNN
	1    4000 5300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5F269EC4
P 4300 5900
F 0 "C?" H 4415 5946 50  0000 L CNN
F 1 "0.1uF" H 4415 5855 50  0000 L CNN
F 2 "" H 4338 5750 50  0001 C CNN
F 3 "~" H 4300 5900 50  0001 C CNN
	1    4300 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1750 4950 1550 4950
Wire Wire Line
	1750 5250 1550 5250
Wire Wire Line
	3350 4950 3550 4950
Wire Wire Line
	3350 5250 3550 5250
Text GLabel 2750 1300 1    50   Input ~ 0
5v
Text GLabel 2450 1100 1    50   Input ~ 0
Vin
Wire Wire Line
	2750 1450 2750 1300
Wire Wire Line
	2450 1450 2450 1100
Text GLabel 4000 4850 1    50   Input ~ 0
5v
$Comp
L power:GND #PWR?
U 1 1 5F295048
P 2550 3850
F 0 "#PWR?" H 2550 3600 50  0001 C CNN
F 1 "GND" H 2555 3677 50  0000 C CNN
F 2 "" H 2550 3850 50  0001 C CNN
F 3 "" H 2550 3850 50  0001 C CNN
	1    2550 3850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F295430
P 2550 7250
F 0 "#PWR?" H 2550 7000 50  0001 C CNN
F 1 "GND" H 2555 7077 50  0000 C CNN
F 2 "" H 2550 7250 50  0001 C CNN
F 3 "" H 2550 7250 50  0001 C CNN
	1    2550 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 5450 4000 5450
Text GLabel 1750 1850 0    50   Input ~ 0
Tx
Text GLabel 1750 1950 0    50   Input ~ 0
Rx
Text GLabel 1600 5950 0    50   Input ~ 0
Tx
Text GLabel 1600 6350 0    50   Input ~ 0
Rx
Wire Wire Line
	2550 3450 2550 3750
Wire Wire Line
	2650 3450 2650 3750
Wire Wire Line
	2650 3750 2550 3750
Connection ~ 2550 3750
Wire Wire Line
	2550 3750 2550 3850
Wire Wire Line
	2050 1850 1750 1850
Wire Wire Line
	2050 1950 1750 1950
Wire Wire Line
	1600 5950 1750 5950
Wire Wire Line
	1600 6350 1750 6350
Wire Wire Line
	2550 7050 2550 7250
Wire Wire Line
	6100 6750 6500 6750
$Comp
L power:GND #PWR?
U 1 1 5F29E1EF
P 6300 6950
F 0 "#PWR?" H 6300 6700 50  0001 C CNN
F 1 "GND" H 6305 6777 50  0000 C CNN
F 2 "" H 6300 6950 50  0001 C CNN
F 3 "" H 6300 6950 50  0001 C CNN
	1    6300 6950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 6850 6300 6850
Wire Wire Line
	6300 6850 6300 6950
Wire Wire Line
	6450 6950 6500 6950
Wire Wire Line
	3350 5750 4300 5750
$Comp
L power:GND #PWR?
U 1 1 5F2A2AEC
P 4300 6150
F 0 "#PWR?" H 4300 5900 50  0001 C CNN
F 1 "GND" H 4305 5977 50  0000 C CNN
F 2 "" H 4300 6150 50  0001 C CNN
F 3 "" H 4300 6150 50  0001 C CNN
	1    4300 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 6050 4300 6150
Text GLabel 2550 4400 1    50   Input ~ 0
5v
Wire Wire Line
	2550 4400 2550 4650
Wire Wire Line
	6100 7150 6450 7150
Wire Wire Line
	6450 7150 6450 6950
$Comp
L Isolator:PC817 U?
U 1 1 5F2A52F1
P 5650 1700
F 0 "U?" H 5650 2025 50  0000 C CNN
F 1 "PC817" H 5650 1934 50  0000 C CNN
F 2 "Package_DIP:DIP-4_W7.62mm" H 5450 1500 50  0001 L CIN
F 3 "http://www.soselectronic.cz/a_info/resource/d/pc817.pdf" H 5650 1700 50  0001 L CNN
	1    5650 1700
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J?
U 1 1 5F2BDEDA
P 6700 4000
F 0 "J?" H 6780 4042 50  0000 L CNN
F 1 "Conn_01x05" H 6780 3951 50  0000 L CNN
F 2 "" H 6700 4000 50  0001 C CNN
F 3 "~" H 6700 4000 50  0001 C CNN
	1    6700 4000
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x12 J?
U 1 1 5F2BF89F
P 6700 5600
F 0 "J?" H 6780 5592 50  0000 L CNN
F 1 "Conn_01x12" H 6780 5501 50  0000 L CNN
F 2 "" H 6700 5600 50  0001 C CNN
F 3 "~" H 6700 5600 50  0001 C CNN
	1    6700 5600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5F2C245F
P 6300 1600
F 0 "R?" V 6093 1600 50  0000 C CNN
F 1 "660" V 6184 1600 50  0000 C CNN
F 2 "" V 6230 1600 50  0001 C CNN
F 3 "~" H 6300 1600 50  0001 C CNN
	1    6300 1600
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5F2C2E26
P 5150 1350
F 0 "R?" H 5080 1304 50  0000 R CNN
F 1 "2.7k" H 5080 1395 50  0000 R CNN
F 2 "" V 5080 1350 50  0001 C CNN
F 3 "~" H 5150 1350 50  0001 C CNN
	1    5150 1350
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F2C345B
P 5150 1900
F 0 "#PWR?" H 5150 1650 50  0001 C CNN
F 1 "GND" H 5155 1727 50  0000 C CNN
F 2 "" H 5150 1900 50  0001 C CNN
F 3 "" H 5150 1900 50  0001 C CNN
	1    5150 1900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F2C39CB
P 6150 1900
F 0 "#PWR?" H 6150 1650 50  0001 C CNN
F 1 "GND" H 6155 1727 50  0000 C CNN
F 2 "" H 6150 1900 50  0001 C CNN
F 3 "" H 6150 1900 50  0001 C CNN
	1    6150 1900
	1    0    0    -1  
$EndComp
Text GLabel 5150 1000 1    50   Input ~ 0
5v
Wire Wire Line
	5150 1000 5150 1200
Wire Wire Line
	6150 1600 5950 1600
Wire Wire Line
	6150 1800 5950 1800
Wire Wire Line
	5350 1800 5150 1800
Wire Wire Line
	5150 1800 5150 1900
Wire Wire Line
	6150 1800 6150 1900
$Comp
L Isolator:PC817 U?
U 1 1 5F2E560D
P 5650 3000
F 0 "U?" H 5650 3325 50  0000 C CNN
F 1 "PC817" H 5650 3234 50  0000 C CNN
F 2 "Package_DIP:DIP-4_W7.62mm" H 5450 2800 50  0001 L CIN
F 3 "http://www.soselectronic.cz/a_info/resource/d/pc817.pdf" H 5650 3000 50  0001 L CNN
	1    5650 3000
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5F2E5617
P 6300 2900
F 0 "R?" V 6093 2900 50  0000 C CNN
F 1 "660" V 6184 2900 50  0000 C CNN
F 2 "" V 6230 2900 50  0001 C CNN
F 3 "~" H 6300 2900 50  0001 C CNN
	1    6300 2900
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F2E562B
P 5150 3200
F 0 "#PWR?" H 5150 2950 50  0001 C CNN
F 1 "GND" H 5155 3027 50  0000 C CNN
F 2 "" H 5150 3200 50  0001 C CNN
F 3 "" H 5150 3200 50  0001 C CNN
	1    5150 3200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5F2E5635
P 6150 3200
F 0 "#PWR?" H 6150 2950 50  0001 C CNN
F 1 "GND" H 6155 3027 50  0000 C CNN
F 2 "" H 6150 3200 50  0001 C CNN
F 3 "" H 6150 3200 50  0001 C CNN
	1    6150 3200
	1    0    0    -1  
$EndComp
Text GLabel 5150 2300 1    50   Input ~ 0
5v
Wire Wire Line
	6150 2900 5950 2900
Wire Wire Line
	6150 3100 5950 3100
Wire Wire Line
	5350 3100 5150 3100
Wire Wire Line
	5150 3100 5150 3200
Wire Wire Line
	6150 3100 6150 3200
Text GLabel 3500 3800 0    50   Input ~ 0
Vin
Text GLabel 6300 3900 0    50   Input ~ 0
StarterContactor
Text GLabel 6300 4000 0    50   Input ~ 0
RunIndicator
Text GLabel 6300 4100 0    50   Input ~ 0
Ignition
Text GLabel 6600 2900 2    50   Input ~ 0
RunIndicator
$Comp
L power:GND #PWR?
U 1 1 5F315D37
P 4500 4750
F 0 "#PWR?" H 4500 4500 50  0001 C CNN
F 1 "GND" H 4505 4577 50  0000 C CNN
F 2 "" H 4500 4750 50  0001 C CNN
F 3 "" H 4500 4750 50  0001 C CNN
	1    4500 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 3900 6500 3900
Wire Wire Line
	6300 4000 6500 4000
Wire Wire Line
	6300 4100 6500 4100
Wire Wire Line
	6500 4200 6300 4200
Text GLabel 6600 1600 2    50   Input ~ 0
RunStop
Wire Wire Line
	6450 2900 6600 2900
Wire Wire Line
	6450 1600 6600 1600
Wire Wire Line
	5150 2300 5150 2500
$Comp
L Device:R R?
U 1 1 5F2E5621
P 5150 2650
F 0 "R?" H 5080 2604 50  0000 R CNN
F 1 "2.7k" H 5080 2695 50  0000 R CNN
F 2 "" V 5080 2650 50  0001 C CNN
F 3 "~" H 5150 2650 50  0001 C CNN
	1    5150 2650
	-1   0    0    1   
$EndComp
Text GLabel 1750 2250 0    50   Input ~ 0
OutputIgnition
Text GLabel 1750 2350 0    50   Input ~ 0
OutputStarterContactor
Text GLabel 1750 2550 0    50   Input ~ 0
OutputIndicatorError
Text GLabel 1750 2650 0    50   Input ~ 0
OutputIndicatorWarning
Text GLabel 1750 2750 0    50   Input ~ 0
OutputIndicatorTemperature
Text GLabel 1750 2850 0    50   Input ~ 0
OutputIndicatorFuel
Wire Wire Line
	1750 2250 2050 2250
Wire Wire Line
	1750 2350 2050 2350
Wire Wire Line
	1750 2450 2050 2450
Wire Wire Line
	1750 2550 2050 2550
Wire Wire Line
	1750 2650 2050 2650
Wire Wire Line
	1750 2750 2050 2750
Wire Wire Line
	5000 1600 5150 1600
Text GLabel 5000 1600 0    50   Input ~ 0
InputRunStop
Wire Wire Line
	5150 1500 5150 1600
Connection ~ 5150 1600
Wire Wire Line
	5150 1600 5350 1600
Wire Wire Line
	4950 2900 5150 2900
Text GLabel 3350 2450 2    50   Input ~ 0
InputRunStop
Text GLabel 3350 2550 2    50   Input ~ 0
InputRunIndicator
Text GLabel 3350 2650 2    50   Input ~ 0
InputAnalogTemperature
Text GLabel 3350 2750 2    50   Input ~ 0
InputAnalogFuelLevel
Wire Wire Line
	3050 2450 3350 2450
Wire Wire Line
	3050 2550 3350 2550
Wire Wire Line
	3050 2650 3350 2650
Wire Wire Line
	3050 2750 3350 2750
Wire Wire Line
	5150 2800 5150 2900
Connection ~ 5150 2900
Wire Wire Line
	5150 2900 5350 2900
Text GLabel 4950 2900 0    50   Input ~ 0
InputRunIndicator
Text GLabel 6300 5100 0    50   Input ~ 0
Vin
Text GLabel 6300 5300 0    50   Input ~ 0
OutputIgnition
Text GLabel 6300 5400 0    50   Input ~ 0
OutputStarterContactor
Text GLabel 6300 5600 0    50   Input ~ 0
OutputIndicatorError
Text GLabel 1750 2450 0    50   Input ~ 0
OutputOutputEnabled
Text GLabel 6300 5500 0    50   Input ~ 0
OutputOutputEnabled
Text GLabel 6300 5700 0    50   Input ~ 0
OutputIndicatorWarning
Text GLabel 6300 5800 0    50   Input ~ 0
OutputIndicatorTemperature
Text GLabel 6300 5900 0    50   Input ~ 0
OutputIndicatorFuel
$Comp
L power:GND #PWR?
U 1 1 5F3C86D7
P 5250 5300
F 0 "#PWR?" H 5250 5050 50  0001 C CNN
F 1 "GND" H 5255 5127 50  0000 C CNN
F 2 "" H 5250 5300 50  0001 C CNN
F 3 "" H 5250 5300 50  0001 C CNN
	1    5250 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 5100 6300 5100
Wire Wire Line
	6500 5200 5250 5200
Wire Wire Line
	5250 5200 5250 5300
Wire Wire Line
	6500 5300 6300 5300
Wire Wire Line
	6500 5400 6300 5400
Wire Wire Line
	6500 5500 6300 5500
Wire Wire Line
	6500 5600 6300 5600
Wire Wire Line
	6500 5700 6300 5700
Wire Wire Line
	6500 5800 6300 5800
Wire Wire Line
	6500 5900 6300 5900
$Comp
L power:GND #PWR?
U 1 1 5F3D5EE3
P 6300 6350
F 0 "#PWR?" H 6300 6100 50  0001 C CNN
F 1 "GND" H 6305 6177 50  0000 C CNN
F 2 "" H 6300 6350 50  0001 C CNN
F 3 "" H 6300 6350 50  0001 C CNN
	1    6300 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 6200 6300 6200
Wire Wire Line
	6300 6200 6300 6350
Wire Wire Line
	6500 6100 6300 6100
Wire Wire Line
	6300 6100 6300 6200
Connection ~ 6300 6200
$Comp
L Device:Fuse F?
U 1 1 5F3E89D1
P 4000 3800
F 0 "F?" V 3803 3800 50  0000 C CNN
F 1 "Fuse" V 3894 3800 50  0000 C CNN
F 2 "" V 3930 3800 50  0001 C CNN
F 3 "~" H 4000 3800 50  0001 C CNN
	1    4000 3800
	0    1    1    0   
$EndComp
$Comp
L Triac_Thyristor:BT169B Q?
U 1 1 5F3F38E9
P 5250 4100
F 0 "Q?" H 5338 4146 50  0000 L CNN
F 1 "BT169B" H 5338 4055 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 5350 4025 50  0001 L CIN
F 3 "https://media.digikey.com/pdf/Data%20Sheets/NXP%20PDFs/BT169_Series.pdf" H 5250 4100 50  0001 L CNN
	1    5250 4100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5F3F3ED0
P 4500 4450
F 0 "R?" H 4430 4404 50  0000 R CNN
F 1 "2.7k" H 4430 4495 50  0000 R CNN
F 2 "" V 4430 4450 50  0001 C CNN
F 3 "~" H 4500 4450 50  0001 C CNN
	1    4500 4450
	-1   0    0    1   
$EndComp
$Comp
L Device:C C?
U 1 1 5F3F8267
P 4850 4450
F 0 "C?" H 4965 4496 50  0000 L CNN
F 1 "47nF" H 4965 4405 50  0000 L CNN
F 2 "" H 4888 4300 50  0001 C CNN
F 3 "~" H 4850 4450 50  0001 C CNN
	1    4850 4450
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Zener D?
U 1 1 5F40EA4B
P 4500 4000
F 0 "D?" V 4454 4079 50  0000 L CNN
F 1 "16v" V 4545 4079 50  0000 L CNN
F 2 "" H 4500 4000 50  0001 C CNN
F 3 "~" H 4500 4000 50  0001 C CNN
	1    4500 4000
	0    1    1    0   
$EndComp
Wire Wire Line
	4150 3800 4500 3800
Wire Wire Line
	4500 3800 4500 3850
Connection ~ 4500 3800
Wire Wire Line
	4500 4150 4500 4200
Connection ~ 4500 4200
Wire Wire Line
	4500 4200 4500 4300
Wire Wire Line
	4850 4200 4850 4300
Connection ~ 4850 4200
Wire Wire Line
	4850 4200 4500 4200
Wire Wire Line
	5250 3800 5250 3950
Wire Wire Line
	3850 3800 3500 3800
Wire Wire Line
	4500 4600 4500 4700
Wire Wire Line
	4500 4700 4850 4700
Wire Wire Line
	4850 4700 4850 4600
Connection ~ 4500 4700
Wire Wire Line
	4500 4700 4500 4750
Wire Wire Line
	5250 4700 5250 4250
Connection ~ 4850 4700
Wire Wire Line
	6300 4200 6300 4700
Connection ~ 5250 3800
Wire Wire Line
	5250 3800 6500 3800
Connection ~ 5250 4700
Wire Wire Line
	5250 4700 6300 4700
Wire Wire Line
	4500 3800 5250 3800
Wire Wire Line
	4850 4700 5250 4700
Wire Wire Line
	4850 4200 5100 4200
Wire Wire Line
	4000 4850 4000 5150
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 5F2976DB
P 6700 6850
F 0 "J?" H 6780 6892 50  0000 L CNN
F 1 "Conn_01x03" H 6780 6801 50  0000 L CNN
F 2 "" H 6700 6850 50  0001 C CNN
F 3 "~" H 6700 6850 50  0001 C CNN
	1    6700 6850
	1    0    0    -1  
$EndComp
Text GLabel 3600 5950 2    50   Input ~ 0
TxExternal
Text GLabel 3600 6350 2    50   Input ~ 0
RxExternal
Wire Wire Line
	3350 5950 3600 5950
Wire Wire Line
	3350 6350 3600 6350
Text GLabel 6100 7150 0    50   Input ~ 0
TxExternal
Text GLabel 6100 6750 0    50   Input ~ 0
RxExternal
$EndSCHEMATC
