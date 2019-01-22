/*
Copyright (c) 2019 Stephen Ludgate. All right reserved.

FortniteAPI - An Arduino wrapper for the unofficial Fortnite API

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/


#ifndef FortniteAPI_h
#define FortniteAPI_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Client.h>

#define FTNAPI_HOST "fortnite-public-api.theapinetwork.com"
#define USER_AGENT "FortniteAPI/1.0.1 (Arduino)"
#define FTNAPI_SSL_PORT 443
#define FTNAPI_TIMEOUT 8000


struct serverStatusInfo{
  String statusFlag;	
  String statusMessage;	
  String statusVersion;	
  long timeUpSince;
  long timeUpDurationSeconds;
  String timeUpDurationFormatted;
};

struct playerStatistics{
  String username;
  String platform;
  String window;
  long totalKills;
  long totalWins;
  long totalMatchesplayed;
  long totalMinutesplayed;
  long totalHoursplayed;
  long totalScore;
  float totalWinrate;
  float totalKD;
  long totalLastupdate;
  long timestamp;
};

class FortniteAPI
{
  public:
    FortniteAPI (Client &client);
    String sendGetToFortnite(String command);
    bool getFortniteServerStatus();
    bool getPlayerStatistics(String playerID,String platformID,String windowID);
    
    serverStatusInfo serverStatus;
    playerStatistics playerStats;

    bool _debug = false;
    int httpStatusCode = 0;
    String httpErrorBody = "";

  private:
    Client *_client;
    const int maxMessageLength = 2000;
    void closeClient();
    int extractHttpCode(String statusCode,String body);
};

#endif
