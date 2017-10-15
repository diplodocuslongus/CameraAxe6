EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:CA6
LIBS:Vibration-cache
EELAYER 25 0
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
L JACK_RJ45 J1
U 1 1 59C40B7A
P 1500 1400
F 0 "J1" H 1675 1900 39  0000 C CNN
F 1 "JACK_RJ45" H 1325 1900 39  0001 C CNN
F 2 "CA6:RJ45_pcbwing" H 1475 1400 60  0001 C CNN
F 3 "" H 1475 1400 60  0000 C CNN
	1    1500 1400
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR01
U 1 1 59C40C28
P 2150 1100
F 0 "#PWR01" H 2150 850 50  0001 C CNN
F 1 "GND" H 2150 950 50  0000 C CNN
F 2 "" H 2150 1100 50  0000 C CNN
F 3 "" H 2150 1100 50  0000 C CNN
	1    2150 1100
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 59C40D7E
P 2450 1400
F 0 "R1" V 2530 1400 50  0000 C CNN
F 1 "1M" V 2450 1400 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2380 1400 50  0001 C CNN
F 3 "" H 2450 1400 50  0000 C CNN
	1    2450 1400
	-1   0    0    1   
$EndComp
$Comp
L D_Schottky D1
U 1 1 59C40E4B
P 2800 1400
F 0 "D1" H 2800 1500 50  0000 C CNN
F 1 "D_Schottky" H 2800 1300 50  0001 C CNN
F 2 "Diodes_SMD:D_SOT-23_ANK" H 2800 1400 50  0001 C CNN
F 3 "" H 2800 1400 50  0000 C CNN
	1    2800 1400
	0    -1   -1   0   
$EndComp
$Comp
L CONN_01X01 P1
U 1 1 59C4105F
P 3200 1650
F 0 "P1" H 3200 1750 50  0000 C CNN
F 1 "C+" V 3300 1650 50  0000 C CNN
F 2 "Wire_Pads:SolderWirePad_single_SMD_5x10mm" H 3200 1650 50  0001 C CNN
F 3 "" H 3200 1650 50  0000 C CNN
	1    3200 1650
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X01 P2
U 1 1 59C410A1
P 3200 1050
F 0 "P2" H 3200 1150 50  0000 C CNN
F 1 "C-" V 3300 1050 50  0000 C CNN
F 2 "Wire_Pads:SolderWirePad_single_SMD_5x10mm" H 3200 1050 50  0001 C CNN
F 3 "" H 3200 1050 50  0000 C CNN
	1    3200 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 1050 3000 1050
Wire Wire Line
	2150 1100 2150 1050
Connection ~ 2150 1050
Wire Wire Line
	2450 1250 2450 1050
Connection ~ 2450 1050
Wire Wire Line
	2800 1250 2800 1050
Connection ~ 2800 1050
Wire Wire Line
	1950 1650 3000 1650
Wire Wire Line
	2800 1550 2800 1650
Connection ~ 2800 1650
Wire Wire Line
	2450 1550 2450 1650
Connection ~ 2450 1650
NoConn ~ 1950 1150
NoConn ~ 1950 1250
NoConn ~ 1950 1350
NoConn ~ 1950 1450
NoConn ~ 1950 1550
NoConn ~ 1950 1750
$EndSCHEMATC
