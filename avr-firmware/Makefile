# Name: Makefile
# Author: Dennis Gentry
# Copyright 2010, Dennis Gentry
# License: Creative Commons Attribution

# As of early 2013, the attiny85 is more capable, available, and maybe even
# cheaper, but the code works fine on either.
DEVICE     = attiny13

# For lower power applications, you could turn this clock down (using
# the fuses below, but reflecting your choice here.)
CLOCK      = 9600000UL

#PROGRAMMER = -F -c stk500hvsp -P /dev/cu.usbserial-FTCTYHO4
PROGRAMMER = -c usbtiny
OBJECTS    = main.o

# Fuse settings:
# Low:internal 9.6 Mhz clock, no divide.  Startup Time 14 clocks + 64ms
# SPI Enable, No Watchdog Timer Always On
#
# High:  Brownout detection 4.3 v,
FUSES      = -U lfuse:w:0x7a:m -U hfuse:w:0xf9:m


# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS) -lm

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
