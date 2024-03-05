# boilermon
ESP32 sketch for remotely monitoring hot water boiler

I was frustrated by the cost of the Lochinvar CON-X-US module and installation/setup for a boiler at a remote building, and figured that I only needed to know some basic information about the state of the system rather than get complete electronic control.

The output screen of my boiler tells me certain system temperatures and the pump status. I figured I could get the same details unintrusively, as well as zone-specific information that the boiler doesn't know about.

The setup has limited extensibility because of the GPIO count of the ESP32, but is set up in this repository to monitor three motors (boiler, circulator, and domestic hot water); six temperatures (DHW tank, inlet, outlet/loop, boiler room ambient, intake/outdoor ambient, exhaust); and four zone valves (with a quad optocoupler module). It outputs to wifi and a 16x2 LCD.

## BOM
1x ESP32
3x 10A CT clamps (SCT-013-010)
10x 4.7k resistors
3x 10uf 16A capacitors
1x PC817 quad optocoupler
6x DS18B20 temperature sensor probes and modules
1x 16x2 I2C LCD display
plenty o' wire
project box
optional: ESP32 breakout board
optional: stripboard or prototype board
