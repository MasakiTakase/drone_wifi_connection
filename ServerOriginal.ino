#include <SPI.h>
#include <ESP8266WiFi.h>

#define SS_PIN 5
#define INTERRUPT_PIN 15

const char* ssid = "Nakamura-lab24";
const char* password = "Nonlinear!";
int receivedData;

WiFiServer server(80);
WiFiClient client;

unsigned long time1;
unsigned long time2;

void initSPI();
void spiCommunication();

void setup() {
  // SPI Settings
  initSPI();
  
  Serial.begin(115200);

  // Connect to WiFi network
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

  delay(1);

  while (client) {
    if (client.available()) {
      Serial.println("Start Server!");
      
      // Receive message from the client
      message = client.readStringUntil('\n');
      Serial.println(message);

      if (message == "This is a client!") {
        Serial.println("Conection suceeded!");
      }

      message = "";

      // Set interrupt
      attachInterrupt(INTERRUPT_PIN, spiCommunication, RISING);
      
      delay(10);

      while (1) {
        // Receive message from the client
        message = client.readStringUntil('\n');
        
        if (message == "Disconnect!") {
          Serial.println(message);
          break;
        }

        message = "";
        
        // Send message to the client
        sprintf(data, "%d", receivedData);
        time1 = micros();
        client.println(time1); //clientに届くデータ
        time2 = micros();
        Serial.println(data); //serverのシリアルポートに表示されるデータ
      }
      delay(10);
      client.stop();
    }
  }
  delay(1);
}

void initSPI() {
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.setDataMode(SPI_MODE3);
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);
}

void spiCommunication() {
  digitalWrite(SS_PIN, LOW);
  receivedData = SPI.transfer(0x00);
  digitalWrite(SS_PIN, HIGH);
}

