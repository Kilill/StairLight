EESchema Schematic File Version 4
LIBS:StairLight-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Stair Led light controller"
Date "2019-01-12"
Rev "1.0"
Comp "LIT"
Comment1 "Creative Commons BY-CA"
Comment2 "Copyright : Kim Lilliestierna"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L StairLight-rescue:815-PCA9685Module U3
U 1 1 5C39DB5E
P 5800 1350
F 0 "U3" H 6400 1615 50  0000 C CNN
F 1 "PCA9685" H 6400 1524 50  0000 C CNN
F 2 "PCA9685Module:815" H 6850 1450 50  0001 L CNN
F 3 "https://www.arrow.com/en/products/815/adafruit-industries" H 6850 1350 50  0001 L CNN
F 4 "Power Management IC Development Tools PWM/Servo Driver PCA9685 12-bit" H 6850 1250 50  0001 L CNN "Description"
F 5 "" H 6850 1150 50  0001 L CNN "Height"
F 6 "815" H 6850 1050 50  0001 L CNN "Arrow Part Number"
F 7 "Adafruit" H 6850 950 50  0001 L CNN "Manufacturer_Name"
F 8 "815" H 6850 850 50  0001 L CNN "Manufacturer_Part_Number"
	1    5800 1350
	-1   0    0    -1  
$EndComp
$Comp
L StairLight-rescue:TrinketPro-TrinketPro5V U2
U 1 1 5C39DFFF
P 3900 3350
F 0 "U2" H 3050 4365 50  0000 C CNN
F 1 "TrinketPro" H 3050 3450 50  0000 C CNN
F 2 "TrinketPro:DIP1524W60P254L3800H400Q24N" H 3050 4300 50  0001 C CNN
F 3 "" H 3050 4300 50  0001 C CNN
	1    3900 3350
	1    0    0    -1  
$EndComp
$Comp
L StairLight-rescue:ULN2803DIP-SparkFun U5
U 1 1 5C39E133
P 7900 4200
F 0 "U5" H 7950 5165 50  0000 C CNN
F 1 "ULN2803DIP" H 7950 5074 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm_LongPads" H 7900 4350 50  0001 C CNN
F 3 "" H 7900 4200 50  0001 C CNN
	1    7900 4200
	1    0    0    -1  
$EndComp
Text Notes 1050 5350 0    50   ~ 0
+12V
Wire Wire Line
	3650 2700 3900 2700
$Comp
L StairLight-rescue:ULN2803DIP-SparkFun U4
U 1 1 5C39E0A4
P 7900 2050
F 0 "U4" H 7950 3015 50  0000 C CNN
F 1 "ULN2803DIP" H 7950 2924 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W10.16mm_LongPads" H 7900 2200 50  0001 C CNN
F 3 "" H 7900 2050 50  0001 C CNN
	1    7900 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 1350 7400 1350
Wire Wire Line
	7200 1550 7200 1750
Wire Wire Line
	7200 1750 7400 1750
Wire Wire Line
	5800 1550 7200 1550
Wire Wire Line
	7100 1650 7100 1950
Wire Wire Line
	7100 1950 7400 1950
Wire Wire Line
	5800 1650 7100 1650
Wire Wire Line
	5800 1750 7000 1750
Wire Wire Line
	7000 1750 7000 2150
Wire Wire Line
	7000 2150 7400 2150
Wire Wire Line
	5800 1850 6900 1850
Wire Wire Line
	6900 1850 6900 2350
Wire Wire Line
	6900 2350 7400 2350
Wire Wire Line
	5800 1950 6800 1950
Wire Wire Line
	6800 1950 6800 2550
Wire Wire Line
	6800 2550 7400 2550
Wire Wire Line
	5800 2050 6700 2050
Wire Wire Line
	6700 2050 6700 2750
Wire Wire Line
	6700 2750 7400 2750
Wire Wire Line
	5800 2150 6600 2150
Wire Wire Line
	5800 2250 6500 2250
Wire Wire Line
	5800 2350 6400 2350
Wire Wire Line
	5800 2450 6300 2450
Wire Wire Line
	5800 2550 6200 2550
Wire Wire Line
	5800 2650 6100 2650
Wire Wire Line
	5800 2750 6000 2750
Wire Wire Line
	5800 2850 5900 2850
Wire Wire Line
	6600 3500 7400 3500
Wire Wire Line
	6600 2150 6600 3500
Wire Wire Line
	6500 3700 7400 3700
Wire Wire Line
	6500 2250 6500 3700
Wire Wire Line
	6400 3900 7400 3900
