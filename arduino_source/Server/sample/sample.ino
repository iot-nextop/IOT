#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""
 
int cds = A0;
int led = 13;

int FB_cds;
int FB_led;

WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(led,OUTPUT);

  // Connect to WiFi network
  wifiConnect();
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  FB_cds = Firebase.getInt("CDS");
  FB_led = Firebase.getInt("LED");
}
 
void loop() {
  String line = "";
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

  // Always update the photocell value anytime there's a request
  // NOTE: We have the cmd=RELOAD_PHOTOCELL command because we need a way
  // to update the photocell without changing the led state for the user
  int cds_value = analogRead(cds);
  
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  //client.println( "Refresh: 20");        // refresh the page automatically every 20 sec
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.println("<p>Cds value is:<b> ");
  client.println(FB_cds); 
  client.println("</b></p>");
  client.println("<a href=\"/C\"><button>LED ON/OFF </button></a>");
  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

  if (request.indexOf("GET /C")>=0)
  {
      if(cds_value > 500)
      {
          FB_cds = cds_value;
          FB_led = 1;
          Firebase.setInt("CDS",FB_cds);
          Firebase.setInt("LED",FB_led);
          digitalWrite(led,HIGH);
          Serial.println("LED is now ON");
       }
       else
       {
          FB_cds = cds_value;
          FB_led = 0;
          Firebase.setInt("CDS",FB_cds);
          Firebase.setInt("LED",FB_led);
          digitalWrite(led,LOW);
          Serial.println("LED is now OFF");
       }
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
 
