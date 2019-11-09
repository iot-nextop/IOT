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
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  //client.println( "Refresh: 20");        // refresh the page automatically every 20 sec
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  //client.println("<p>Sample Text:<b> ");
  //client.println(FireBase_cds); 
  client.println("</b></p>");
  client.println("<a href=\"/C\"><button>ON/OFF</button></a>");
  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

  if (request.indexOf("GET /C")>=0)
  {
      String jsondata = "";
      StaticJsonBuffer<200> jsonbuffer;
      JsonObject& data = jsonbuffer.createObject();
      for(int i=0;i<10;i++)
      {
        data["index"]=i;
        data["name"]="hello";
        Firebase.push("data_esp",data);
      }
      data.printTo(jsondata);
      Serial.println(jsondata);
  }
}

void wifiConnect()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);             // Connect to the network
  Serial.print("Connecting to : ");
  Serial.print(WIFI_SSID); 
  Serial.println(" ...");
  
  while (WiFi.status() != WL_CONNECTED)
  {                                       // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print('.'); Serial.print(' ');
  }
    // Start the server
  server.begin();
  Serial.println("Server started");
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}
 