Wire Wire Line
	6400 2350 6400 3900
Wire Wire Line
	6300 4100 7400 4100
Wire Wire Line
	6300 2450 6300 4100
Wire Wire Line
	6200 4300 7400 4300
Wire Wire Line
	6200 2550 6200 4300
Wire Wire Line
	6100 4500 7400 4500
Wire Wire Line
	6100 2650 6100 4500
Wire Wire Line
	6000 4700 7400 4700
Wire Wire Line
	6000 2750 6000 4700
Wire Wire Line
	5900 4900 7400 4900
Wire Wire Line
	5900 2850 5900 4900
Wire Wire Line
	7400 2950 7300 2950
Wire Wire Line
	7300 2950 7300 3050
Wire Wire Line
	7300 5150 7300 5100
Wire Wire Line
	7300 5100 7400 5100
Text GLabel 1900 5750 3    50   Input ~ 0
+12v
Text GLabel 9900 5100 0    50   Input ~ 0
+12V
Wire Wire Line
	8500 1550 10000 1550
Wire Wire Line
	10000 1550 10000 2450
Wire Wire Line
	10000 2450 10150 2450
Wire Wire Line
	8500 1750 9900 1750
Wire Wire Line
	9900 1750 9900 2550
Wire Wire Line
	9900 2550 10150 2550
Wire Wire Line
	8500 1950 9800 1950
Wire Wire Line
	9800 1950 9800 2650
Wire Wire Line
	9800 2650 10150 2650
Wire Wire Line
	8500 2150 9700 2150
Wire Wire Line
	9700 2150 9700 2750
Wire Wire Line
	9700 2750 10150 2750
Wire Wire Line
	8500 2350 9600 2350
Wire Wire Line
	9600 2350 9600 2850
Wire Wire Line
	9600 2850 10150 2850
Wire Wire Line
	8500 2550 9500 2550
Wire Wire Line
	9500 2550 9500 2950
Wire Wire Line
	9500 2950 10150 2950
Wire Wire Line
	8500 2750 9400 2750
Wire Wire Line
	9400 2750 9400 3050
Wire Wire Line
	9400 3050 10150 3050
Wire Wire Line
	8500 3700 9500 3700
Wire Wire Line
	9500 3700 9500 3250
Wire Wire Line
	9500 3250 10150 3250
Wire Wire Line
	8500 3900 9600 3900
Wire Wire Line
	9600 3900 9600 3350
Wire Wire Line
	9600 3350 10150 3350
Wire Wire Line
	8500 4100 9700 4100
Wire Wire Line
	9700 4100 9700 3450
Wire Wire Line
	9700 3450 10150 3450
Wire Wire Line
	8500 4300 9800 4300
Wire Wire Line
	9800 4300 9800 3550
Wire Wire Line
	9800 3550 10150 3550
Wire Wire Line
	8500 4500 9900 4500
Wire Wire Line
	9900 4500 9900 3650
Wire Wire Line
	9900 3650 10150 3650
Wire Wire Line
	8500 4700 10000 4700
Wire Wire Line
	10000 4700 10000 3750
Wire Wire Line
	10000 3750 10150 3750
Wire Wire Line
	8500 4900 10100 4900
Wire Wire Line
	10100 4900 10100 3850
Wire Wire Line
	10100 3850 10150 3850
Wire Wire Line
	8500 3500 9400 3500
Wire Wire Line
	9400 3500 9400 3150
Wire Wire Line
	9400 3150 10150 3150
Wire Wire Line
	8500 1350 10100 1350
Wire Wire Line
	10100 1350 10100 2350
Wire Wire Line
	10100 2350 10150 2350
Wire Wire Line
	5800 1450 7300 1450
Wire Wire Line
	7300 1450 7300 1550
Wire Wire Line
	7300 1550 7400 1550
Text Notes 600  3250 0    50   ~ 0
Up Sens
Text Notes 600  3850 0    50   ~ 0
Down Sens
Wire Wire Line
	1300 3150 1900 3150
Wire Wire Line
	2000 3150 2000 2800
Wire Wire Line
	2000 2800 2450 2800
Wire Wire Line
	1300 3750 1950 3750
Wire Wire Line
	2150 3750 2150 2900
Wire Wire Line
	2150 2900 2450 2900
Wire Wire Line
	2450 3600 2300 3600
Wire Wire Line
	3800 4600 3800 3350
Wire Wire Line
	3800 3350 4600 3350
Wire Wire Line
	2450 3700 2450 4250
Wire Wire Line
	2450 4250 2650 4250
Wire Wire Line
	3900 4250 3900 3450
