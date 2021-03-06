TinyMite PWM
=============

A small, cheap, programmable PWM controller suitable for driving
LEDs or motors.  Based on 8 pin ATTiny uControllers.

What good is it?  Features:
* Compact (1" x 1.25")
* Can drive two paralleled MOSFETs
* Can take a kickback suppression (flywheel) diode for driving motors or solenoids.
* Programmable since it's based on an AtTiny microcontroller (ATTiny 25, 45, 85, and probably 13 all work).
* Has space for a couple of programmable status LEDs
* Capable of input from a switch or potentiometer
* Can monitor a thermistor for high temperature protection

Choose your MOSFET(s), and if you're driving a
motor, your kickback diode, and have at it.

I have built it with various MOSFETS and kickback diodes, but a good
combination seems to be an IRF2804 (42 volt, 270 amp) and On Semi MBR30H150
(30 amp, 150 volt dual Schottky in a TO-220 package), where it happily
controls a 25 amp fan motor in my Crown Victoria.  In that
application, it's on the edge of needing heat sinks.



A note on PWM and component choices
-----------------------------------
Here's what I have learned about MOSFETs and kickback diodes.

IRF2804s cost something like $3.00 these days, so maybe there's
something newer, better and cheaper (the IRFZ44N looks good to me at
$1.18 each for 10 from Newark).  The parameters to pay attention to
are:
* I_d (Continuous Drain current)
* V_ds (Drain Source voltage), and
* R_dson (On Resistance) at the manufacturer's rated test voltage.

I choose I_d to be at least twice the (fuse) rating of the motor, but
with LEDs you could probably get away with something just above the
total load current.  The IRFZ44 is only rated at 49 amps (vs. 270 amps
for the IRF2804), but that should be OK (with a heatsink) for my 25
amp fan motor.

V_ds must be well above the maximum transient voltage spike you ever
expect, since MOSFETs can instantly fail if you exceed that voltage,
even for a moment.  In a car (nominally 13.6 volts), Automotive rated
MOSFETS like the IRF2804 or the IRFZ44N should be fine, and they have
Vgs ratings of 42 and 55 volts, respectively.

R_dson primarily determines one source of power (heat) you'll have to
dissipate from your MOSFET.  Automotive MOSFETs (in particular, the
IRF2804) have amazingly low R_dson, so they can have crazily high I_d
ratings (270 amps continuous), limited only by the TO-220 packaging.
Power is I^2 * R, so 25 amps * 2.3 mOhm = 1.5 watts, which technically
requires a heatsink above ~ 50 degrees C ambient, but my car interior
doesn't stay at 50 degrees C when I'm running the A/C + fan.

The other source of heat is switching losses -- every time the PWM
changes from high to low or the other way around, for a very brief
period (and we make it as brief as possible, in the microseconds
range) some current is flowing through the MOSFET with moderate
resistance.  At lower switching frequencies (say 1 khz to 10 khz), the
several microseconds switching time is negligible compared to the
pulse width (1 millisecond to 100 microseconds).  As the frequencies
get higher (say 20 khz to 100khz giving 50 microsecond to 10
microsecond pulses), the switching time is a significant fraction of
the pulse width, so you have to pay attention to the heat generated
then.  I haven't noticed any whine from my motor or circuit (it's in a
noisy car, after all), so switching at 2 khz doesn't add much in the
way of switching losses, while the motor still "sees" relatively
smooth power.

Datasheets
----------
IRF2804:  http://www.irf.com/product-info/datasheets/data/irf2804.pdf
http://www.newark.com/international-rectifier/irf2804pbf/n-ch-mosfet-40v-270a-to-220ab/dp/63J7214
