# Blink led Example

This example demonstrates how to use GPIO pin 2 (output) and GPIO pin 4 (input with interrupt) to handle LED blinking events.

### Hardware Required

* The example can be run on any development board. The board shall be connected to a computer with a single USB cable for flashing and monitoring. 

* 1 Breadboard

* 1 button

### Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```j
idf.py build
```

```j
idf.py -p COM_number flash
```

```j
idf.py monitor
```

(To exit the serial monitor, type `Ctrl-].)

### Result

* You should see the LED blinking when the button is pressed and an interrupt is triggered.
