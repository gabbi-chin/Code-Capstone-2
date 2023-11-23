

unsigned long previousMillis = 0;
const long interval = 3000;
char inChar;
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Wire.h>

const char* ssid = "HUAWEI-2.4G-x6nM";
const char* password = "1234567a";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "https://nauticaloptionslms.com/fire/bell.php";

int relayInput = 2; // the input to the relay pin


char array2[] = "0,71,20,996.40,15";

char *strings[6]; // an array of pointers to the pieces of the above array after strtok()
char *ptr = NULL;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
pinMode(relayInput, OUTPUT); // initialize pin as OUTPUT
 WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

  
  // this code runs every 2 seconds and display data on the Serial Monitor
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;


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
    String httpRequestData = "&uleft=" + 1;
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);



    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = https.POST(httpRequestData);
      Serial.println("Values uploaded successfully."); Serial.println(httpResponseCode); 
String webpage = https.getString();    // Get html webpage output and store it in a string
Serial.println(webpage + "\n"); 


String str = webpage; 

// Length (with one extra character for the null terminator)
int str_len = str.length() + 1; 

// Prepare the character array (the buffer) 
char char_array[str_len];

// Copy it over 
str.toCharArray(char_array, str_len);





   byte index = 0;
   ptr = strtok(char_array, ",");  // delimiter
   while (ptr != NULL)
   {
      strings[index] = ptr;
      index++;
      ptr = strtok(NULL, ",");
   }
   //Serial.println(index);
   // print all the parts
   Serial.println("The Pieces separated by strtok()");
   for (int n = 0; n < index; n++)
   {
      Serial.print(n);
      Serial.print("  ");
      Serial.println(strings[n]);
   }
int ubl=atoi(strings[0]);

if(ubl>=1){

   digitalWrite(relayInput, HIGH); // turn relay on

}else{
  

  digitalWrite(relayInput, LOW); // turn relay off

 
}



    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = https.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = https.POST("{\"value1\"😕"19\",\"value2\"😕"67\",\"value3\"😕"78\"}");
        delay(1000);
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
  }
}
  
