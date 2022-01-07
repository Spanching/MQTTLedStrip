# MQTTLedStrip

## Disclaimer

I am glad you are interested in my project. But I realized, if I wanted effects and all the good stuff, this project would take me a lot of work. Fortunately someone else invests his time in exactly that. 
So for me I decided to use [WLED](https://github.com/Aircoookie/WLED) instead of programming my own controller. 

Feel free to browse the code and use whatever you may need, but keep in mind all of this will not be updated anymore.

### MQTT Client
This LED strip is controlled by an App (coming soon on github), that sends http requests to a python server running on my raspberry pi. 
The MQTT Broker is on that raspberry pi too, so this way it publishes the MQTT command needed to perform the action.

### WifiManager
As I don't want my Wifi credentials hardcoded (and the ip for the MQTT Broker, name of my led strip etc.) in the code, I use the 
[WifiManager](https://github.com/tzapu/WiFiManager) library to receive those informations with my Wemos as AP. Then those informations get
stored in the EEPROM of the ESP8266 for the next use. </br>
Can also be used to change the Wifi credentials or other data with no need to connect it to a computer, which is very helpful.
It is used in the [accesspoint](/accesspoint.ino) file.

### LED Controlling
The LED strip itself is a WS2811, which means all LEDs are addressable separately. The Library used is 
[FastLED](https://github.com/FastLED/FastLED) (RIP Dan Garcia).</br>
There is work to do here to make custom effects possible but for now there is the Rainbow effect and
some colors.
You can have a look at the implementation at the [led](/led.ino) file.

### Fauxmo
This is one of my favourite libraries for the ESP8266, it gives you a simple way to interact with Alexa.
The [Fauxmo library](https://github.com/n8henrie/fauxmo) works with all generations of the echo dot.</br>
In this project I use a little workaround as the only information Alexa can send to the Wemos ar brightness
and on/off, I just register multiple devices with different names for the same LED strip, those are used
for different colors.
You can have a look at the file [here](/fauxmo.ino)
