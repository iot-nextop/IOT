#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""

WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
  // Connect to WiFi network
  wifiConnect();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  if(Firebase.failed())
  {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  //client.println( "Refresh: 20");        // refresh the page automatically every 20 sec
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.println("<p>Cds value is:<b> ");
  //client.println(FireBase_cds); 
  client.println("</b></p>");
  client.println("<a href=\"/C\"><button>LED ON/OFF </button></a>");
  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

  if (request.indexOf("GET /C")>=0)
  {
      String jsondata = "";
      StaticJsonBuffer<200> jsonbuffer;
      JsonObject& root = jsonbuffer.createObject();
      root["index"]=0;
      root["name"]="hello";
      root.printTo(jsondata);
      Serial.println(jsondata);
      Firebase.push("tests",root);
  }
}

void wifiConnect()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID); Serial.println(" ...");
  
  while (WiFi.status() != WL_CONNECTED)
  {                                       // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print('.'); Serial.print(' ');
  }
    // Start the server
  server.begin();
  Serial.println("Server started");
  
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}
 