Wire Wire Line
	3900 3450 4600 3450
Wire Wire Line
	4600 3650 4300 3650
$Comp
L StairLight-rescue:VCC-SparkFun #PWR0108
U 1 1 5C471391
P 3550 5550
F 0 "#PWR0108" V 3504 5679 50  0001 L CNN
F 1 "VCC" V 3550 5700 50  0000 L CNN
F 2 "" H 3550 5550 50  0001 C CNN
F 3 "" H 3550 5550 50  0001 C CNN
	1    3550 5550
	0    1    1    0   
$EndComp
Wire Wire Line
	3300 5550 3550 5550
$Comp
L StairLight-rescue:VCC-SparkFun #PWR0109
U 1 1 5C475F34
P 4250 3250
F 0 "#PWR0109" V 4296 3379 50  0001 L CNN
F 1 "VCC" V 4250 3400 50  0000 L CNN
F 2 "" H 4250 3250 50  0001 C CNN
F 3 "" H 4250 3250 50  0001 C CNN
	1    4250 3250
	0    -1   -1   0   
$EndComp
$Comp
L StairLight-rescue:VCC-SparkFun #PWR0110
U 1 1 5C47F9A4
P 3800 2900
F 0 "#PWR0110" V 3754 3029 50  0001 L CNN
F 1 "VCC" V 3800 3029 50  0000 L CNN
F 2 "" H 3800 2900 50  0001 C CNN
F 3 "" H 3800 2900 50  0001 C CNN
	1    3800 2900
	0    1    1    0   
$EndComp
Wire Wire Line
	3650 2900 3800 2900
$Comp
L Device:R R3
U 1 1 5C490E0E
P 2800 4050
F 0 "R3" V 2700 4050 50  0000 C CNN
F 1 "4.7K" V 2800 4050 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 2730 4050 50  0001 C CNN
F 3 "~" H 2800 4050 50  0001 C CNN
	1    2800 4050
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 5C490E80
P 2800 4450
F 0 "R4" V 2700 4450 50  0000 C CNN
F 1 "4.7k" V 2800 4450 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 2730 4450 50  0001 C CNN
F 3 "~" H 2800 4450 50  0001 C CNN
	1    2800 4450
	0    1    1    0   
$EndComp
Wire Wire Line
	2650 4050 2650 4250
Connection ~ 2650 4250
Wire Wire Line
	2650 4250 3900 4250
Wire Wire Line
	2650 4450 2650 4600
Wire Wire Line
	2650 4600 3800 4600
$Comp
L StairLight-rescue:VCC-SparkFun #PWR0111
U 1 1 5C49AD2B
P 3100 4050
F 0 "#PWR0111" V 3054 4179 50  0001 L CNN
F 1 "VCC" V 3100 4179 50  0000 L CNN
F 2 "" H 3100 4050 50  0001 C CNN
F 3 "" H 3100 4050 50  0001 C CNN
	1    3100 4050
	0    1    1    0   
$EndComp
$Comp
L StairLight-rescue:VCC-SparkFun #PWR0112
U 1 1 5C49ADA3
P 3100 4450
F 0 "#PWR0112" V 3054 4579 50  0001 L CNN
F 1 "VCC" V 3100 4579 50  0000 L CNN
F 2 "" H 3100 4450 50  0001 C CNN
F 3 "" H 3100 4450 50  0001 C CNN
	1    3100 4450
	0    1    1    0   
$EndComp
Wire Wire Line
	2950 4050 3100 4050
Wire Wire Line
	2950 4450 3100 4450
$Comp
L Device:R R2
U 1 1 5C4AFFC7
P 1800 3850
F 0 "R2" V 1900 3850 50  0000 C CNN
F 1 "100K" V 1800 3850 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 1730 3850 50  0001 C CNN
F 3 "~" H 1800 3850 50  0001 C CNN
	1    1800 3850
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R1
U 1 1 5C4B0021
P 1750 3250
F 0 "R1" V 1850 3250 50  0000 C CNN
F 1 "100K" V 1750 3250 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 1680 3250 50  0001 C CNN
F 3 "~" H 1750 3250 50  0001 C CNN
	1    1750 3250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1900 3250 1900 3150
Connection ~ 1900 3150
Wire Wire Line
	1900 3150 2000 3150
Wire Wire Line
	1950 3850 1950 3750
Connection ~ 1950 3750
Wire Wire Line
	1950 3750 2150 3750
Wire Wire Line
	2300 4600 2650 4600
Wire Wire Line
	2300 3600 2300 4600
