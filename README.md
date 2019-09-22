https://www.thingiverse.com/thing:3875189
https://youtu.be/RDJB3YbSjZ4
A remix of the Rotary Tumbler by Colleone. (https://www.thingiverse.com/thing:3101842)
These are the changes:
1. A 3D printed base instead of using the wood plate.
2. Longer and higher box for the motor and electronics.
3. An Arduino based digital motor controller that supports settings the motor speed and the duration.
4. The 3D parts are cut to fit smaller printer print-volume.

Parts:
Follow the original part list for building the tumbler. 
For the digital controller:
Resistors:
2X470 ohm
1K ohm
330 ohm
100 uf capacitor
Arduino pro micro (You can use any 5V Arduino)
7805 voltage regulator
LM317T voltage regulator
LM741 Op-Amp
1.5 A Fuse and Fuse House
4 Digit 7-segment display
A Push-button
A simple relay
A prototype board
Wires
Connectors

Follow the Fritzing diagram. Pay attention to the inputs and outputs of the LM317 and the 7805, they might be different than the Fritzing diagram.  
You may want to change the speed factor in the Arduino code if you have a different motor than mine. 
