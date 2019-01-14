/*
   Copyright (c) 2017 Stephen Ludgate. All right reserved.

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


#include "FortniteAPI.h"

FortniteAPI::FortniteAPI(Client &client)	{
	_client = &client;
}

String FortniteAPI::sendGetToFortnite(String command) {
	if (_debug) Serial.println("FortniteAPI::sendGetToFortnite() CALLED");

	String statusCode="";
	String headers="";
	String body="";
	 bool finishedStatusCode = false;
	bool finishedHeaders = false;
	bool currentLineIsBlank = true;
	unsigned long now;
	bool avail;

	bool connected;
	// Connect with fortniteapi api over ssl
	
	if (_client->connect(FTNAPI_HOST, FTNAPI_SSL_PORT)) {
		if (_debug) Serial.println(".... connected to server");

		char useragent[64];
    	snprintf(useragent, 64, "User-Agent: %s",USER_AGENT);

		String a="";
		char c;
		int ch_count=0;

// String url = "/prod09/users/public/br_stats?";
  //user_id=4735ce9132924caf8a5b17789b40f79c&platform=pc";

   

//   _client.print(String("GET ") + url +
//                 "user_id=" + USER_ID + 
//                 "&platform=pc" +
//                 "&window=current" +
//                 " HTTP/1.1\r\n" +
//                "Host: " + host + "\r\n" +
//                "X-Fortnite-API-Version: v1.1\r\n" +
//                "User-Agent: BuildFailureDetectorESP8266\r\n" +
//                "Connection: close\r\n\r\n");
_client->print(String("GET ") + command +
                " HTTP/1.1\r\n" +
               "Host: " + FTNAPI_HOST + "\r\n" +
               "X-Fortnite-API-Version: v1.1\r\n" +
               useragent + "\r\n" +
               "Connection: close\r\n\r\n");


		// _client->println("GET "+command);
		now=millis();
		avail=false;
		while (millis() - now < FTNAPI_TIMEOUT) {
			while (_client->available()) {

				// Allow body to be parsed before finishing
				avail = finishedHeaders;
				char c = _client->read();
				if (_debug) Serial.print(c);

				if(!finishedStatusCode){
					if(c == '\n'){
						finishedStatusCode = true;
					} else {
						statusCode = statusCode + c;
					}
				}

				if(!finishedHeaders){
					if (currentLineIsBlank && c == '\n') {
						finishedHeaders = true;
					}
					else{
						headers = headers + c;

					}
				} else {
					if (ch_count < maxMessageLength)  {
						body=body+c;
						ch_count++;
					}
				}

				if (c == '\n') {
					currentLineIsBlank = true;
				}else if (c != '\r') {
					currentLineIsBlank = false;
				}
			}
			if (avail) {
				//Serial.println("Body:");
				//Serial.println(body);
				//Serial.println("END");
				break;
			}
		}
		// _client->stop();
	}
	else{
		if (_debug){
			Serial.println(".... connection failed");
			Serial.println(connected);	
		}
	}
	closeClient();

	int httpCode = extractHttpCode(statusCode,body);
	if (_debug){
		Serial.print("\nhttpCode:");
		Serial.println(httpCode);
	}
	httpStatusCode = httpCode;

	return body;
}

bool FortniteAPI::getPlayerStatistics(String playerID,String platformID,String windowID){
	String command="/prod09/users/public/br_stats?user_id="+playerID+"&platform="+platformID+"&window="+windowID;
	// Serial.println(command);
	String response = sendGetToFortnite(command);       //recieve reply from fortniteapi
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(response);
	if(root.success()) {
		playerStats.timestamp = 0;
		if (root.containsKey("timestamp")) {
			long timestamp = root["timestamp"];			
			playerStats.timestamp = timestamp;
		}
		String username = root["username"];	
		String platform = root["platform"];	
		String window = root["window"];	
		playerStats.username = username;
		playerStats.platform = platform;
		playerStats.window = window;
		if (root.containsKey("totals")) {
			long totalKills = root["totals"]["kills"];
			long totalWins = root["totals"]["wins"];
			long totalMatchesplayed = root["totals"]["matchesplayed"];
			long totalMinutesplayed = root["totals"]["minutesplayed"];
			long totalHoursplayed = root["totals"]["hoursplayed"];
			long totalScore = root["totals"]["score"];
			float totalWinrate = root["totals"]["winrate"];
			float totalKD = root["totals"]["kd"];
			long totalLastupdate = root["totals"]["lastupdate"];
			
			playerStats.totalKills = totalKills;
			playerStats.totalWins = totalWins;
			playerStats.totalMatchesplayed = totalMatchesplayed;
			playerStats.totalMinutesplayed = totalMinutesplayed;
			playerStats.totalHoursplayed = totalHoursplayed;
			playerStats.totalScore = totalScore;
			playerStats.totalWinrate = totalWinrate;
			playerStats.totalKD = totalKD;
			playerStats.totalLastupdate = totalLastupdate;
			return true;
		}
	}
	return false;
}


/***** GENERAL FUNCTIONS *****/

/**
 * Close the client
 * */
void FortniteAPI::closeClient() {
  // if(_client->connected()){    //1.1.4 - Seems to crash/halt ESP32 if 502 Bad Gateway server error
    _client->stop();
  // }
}

/**
 * Extract the HTTP header response code. Used for error reporting - will print in serial monitor any non 200 response codes (i.e. if something has gone wrong!).
 * */
int FortniteAPI::extractHttpCode(String statusCode, String body) {
  if(_debug){
    Serial.print("\nStatus code to extract: ");
    Serial.println(statusCode);
  }
  int firstSpace = statusCode.indexOf(" ");
  int lastSpace = statusCode.lastIndexOf(" ");
  if(firstSpace > -1 && lastSpace > -1 && firstSpace != lastSpace){
    String statusCodeALL = statusCode.substring(firstSpace + 1);                //"400 BAD REQUEST"
    String statusCodeExtract = statusCode.substring(firstSpace + 1, lastSpace); //May end up being e.g. "400 BAD"
    int statusCodeInt = statusCodeExtract.toInt();                              //Converts to "400" integer - i.e. strips out rest of text characters "fix"
    if(statusCodeInt != 200
    and statusCodeInt != 201
    and statusCodeInt != 202
    and statusCodeInt != 204){
      Serial.print("\nSERVER RESPONSE CODE: " + String(statusCodeALL));
      if(body!="") Serial.println(" - " + body);
      else Serial.println();
    }
    return statusCodeInt;
  } else {
    return -1;
  }
}