Connection ~ 2650 4600
$Comp
L Connector:Screw_Terminal_01x08 J4
U 1 1 5C3FC45B
P 10350 2650
F 0 "J4" H 10430 2596 50  0000 L CNN
F 1 "Screw_Terminal_01x08" H 10430 2551 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_PT-1,5-8-3.5-H_1x08_P3.50mm_Horizontal" H 10350 2650 50  0001 C CNN
F 3 "~" H 10350 2650 50  0001 C CNN
	1    10350 2650
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x08 J5
U 1 1 5C3FC4DF
P 10350 3450
F 0 "J5" H 10430 3396 50  0000 L CNN
F 1 "Screw_Terminal_01x08" H 10430 3351 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_PT-1,5-8-3.5-H_1x08_P3.50mm_Horizontal" H 10350 3450 50  0001 C CNN
F 3 "~" H 10350 3450 50  0001 C CNN
	1    10350 3450
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J1
U 1 1 5C400318
P 1100 3150
F 0 "J1" H 1100 3250 50  0000 C CNN
F 1 "Screw_Terminal_01x02" H 1020 3276 50  0001 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_PT-1,5-2-3.5-H_1x02_P3.50mm_Horizontal" H 1100 3150 50  0001 C CNN
F 3 "~" H 1100 3150 50  0001 C CNN
	1    1100 3150
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J2
U 1 1 5C400384
P 1100 3750
F 0 "J2" H 1100 3850 50  0000 C CNN
F 1 "Screw_Terminal_01x02" H 1020 3876 50  0001 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_PT-1,5-2-3.5-H_1x02_P3.50mm_Horizontal" H 1100 3750 50  0001 C CNN
F 3 "~" H 1100 3750 50  0001 C CNN
	1    1100 3750
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J3
U 1 1 5C400DD1
P 1400 5350
F 0 "J3" H 1400 5150 50  0000 C CNN
F 1 "Screw_Terminal_01x02" H 1320 5476 50  0001 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_PT-1,5-2-3.5-H_1x02_P3.50mm_Horizontal" H 1400 5350 50  0001 C CNN
F 3 "~" H 1400 5350 50  0001 C CNN
	1    1400 5350
	-1   0    0    1   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J6
U 1 1 5C401A70
P 10350 5100
F 0 "J6" H 10300 5200 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 10430 5001 50  0001 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_PT-1,5-2-3.5-H_1x02_P3.50mm_Horizontal" H 10350 5100 50  0001 C CNN
F 3 "~" H 10350 5100 50  0001 C CNN
	1    10350 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 5100 10150 5100
Wire Wire Line
	10150 5200 10150 5100
Connection ~ 10150 5100
Text Notes 10450 5200 0    50   ~ 0
+12V to LED
Wire Notes Line
	10850 2250 10850 3950
Wire Notes Line
	10850 3950 10300 3950
Wire Notes Line
	10300 3950 10300 2250
Wire Notes Line
	10300 2250 10850 2250
Text Notes 10600 3100 0    50   ~ 0
LED
$Comp
L MP1584:MP1584 U1
U 1 1 5C3B9473
P 2700 5300
F 0 "U1" H 2700 5815 50  0000 C CNN
F 1 "MP1584" H 2700 5724 50  0000 C CNN
F 2 "MP1584Module:MP1584Module" H 2700 5650 50  0001 C CNN
F 3 "" H 2700 5650 50  0001 C CNN
	1    2700 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 5050 3300 5150
Wire Wire Line
	3550 5150 3300 5150
Connection ~ 3300 5150
Wire Wire Line
	3300 5450 3300 5550
Connection ~ 3300 5550
Wire Wire Line
	1600 5250 1600 5050
Wire Wire Line
	1600 5050 2100 5050
Wire Wire Line
	2100 5200 2100 5050
Connection ~ 2100 5050
Wire Wire Line
	1600 5550 1900 5550
Wire Wire Line
	1600 5350 1600 5550
Wire Wire Line
	2100 5550 2100 5400
Connection ~ 2100 5550
Wire Wire Line
	1900 5750 1900 5550
Connection ~ 1900 5550
Wire Wire Line
	1900 5550 2100 5550
Wire Wire Line
	4250 3250 4600 3250
$Comp
L power:GND #PWR?
U 1 1 5C46A779
P 3900 2700
F 0 "#PWR?" H 3900 2450 50  0001 C CNN
F 1 "GND" H 4050 2650 50  0000 C CNN
F 2 "" H 3900 2700 50  0001 C CNN
F 3 "" H 3900 2700 50  0001 C CNN
	1    3900 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5C46A84B
