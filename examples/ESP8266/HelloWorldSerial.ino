/*******************************************************************
 *  A "Hello World" sketch to make sure your Arduino (ESP8266) and the
 *  Fortniteapi server can speak to each other with a valid user stats check.
 * 
 *  By Stephen Ludgate https://www.youtube.com/channel/UCVEEuAouZ6ua4oetLjjHAuw
 *******************************************************************/


#include <FortniteAPI.h> //This is where the magic happens... shazam!

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <Time.h>         //We will need these two just to do some rough time math on the timestamps we get to make it pretty
#include <TimeLib.h>

const char* ssid = "SSID";          // your network SSID (name)
const char* password = "PASSWORD";         // your network password

String user_id_ninja = "4735ce9132924caf8a5b17789b40f79c";  //UID (user ID) of Ninja - https://fortnite-public-api.theapinetwork.com/prod09/users/id?username=Ninja
String user_platform = "pc";  //games console platform - available are: pc, xb1, ps4
String user_stats_window = "alltime";  //timeframe window to get stats from - available are: season4,season5,season6,season7,current,alltime

//WiFiClientSecure client;
BearSSL::WiFiClientSecure client; //Seem to need this for this connection and ESP8266 core 2.4.2 and above??
FortniteAPI api(client);

unsigned long api_mtbs = 60000; //mean time between api requests (60 seconds)
unsigned long api_lasttime = 0;   //last time api request has been done

void setup () {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //if you get here you have connected to the WiFi
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setInsecure(); //Related to BearSSL(), otherwise we would need to give SSL certificate credentials. API is public with public data, no security issues here.
}


void loop() {
  if (millis() - api_lasttime > api_mtbs || api_lasttime==0) {  //Check if time has expired to go check Fortnite
    if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

      BearSSL::WiFiClientSecure client; //Seem to have to create client on each connection loop
      client.setInsecure();             //See above about this
      FortniteAPI api(client);
    
      long api_timestamp = 0;
      if(api.getPlayerStatistics(user_id_ninja,user_platform,user_stats_window)){
        Serial.print("------Player Stats (Totals) ");
        Serial.print(api.playerStats.username);
        Serial.print("/");
        Serial.print(api.playerStats.platform);
        Serial.print(" (");
        Serial.print(api.playerStats.window);
        Serial.println(")------");
        Serial.print("Kills:\t\t");
        Serial.println(api.playerStats.totalKills);
        Serial.print("Wins:\t\t");
        Serial.println(api.playerStats.totalWins);
        Serial.print("matchesplayed:\t");
        Serial.println(api.playerStats.totalMatchesplayed);
        Serial.print("minutesplayed:\t");
        Serial.println(api.playerStats.totalMinutesplayed);
        Serial.print("hoursplayed:\t");
        Serial.println(api.playerStats.totalHoursplayed);
        Serial.print("score:\t\t");
        Serial.println(api.playerStats.totalScore);
        Serial.print("winrate:\t");
        Serial.print(api.playerStats.totalWinrate);
        Serial.println("%");
        Serial.print("kd:\t\t");
        Serial.println(api.playerStats.totalKD);
        Serial.print("lastupdate:\t");
        Serial.println(api.playerStats.totalLastupdate);
        Serial.println("------------------------");
        Serial.println();
        
        api_timestamp = api.playerStats.timestamp;
        //API Sync time in human readable date and time YYYY-MM-DD HH:MM:SS
        char bufDate[31];
        sprintf(bufDate,"API Sync:\t%04d-%02d-%02d %02d:%02d:%02d",
        year(api_timestamp),
        month(api_timestamp),
        day(api_timestamp),
        hour(api_timestamp),
        minute(api_timestamp),
        second(api_timestamp)
        );
        Serial.println("------Example Use-----");
        Serial.println(bufDate);

        long progressPrintTimeLeft = api.playerStats.totalMinutesplayed*60;
        int runDays = progressPrintTimeLeft/86400;
        int hoursRemaining=progressPrintTimeLeft%86400;
        int runHours= hoursRemaining/3600;
        int secsRemaining=progressPrintTimeLeft%3600;
        int runMinutes=secsRemaining/60;
        char buf[31];
        sprintf(buf,"Time played:\tDays:\t%02d\n\t\tHours:\t%02d\n\t\tMinutes:%02d",runDays,runHours,runMinutes);
        Serial.println(buf);
        Serial.println("------------------------");
        Serial.println();
      }
    }
    api_lasttime = millis();  //Set api_lasttime to current milliseconds run
  }
}
