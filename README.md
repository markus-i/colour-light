# colour-light
a basic (and, necessarily, adaptable) colour LED light for various thingiverse prints
running on Arduino (tested for Nano)

uses the FastLed, ClickEncoder and TimerOne libraries

In the current version, 4 strings of 3 LEDs (WS8212B / Nanopixel type) are used. Adapt to whatever 
LED setup you want to use.
Also, in this version, to simplify the UI, only brightness (=value) and colour (=hue) are changeable. 
Further modes for switching saturation (pastel colours) and permanent colour change (=disco) are
foreseen in the source code, but not reacheable.

Single click = on/off
double click = switch between brightness and colour
long press = reset colour to white

This version uses the pins A4, A5, D2 and D3 for the LEDs, A2, A1 and A0 for the incremental encoder
