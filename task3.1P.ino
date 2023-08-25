#include <DHT.h>
#include <WiFiNINA.h>

// put your Wifi name and password 
char ssid[] = "Ridhima";
char pass[] = "Ridhima@123";

// put the API key from account of ThingSpeak
char server[] = "api.thingspeak.com";
String apiKey = "3IUTH6U42FPGHUK";

//DHT11 pins connections 
#define DHTPIN 3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(9600);

  
  while (WiFi.status() != WL_CONNECTED) { // connection to Wi-Fi
    Serial.println("Connecting..");
    WiFi.begin(ssid, pass);
    delay(3000);
  }
 
  Serial.println("Connection with Wifi Done Successfully!!");
  dht.begin();
}

void loop() {
  // this start reading humidity and temp from sensor 
  Serial.print("START FROM HERE");
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read data from  DHT sensor!");
    delay(1000);
    return;
  }
 
  // Information to send to ThingSpeak
  String data = "api_key=" + apiKey;
  data += "&field1=" + String(temperature);
  data += "&field2=" + String(humidity);
  Serial.println(data);

  // HTTP post 
  if (client.connect(server, 80)) {
    client.print("POST /update HTTP/1.1\r\n");
    client.print("Host: " + String(server) + "\r\n");
    client.print("Content-Type: application/x-www-form-urlencoded\r\n");
    client.print("Content-Length: " + String(data.length()) + "\r\n");
    client.print("\r\n");
    client.print(data);
    
    Serial.println("Data has been sent to ThingSpeak!");
  } else {
    Serial.println("Connection Failed!");
  }
delay(30000);
}
