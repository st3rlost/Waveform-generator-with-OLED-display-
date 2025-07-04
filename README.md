# Waveform-generator-with-OLED-display-

A waveform generator made using the ESP32 microcontroller, which displays Sine , Square and Triangle waves on an attached OLED screen, which uses the I2C communciation protocol.

Requirements:
-> Software libraries mentioned in the code.
-> Arduino IDE to implement the code.
-> Push button , jumper wires, ESP32, OLED screen (0.96 inch, 128x64), potentiometer, breadboard.

The potentiometer is used to change the frequency of the given wave, when the push button is pressed, the project cycles through three types of waveforms and displays the name, frequency and shape of the waveform on the OLED screen, as shown in the JPG files.
The potentiometer wiring is a bit noisy, hence the jitters in frequency.

This project could be further built upon to connect it to cloud and make a web-based version of it, as well as add on more types of waveforms.

