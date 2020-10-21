# OnAir Sign

Would you like to be notified when your favorite Twitch streamer is online?  Or are you a streamer yourself and would like others in the vicinity to know when you are live and streaming so they (hopefully) do not interrupt you?

This project contains Arduino code that illuminates an "On Air" sign red when the designated Twitch streamer is live and is a dim white when the streamer is not live.

A lot of the inspiration, hardware suggestions, and code came from the following Adafruit post:
[3D Printed IoT On Air Sign for Twitch](https://learn.adafruit.com/3d-printed-iot-on-air-sign-for-twitch "3D Printed IoT On Air Sign for Twitch")
Note that Twitch's API has changed since the Adafruit post was created and Adafruit's code no longer works as presented.

This design uses a premade acrillic LED sign that I purchased off EBay.  There are a few different graphic designs that one can choose from to fit your style or go a totally different route.

I replaced the LED strip that came with the acrillic sign with a skinny NeoPixel RGB LED Strip to give the flexibility of lighting up as different colors to signify different states (ie, off air, on air).

The code is developed around the Adafruit Feather Huzzah (ESP8266 w/WiFi) since that is what the original Adafruit design used, tho any similar development board should work.  A limiting factor with this board is the sparce onboard RAM (64 KiB of instruction RAM, 96 KiB of data RAM), limiting the ability to add addional libraries such as a JSON decoder.


## Hardware
* [EBay On Air Sign](http://www.ebay.com/sch/?_nkw=LED%20On%20Air%20Sign "On Air Signs at EBay")
* [Adafruit Feather HUZZAH w/ESP8266](https://www.adafruit.com/product/2821 "Adafruit Feather HUZZAH w/ESP8266")
* [Adafruit Mini Skinny NeoPixel Digital RGB LED Strip (1 meter)](https://www.adafruit.com/product/2821 "Adafruit Mini Skinny NeoPixel Digital RGB LED Strip - 1 meter") - Only need 1 foot of the LED strip
* [3-pin JST SM Plug + Receptacle](https://www.adafruit.com/product/1663 "3-pin JST SM Plug + Receptacle") - For easy separation of the ESP and sign
* [5V 2.5A Switching Power Supply w/MicroUSB](https://www.adafruit.com/product/1995 "5V 2.5A Switching Power Supply w/MicroUSB")


With the On Air sign, I replaced the LED that shipped with the sign with a 1 foot section of the NeoPixel LED strip.  I then connected up the JST Plug/Receptacle to the LEDs so I could easly separate the microcontroller from the sign.

## How to use code
Since we are using the NeoPixel LEDs, the Adafruit NeoPixel Library needs to be included into the Sketch.  A quick rundown of the files...

OnAir.ino - The main Arduino code  Interesting variables:
* REFRESH - How many seconds to waits before checking again if user is streaming.
* LED - Arduino Pin the NeoPixel LED is powered from
* PIN - Arduino Pin that is connected to the communication wire of the NeoPixels

arduino_secrets.h - This file has configuration specific settings for your personal sign. Things such as the WiFi network and passphrase to connect to, your Twitch developer client_id and secret (unique to your account), and the twitch user to watch (can be any twitch user).
* SECRET_SSID - Wireless SSID of network to connect to
* SECRET_PASS - Wireless passphrase of network to connect to
* CLIENT_ID - String obtained from Twitch's Developers Console
* SECRET - String obtained from Twitch's Developers Console

### How to obtain the CLIENT_ID and SECRET from Twitch
(Information accurate as of 8/1/2020)
Go to the [Twitch Development Console](https://dev.twitch.tv/console, "Twitch Development Console") and click 'Register Your Application' on the right hand side.  Fill out the information as follows:
* Name: Discription for your use, something so you'll know what it is for, such as OnAirSign
* OAuth Redirect URLs:  http://localhost
* Category: "Application Integration"
* ClientID (Copy this value and set it as CLIENT_ID in arduino_secrets.h)
* Client Secret (Copy this value and set it as SECRET in arduino_secrets.h)

### Default Sign Color States
Below are the default color states for the sign:
* Blue - We are disconnected from the wireless network, usually only at startup
* Green/Blue - We are connected to the wireless network but have not yet checked if the stream is live or not
* Dim White - No live stream detected
* Red - Streamer is live
