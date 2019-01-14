# Fortnite API
<img align="left" style="padding-right:10px;" src="resources/fortnite-logo-black.png">
Library for use with Arduino compatible micro controllers (web enabled) such as ESP8266 and ESP32 to access the simple, yet very powerful (and FREE!) Fortnite API (UNOFFICIAL) - https://fortniteapi.com/. **All rights go to Epic Games**.

### What does it do?
This API is an Arduino wrapper class for the API hosted at https://fortniteapi.com/ which is an unofficial API that scrapes data from the publically available data from the Epic Games API.

This library will make it easy for you to get information from Fortnite, such as player stats, store items, leaderboards and challenges. I leave it up to you what and how you process this data in the most useful or pointless way possible!

All hats got off to @samhoogantink [Sam](https://github.com/samhoogantink) who manages and maintains the API servers and code - this just pulls in that data for use on Arduino.

[![Buy me a beer!](https://img.shields.io/badge/Buy_me_Beer!-PayPal-green.svg)](https://www.paypal.me/chunkysteveo/3.5)

## Getting Started

You will need an Arduino compatible microcontroller that can connect to the internet/your network - ideally an ESP8266, that's it. The API doesn't use (anymore) any authentication method or password, so you don't need to sign up or request a key.

### Test your connection
Check that the Fortnite API is up and running, and polling the Fortnite game servers.

```
https://fortnite-public-api.theapinetwork.com/prod09/status/fortnite_server_status
```

Your browser should give you something like this:

```
{
  "status": "UP",
  "message": "Fortnite is up.",
  "version": "7.10",
  "time": {
    "since": {
      "seconds": "1545132605"
    },
    "duration": {
      "seconds": 2338740,
      "formated": "27 days, 01 hour, 39 minutes and 00 seconds"
    }
  }
}
```


## Installation
Available in the official Arduino Library Manager - Sketch -> Include Library -> Manage Libraries... and then search for "fortniteapi" and click Install!

Also available via GitHub if you clone it or download as a ZIP. The downloaded ZIP code can be included as a new library into the IDE selecting the menu:

     Sketch / include Library / Add .Zip library

You will also have to install the awesome ArduinoJson library written by [Benoît Blanchon](https://github.com/bblanchon). Search **ArduinoJson** on the Arduino Library manager or get it from [here](https://github.com/bblanchon/ArduinoJson).

Include Fortnite API in your project:

    #include <FortniteAPI.h>


## Examples

### HelloWorldSerial
This is the first sketch to try out. Enter your WiFi credentials, compile and upload. Open the serial monitor and you should start to see player stat information coming back from user "Ninja".


## Acknowledgments

* Hat tip to Brian Lough, aka @witnessmenow for his tireless work on his Arduino API libraries which gave me the base to create my own.
* Sam (@samhoogantink) for his amazing work on the API. [https://fortniteapi.com/](https://fortniteapi.com/)

## Authors

* **Stephen Ludgate** - *Initial work* - [chunkysteveo](https://github.com/chunkysteveo)

## License

See the [LICENSE.md](LICENSE.md) file for details


## Release History
* 1.0.0
    * First release, just getting it out there. Bear with me while I fill it out!

## Requests / Future To Do List
* Crikey - lots to do!

## Meta

Stephen Ludgate:

[@Instagram](https://www.instagram.com/chunkysteveo/)

[YouTube](https://www.youtube.com/c/StephenLudgate) 

[Blog](https://www.chunkymedia.co.uk)

Email - info@chunkymedia.co.uk

[https://github.com/chunkysteveo/FortniteAPI](https://github.com/chunkysteveo/FortniteAPI)

## Contributing

1. Fork it (<https://github.com/chunkysteveo/FortniteAPI/fork>)
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request
