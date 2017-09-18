/*
 * This sketch desctibes the servo energy a bit
 * ServerMotor
 */

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Servo.h> 

#define SS_PIN 5
#define INTERRUPT_PIN 15

void initSPI();
 
const char* ssid = "Nakamura-lab24";
const char* password = "Nonlinear!";
int receivedData;
 
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(50001);
WiFiClient client;
 
void setup() {
 initSPI();
  
 Serial.begin(115200);
 delay(10);
 
 // prepare GPIO2
 pinMode(2, OUTPUT);
 digitalWrite(2, 0);
  
 // Connect to WiFi network
 Serial.println();
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid);
  
 WiFi.begin(ssid, password);
  
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");
  
 // Start the server
 server.begin();
 Serial.println("Server started");
 
 // Print the IP address
 Serial.println(WiFi.localIP());
}
 
void loop() {
  String message;
  char data[256];
  client = server.available();

//whileで回すべき？


    if (client.available()) {
      Serial.println("Start Server!");
      
      // Receive message from the client
      String message = client.readStringUntil('\n');
      Serial.println(message);

      if (message == "This is a client!") {
        Serial.println("Conection suceeded!");
      }

      message = "";
  
      while(1)
      {
        if(message == "Disconnect!")
        {
          Serial.println(message);
          break;
        }
      } 

      message = "";

   if(digitalRead(INTERRUPT_PIN)==HIGH)
   {
     digitalWrite(SS_PIN, LOW);
     receivedData = SPI.transfer(0x00);  

     // Match the request
     int val;
     val = 1;
     
     // Set GPIO2 according to the request
     digitalWrite(2, val); 
     client.flush(); 
     Serial.println("点灯中 on Server");
   }
   else
   {
     Serial.println("消灯中 on Server");
     client.stop();
     return;
   }
 
   // Send the response to the client
   client.println("点灯中 on Client");
   delay(1);
   Serial.println("Client disonnected");
 
   // The client will actually be disconnected 
   // when the function returns and 'client' object is detroyed
  }
}


void initSPI() {
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.setDataMode(SPI_MODE3);
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);
}
