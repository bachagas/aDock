aDock
=====

An Arduino based IoT dock station for Android phones to control some home appliances:
   - a TV through infrared remote
   - a lamp or something to switch on and off through a 110V/220V AC output relay
   - a (hacked) Sennheiser Office 1 Headset ( http://en-us.sennheiser.com/wireless-phone-headset-single-sided-dect-sd-office ) through two optocouplers

aDock was originally developed to be used as an assistive technology device for people with severe physical disabilities, like tetraplegia.

![aDock](https://lh6.googleusercontent.com/-ZKWjFbm4LK8/VEEmTJqalMI/AAAAAAAADxw/OPLhkcUFIHE/w1027-h577-no/IMG_20141017_112033474.jpg)
![aDock 1](https://lh5.googleusercontent.com/p1U49aUQxMMKjyUqoll-KXYxJKiNjKQgQXaAHQ1dS5g=w390-h219-p-no)
![aDock 2](https://lh4.googleusercontent.com/Fauxd9TIfo0pjvG5zhMxCcIeeX88-kqLog4BqgA2DdA=w391-h219-p-no)
![aDock 3](https://lh3.googleusercontent.com/W8GNl8H-xUPrIfo8NvUDYPI14jcjQFZYaPtALP3RTHU=w390-h219-p-no)
![aDock 4](https://lh6.googleusercontent.com/fWBDQ83xDXv43E13XOrkDvRGFU-X3mOkLNQPMSFnPgw=w396-h222-p-no)

Hardware
--------

- 1 x Arduino Board (a Uno will work);
- 1 x [ProtoScrewShield](https://www.sparkfun.com/products/9729);
- 1 x basic 16x2 character LCD display, like the [LCD ROHS 1602B](http://multilogica-shop.com/LCD_16x2_preto_sobre_verde);
- 1 x 12V relay circuit mounted on a PCB-like board like the one described [here](http://www.instructables.com/id/Connecting-a-12V-Relay-to-Arduino/?ALLSTEPS). You can see a simplified schematics and simulation [here](http://www.falstad.com/circuit/#%24+1+5.0E-6+11.086722712598126+43+2.0+50%0Aw+336+288+336+336+1%0Aw+336+256+336+208+1%0AL+128+272+80+272+0+0+true+5.0+0.0%0AO+336+256+400+256+1%0A178+336+112+432+112+0+1+0.2+1.0000262328429621E-13+0.05+1000000.0+0.02+233.0%0Ad+336+160+288+160+1+0.805904783%0Aw+288+144+336+144+0%0Aw+288+160+288+144+0%0Ar+288+272+192+272+0+1000.0%0A181+480+96+480+128+0+398.8232284946172+40.0+120.0+0.4+0.4%0Av+336+48+432+48+0+1+60.0+127.0+0.0+0.0+0.5%0Aw+336+48+336+112+0%0Aw+480+48+480+96+0%0Aw+432+48+480+48+0%0Aw+432+128+480+128+0%0At+288+272+336+272+0+1+-11.999999999926683+-0.11562430661010605+100.0%0Ag+128+368+128+400+0%0A162+128+304+128+368+1+2.1024259+0.0+1.0+0.0%0Ar+128+272+128+304+0+220.0%0Ag+592+320+592+352+0%0Aw+128+272+192+272+0%0A162+528+320+592+320+1+2.1024259+1.0+0.0+0.0%0At+400+336+448+336+0+1+-10.101880446495288+0.11562430665900605+100.0%0AR+288+144+288+96+0+0+40.0+12.0+0.0+0.0+0.5%0Ag+448+352+448+384+0%0AO+448+320+480+288+1%0Ar+448+224+448+272+0+100.0%0Aw+448+272+448+320+0%0Ar+448+320+528+320+0+470.0%0Aw+336+160+336+208+0%0AR+448+224+448+176+0+0+40.0+12.0+0.0+0.0+0.5%0Ar+336+336+400+336+0+22.0%0Ax+548+294+575+300+0+24+off%0Ax+153+342+180+348+0+24+on%0Ax+64+248+89+254+0+24+ctl%0Ao+2+64+0+35+7.62939453125E-5+9.765625E-5+0+-1%0Ao+3+64+0+34+80.0+9.765625E-5+1+-1%0A);
- 1 x ["Max Power IR LED Kit"](https://www.sparkfun.com/products/10732) circuit together with an IR Receiver Diode (e.g. like the [TSOP38238](https://www.sparkfun.com/products/10266) mounted on a single PCB small board. You can see a simplified schematics and simulation [here](http://www.falstad.com/circuit/#%24+1+5.0E-6+10.634267539816555+43+2.0+50%0Aw+400+320+400+368+1%0Aw+400+288+400+240+1%0A172+400+112+400+80+0+6+5.0+5.0+0.0+0.0+0.5+Collector+Voltage%0Ag+400+368+400+384+0%0At+352+304+400+304+0+1+-4.843611783341876+1.649999999978436E-11+100.0%0Aw+288+304+352+304+1%0Ar+400+112+400+176+0+56.0%0AL+192+304+144+304+0+0+true+5.0+0.0%0Ar+288+304+192+304+0+330.0%0A162+400+176+400+240+1+2.1024259+1.0+0.0+0.0%0Ax+92+314+117+320+0+24+ctl%0Ax+423+215+558+221+0+24+infrared+LED%0A) (only the sending part);
- 2 x optocouplers (I used the [Vishay MOC8101](http://www.vishay.com/product?docid=83660) but anything that works with TTL input will work, like the [4N35](http://arduino.cc/documents/datasheets/opto-4n35.pdf) in the Arduino Starter Kit);
- 2 x LEDs (one yellow, one blue, if you like);
- 4 x momentary push-buttons;
- 1 x 10 or 20 KOhms trim-pot;
- 1 x 100 Ohm;
- 2 x 330 Ohm resistors;
- 4 x standard 3.5mm female mono audio jacks to plug to the optocouplers outputs;
- 1 x 12VDC power source (yes, you will need a 12VDC output power source to supply enough voltage for the 12V relay).

Hookup Guide
------------

In a very quick and simplified way, everything should be connected like this:

#### 1) Connect the LCD:
		
		LCD 1602B     Arduino
		---------------------
		 16 (K)   ->  GND
		 15 (A)   ->  5V (if you want the backlight on)
		  1 (VSS) ->  GND
		  2 (VDD) ->  5V
		  3 (V0)  ->  trim-pot's wiper (output) - Note: wire the others pot's terminals to +5V and GND. (Note on note: pots have no polarity.)
		  4 (RS)  ->  10
		  5 (R/W) ->  GND
		  6 (E)   ->  - (not used)
		  7 (DB0) ->  - (not used)
		  8 (DB1) ->  - (not used)
		  9 (DB2) ->  - (not used)
		 10 (DB3) ->  - (not used)
		 11 (DB4) ->  8
		 12 (DB5) ->  7
		 13 (DB6) ->  6
		 14 (DB7) ->  5

That way the LCD should be initiated like this in the code:

	LiquidCrystal lcd(10,  9, 8, 7, 6, 5);

#### 2) Connect the relay circuit:

		Arduino         Relay board
		----------------------------
			VIN     ->  12V
		    GND     ->  GND
		    14 (A0) ->  CTL

#### 3) Connect the infrared circuit:

		Arduino      IR board
		---------------------
			 5V  ->  VCC
		    GND  ->  GND
		      2  ->  RCV (IR sensor output)
			  3  ->  CTL (driving transistor input for the IR LED)
		      4  ->  CTL (just a status LED)

#### 4) Connect the two optocouplers:

Digital pins 11 and 12 go to pin 1 (input +) of each chip through a 330 Ohms resistor each and ground to pin 2 (input -). Then, connect the two 3.5mm female audio jacks to the optocouplers' outputs: wire the tip to pin 5 (output +) and the sleeve to pin 4 (output -);

#### 5) Connect the LEDs:

The blue LED goes to the Arduino A1 pin (digital pin 15) through a 100 Ohms resistor and the yellow to pin 13 through a 330 Ohms resistor (Note: the ProtoScrewShield have this last connection on the shield);

#### 6) Connect the push-buttons:

Wire the 4 push-buttons to Arduino pins A2, A3, A4 and A5 --- they will be used as digital pins 16, 17, 18 and 19, respectively;.
	 
External dependencies
---------------------

You will need the following Arduino libraries:
- [LyquidCrystal](http://arduino.cc/en/Reference/LiquidCrystal): allows an Arduino board to control standard LiquidCrystal displays (LCDs);
- [IRremote](https://github.com/bachagas/Arduino-IRremote): Infrared remote library for Arduino: send and receive infrared signals with multiple protocols.

Installation
------------

Download the archive from GitHub, decompress it, and use it :)
Make sure you have all the libraries above correctly installed in your Arduino development environment before opening the code.
If not, you will need to restart the Arduino IDE after installing the libs.

References
----------

Some parts of the code were based on the following references:
   - http://www.arduino.cc/en/Tutorial/LiquidCrystal: for the LCD (LCM1602B) control;
   - http://forum.arduino.cc/index.php/topic,4104.0.html and http://playground.arduino.cc/uploads/Learning/relays.pdf : for the relay control;
   - https://learn.sparkfun.com/tutorials/ir-communication and http://www.righto.com/2009/08/multi-protocol-infrared-remote-library.html: for the infrared communication control using the awesome [IRremote library](http://github.com/shirriff/Arduino-IRremote) and examples by Ken Shirriff;
   - http://www.martyncurrey.com/?p=34 and http://tronixstuff.com/2010/09/21/education-introduction-to-the-optocoupler/: for the optocouplers part.

For Android communication with Arduino through serial USB take a look at the following links: 
   - http://android-er.blogspot.com.br/2014/02/android-usb-host-mode-step-by-step.html;
   - http://android-er.blogspot.com.br/2014/02/send-hello-to-arduino-from-android-in.html;
   - http://android-er.blogspot.com.br/2014/09/send-data-from-android-to-arduino-uno.html;
   - http://github.com/bachagas/usb-serial-for-android (Android Java library);
   - http://github.com/bachagas/USB-Serial-Module (Titanium library for Android).

Other information
-----------------

Nothing else to say for now ;-)