P 4300 3650
F 0 "#PWR?" H 4300 3400 50  0001 C CNN
F 1 "GND" H 4200 3650 50  0000 C CNN
F 2 "" H 4300 3650 50  0001 C CNN
F 3 "" H 4300 3650 50  0001 C CNN
	1    4300 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 3250 1450 3250
Wire Wire Line
	1300 3850 1450 3850
$Comp
L power:GND #PWR?
U 1 1 5C46A8B3
P 1450 3400
F 0 "#PWR?" H 1450 3150 50  0001 C CNN
F 1 "GND" H 1455 3227 50  0000 C CNN
F 2 "" H 1450 3400 50  0001 C CNN
F 3 "" H 1450 3400 50  0001 C CNN
	1    1450 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5C46A8F3
P 1450 4000
F 0 "#PWR?" H 1450 3750 50  0001 C CNN
F 1 "GND" H 1455 3827 50  0000 C CNN
F 2 "" H 1450 4000 50  0001 C CNN
F 3 "" H 1450 4000 50  0001 C CNN
	1    1450 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 4000 1450 3850
Connection ~ 1450 3850
Wire Wire Line
	1450 3850 1650 3850
Wire Wire Line
	1450 3400 1450 3250
Connection ~ 1450 3250
Wire Wire Line
	1450 3250 1600 3250
$Comp
L power:GND #PWR?
U 1 1 5C477A5E
P 3550 5250
F 0 "#PWR?" H 3550 5000 50  0001 C CNN
F 1 "GND" H 3555 5077 50  0000 C CNN
F 2 "" H 3550 5250 50  0001 C CNN
F 3 "" H 3550 5250 50  0001 C CNN
	1    3550 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 5250 3550 5150
$Comp
L power:GND #PWR?
U 1 1 5C47F6A5
P 7300 5150
F 0 "#PWR?" H 7300 4900 50  0001 C CNN
F 1 "GND" H 7305 4977 50  0000 C CNN
F 2 "" H 7300 5150 50  0001 C CNN
F 3 "" H 7300 5150 50  0001 C CNN
	1    7300 5150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5C48037F
P 7300 3050
F 0 "#PWR?" H 7300 2800 50  0001 C CNN
F 1 "GND" H 7305 2877 50  0000 C CNN
F 2 "" H 7300 3050 50  0001 C CNN
F 3 "" H 7300 3050 50  0001 C CNN
	1    7300 3050
	1    0    0    -1  
$EndComp
NoConn ~ 3650 2600
NoConn ~ 3650 2800
NoConn ~ 3650 3000
NoConn ~ 3650 3100
NoConn ~ 3650 3200
NoConn ~ 3650 3300
NoConn ~ 3650 3400
NoConn ~ 3650 3500
NoConn ~ 3650 3600
NoConn ~ 3650 3700
NoConn ~ 2450 3500
NoConn ~ 2450 3400
NoConn ~ 2450 3300
NoConn ~ 2450 3200
NoConn ~ 2450 3100
NoConn ~ 2450 3000
NoConn ~ 2450 2700
NoConn ~ 2450 2600
NoConn ~ 4600 4250
NoConn ~ 4600 4150
NoConn ~ 4600 4050
NoConn ~ 4600 3950
NoConn ~ 4600 3850
NoConn ~ 4600 3750
NoConn ~ 4600 3550
NoConn ~ 4600 3150
NoConn ~ 4600 3050
NoConn ~ 4600 2950
NoConn ~ 4600 2850
NoConn ~ 4600 2750
NoConn ~ 4600 2650
NoConn ~ 4600 2550
NoConn ~ 4600 2450
NoConn ~ 4600 2350
NoConn ~ 4600 2250
NoConn ~ 4600 2150
NoConn ~ 4600 2050
NoConn ~ 4600 1950
NoConn ~ 4600 1850
NoConn ~ 4600 1750
NoConn ~ 4600 1650
NoConn ~ 4600 1550
NoConn ~ 4600 1450
NoConn ~ 4600 1350
NoConn ~ 5800 2950
NoConn ~ 5800 3050
NoConn ~ 5800 3150
NoConn ~ 5800 3250
NoConn ~ 5800 3350
NoConn ~ 5800 3450
NoConn ~ 5800 3550
NoConn ~ 5800 3650
NoConn ~ 5800 3750
NoConn ~ 5800 3850
NoConn ~ 5800 3950
NoConn ~ 5800 4050
NoConn ~ 5800 4150
NoConn ~ 5800 4250
NoConn ~ 8500 2950
NoConn ~ 8500 5100
$EndSCHEMATC
