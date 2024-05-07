# Introduction

This example code is meant to be an example for your first hardware lab.
If your connections are all correct, your LCD will work correctly.

# Compilation

Unlike previous labs, this code base is broken up into multiple files
For compilation and uploading to device use:
	"sdcc -c lcdDriver.c"
	"sdcc ecen433Example.c lcdDriver.rel"
	"packihx ecen433Example.ihx > ecen433Example.hex"

# Notes

This driver can be used for a stand-alone keypad and LCD driver or
with multiple peripherals and address bus decoding. By default,
The LCD address is 0x4000. If nothing else is connected, we'll
be taking advantage of the data always being on P0. 

# Disclaimer

I took my time to completely rewrite this library to improve on
speed and show the memory management concepts in practice. All 
students of ECEN-433, feel free to utilize and modify this code 
for your projects. Enjoy!

Tested on:
	AT89C55WD
	AT89C51RC 
