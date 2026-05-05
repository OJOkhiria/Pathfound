# Pathfound
A custom PCB controller board and efidget that uses a Xiao RP2040 microcontroller and a MAX7219 IC to count your clicks from 0-99 and use them to turn on LEDS. The Xiao microcontroller provides the logic for the LEDs and the MAX7219. Specifically, by assigning the buttons to inputs on the Xiao (pressed is high and unpressed is low), you can assign high and low values to the LEDs, which are designated as outputs. Using the same logic, the controller outputs counting data to the MAX7219, which uses two seven-segment displays: one for the ones digit, and one for the tens digit. There are two buttons that you can use with this counting logic, an increase button that increments your count by 1, and a reset button that sets your count to 0, regardless of what number you're on (might add a decrease button in the future). The project schematics and PCB were designed in KiCad, and the firmware was programmed in C++ using the Arduino IDE.
<img width="1288" height="770" alt="Screenshot 2026-05-03 at 11 28 37 PM" src="https://github.com/user-attachments/assets/d1b6a53b-58de-4adf-8ac7-92c4b40c24d3" />
<img width="996" height="596" alt="Screenshot 2026-05-03 at 11 27 09 PM" src="https://github.com/user-attachments/assets/9b85a183-6f0b-4064-94c5-cb414830dc8c" />

<img width="829" height="515" alt="Screenshot 2026-05-04 at 1 34 44 PM" src="https://github.com/user-attachments/assets/cdce3be5-1e6c-4705-be89-920ce8c0eaea" />

BOM:

Reference	Qty	Value	DNP	Exclude from BOM	Exclude from Board	Footprint	Datasheet
AFF1,AFF2	2	LTS-6980HR				Display_7Segment:7SegmentLED_LTS6760_LTS6780	http://datasheet.octopart.com/LTS-6960HR-Lite-On-datasheet-11803242.pdf
C1	1	0.1uF				Capacitor_THT:CP_Radial_D10.0mm_P2.50mm	
C2	1	10uF				Capacitor_THT:CP_Radial_D5.0mm_P2.00mm	
D1,D2,D3	3	L08R5000Q1				LED:LEDRD254W57D500H1070	
R1,R2,R3	3	470				Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Vertical	
R4	1	10k				Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal	
SW1,SW2,SW3	3	SW_Push				Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB	
U1	1	XIAO-RP2040-DIP				XIAO:XIAO-RP2040-DIP	
U2	1	MAX7219				MAX7219:DIL24	https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf


