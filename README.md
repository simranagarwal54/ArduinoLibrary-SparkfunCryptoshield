# Authentication-of-ECC108-with-Arduino-Uno-
Hardware used: Sparkfun CryptoShield and Arduino Uno

This Arduino Library has been taken and implemented from https://github.com/cryptotronix/cryptoauth-arduino.

Few Changes have been made and this library is the edited version and contains implementation of digital signing and verification(Basic Authentication) of a user input using a simple menu based interface.
Chip used to successfully check Authentication: AtEcc108

-User Input
-Sha256 Hash Calculator
-Signature Creation using Key Slot of your choice
-Verification using key Slot Of your choice
-You can send two messages and private-public key pairs to verify if the code works correctly.

*To Run the code*,
Simply download this library and run the FINAL_ code in the example folder.

The RTC_GGWP displays the real time. Download the RTC library from the link given below run the RTC_GGWP in the example folder.  

Make Sure to personalize your device. The steps on how to personalize your device is given on the link above.

## Important Links

Sparkfun Hookup Guide
https://learn.sparkfun.com/tutorials/crypto-shield-hookup-guide

Original library and source codes
https://github.com/cryptotronix/cryptoauth-arduino

ATSHA204 datasheet
http://www.atmel.com/Images/Atmel-8740-CryptoAuth-ATSHA204-Datasheet.pdf

AtAES132 datasheet
http://www.atmel.com/Images/Atmel-8914-CryptoAuth-ATAES132A-Datasheet.pdf

ATECC108 datasheet
http://www.atmel.com/Images/Atmel-8873S-CryptoAuth-ATECC108-Datasheet-Summary.pdf

Library and source codes for RTC 
https://github.com/adafruit/RTClib



