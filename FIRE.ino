
#include "DHT.h"
#define DHTPIN D1 
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);


#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Wire.h>

const char* ssid = "decastro2";
const char* password = "1234567aa";

const char* serverName = "https://nauticaloptionslms.com/fire/add_data2.php";

int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str;
int pm;



void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
   WiFi.begin(ssid, password);

  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  dht.begin();
}

void loop() {
  delay(2000);
  float gas = analogRead(A0);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

String gas2=String(h/1023*100);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));


String tempe;
tempe = String(t);


String humi;
humi = String(h);
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    // Ignore SSL certificate validation
    client->setInsecure();
    //create an HTTPClient instance
    HTTPClient https;
    // Your Domain name with URL path or IP address with path
    https.begin(*client, serverName);
    // Specify content-type header
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepare your HTTP POST request data
    String httpRequestData = "&temp=" + tempe+ "&humi=" + humi+ "&gas=" + gas2+ "&device=" + 3;
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = https.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = https.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = https.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
        delay(2000);
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    https.end();



 




    
  }
  else {
    Serial.println("WiFi Disconnected");
  }


delay(100);


  
}
