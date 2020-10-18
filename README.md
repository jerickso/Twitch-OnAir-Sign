# OnAir Sign

Would you like to be notified when your favorite Twitch streamer is online?  Or maybe you are a streamer yourself and would like others in the building to know when you are live and streaming?

This project contains Arduino code that illuminates an "On Air" sign red when the designated Twitch Streamer is live and is a dim white when the streamer is not live.

A lot of the inspiration, hardware suggestions, and code came from the following Adafruit post:
[3D Printed IoT On Air Sign for Twitch](https://learn.adafruit.com/3d-printed-iot-on-air-sign-for-twitch "3D Printed IoT On Air Sign for Twitch")
Note that Twitch's API has changed since the Adafruit post was created, so Adafruit's code no longer works as presented.

This design uses a premade acrillic LED sign that I purchased off EBay.  There are a few different graphics so you could choose one that fits your style.

I replaced the LED strip that came with the signwith a skinny NeoPixel RGB LED Strip

The code is developed around the Adafruit Feather Huzzah (ESP8266 w/WiFi) since that is what the original Adafruit design used, tho any similar development board should work.


## Hardware
* [EBay On Air Sign](http://www.ebay.com/sch/?_nkw=LED%20On%20Air%20Sign "On Air Signs at EBay")
* [Adafruit Feather HUZZAH w/ESP8266](https://www.adafruit.com/product/2821 "Adafruit Feather HUZZAH w/ESP8266")
* [Adafruit Mini Skinny NeoPixel Digital RGB LED Strip (1 meter)](https://www.adafruit.com/product/2821 "Adafruit Mini Skinny NeoPixel Digital RGB LED Strip - 1 meter") - Only need 1 foot of the LED strip
* [3-pin JST SM Plug + Receptacle](https://www.adafruit.com/product/1663 "3-pin JST SM Plug + Receptacle") - For easy separation of the ESP and sign
* [5V 2.5A Switching Power Supply w/MicroUSB](https://www.adafruit.com/product/1995 "5V 2.5A Switching Power Supply w/MicroUSB")


With the On Air sign, I replaced the LED that shipped with the sign with a 1 foot section of the NeoPixel LED strip.  I then connected up the JST Plug/Receptacle to the LEDs so I could easly separate the microcontroller from the sign.

## How to use code
Since we are using the NeoPixel LEDs, the Adafruit NeoPixel Library needs to be included into the Sketch.

OnAir.ino
REFRESH - How many seconds to waits before checking again if user is streaming.
LED - Arduino Pin the NeoPixel LED is powered from
PIN - Arduino Pin that is connected to the communication wire of the NeoPixels

arduino_secrets.h - WiFi network and passphrase, the user
    SECRET_SSID - Wireless SSID
    SECRET_PASS - Wireless passphrase

    CLIENT_ID - Obtained from Twitch's Developers Console
    SECRET - Obtained from Twitch's Developers Console

### How to obtain information from Twitch for CLIENT_ID and SECRET
(Information accurate as of 8/1/2020)
Go to the [Twitch Development Console](https://dev.twitch.tv/console, "Twitch Development Console") and click 'Register Your Application' on the right hand side.  Fill out the information as follows

Name: Something so you'll know what it is for, such as OnAirSign
OAuth Redirect URLs:  http://localhost
Category: "Application Integration"
ClientID (Set this value as CLIENT_ID in arduino_secrets.h)
Client Secret (Set this value as SECRET in arduino_secrets.h)

### Default Sign Color States
Below are what the default color states mean for the sign
* Blue - We are disconnected from the wireless network
* Green/Blue - We are connected to the wireless network
* Dim White - No live stream detected
* Red - Streamer is live
