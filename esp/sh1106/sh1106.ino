#include <WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

const char* ssid     = "";
const char* password = "";

WiFiUDP udp;
const uint16_t localPort = 5000;

#define OLED_MOSI 23
#define OLED_CLK  18
#define OLED_DC   16
#define OLED_CS   5
#define OLED_RST  17

Adafruit_SH1106G display =
  Adafruit_SH1106G(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);

uint8_t bitmap[1024];

void setup() {
  Serial.begin(115200);

  /* WiFi */
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  /* UDP */
  udp.begin(localPort);
  Serial.printf("Listening on UDP port %d\n", localPort);

  /* OLED */
  display.begin(0, true);
  display.clearDisplay();
  display.display();
}

void loop() {
  int packetSize = udp.parsePacket();

  if (packetSize == 1024) {
    int len = udp.read(bitmap, 1024);

    if (len == 1024) {
      // Serial.println("Bitmap received");

      display.clearDisplay();
      display.drawBitmap(0, 0, bitmap, 128, 64, SH110X_WHITE);
      display.display();
    }
  }
  else if (packetSize > 0) {
    while (udp.available()) udp.read();
    Serial.printf("Discarded packet size: %d\n", packetSize);
  }
}
