* the PWM output from the atTiny is at 5 volts, so the totem pole
  output is only driven between 0 and 5v instead of the full voltage
  input range

* I haven't proved that the internal pullup resistor is sufficient for
  making a voltage divider with my thermistor, so we might need to add
  an external resistor.
