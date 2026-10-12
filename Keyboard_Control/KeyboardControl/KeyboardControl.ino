#include <HIRO.h>
#include <HIRO_Motion.h>
#include <WiFi.h>

HIRO hiro; // Declare an instance of HIRO.

// Set to true to have HIRO create its own Wi-Fi network.
// If false, HIRO will try to connect to home_ssid.
const bool ap = false;

// AP Settings
const char* ap_ssid = "Hiro";
const char* ap_password = "password123";

// Home Wi-Fi Settings
const char* home_ssid = "bluehouse";
const char* home_password = "1234432112344321";

WiFiServer wifiSerialServer(23);
WiFiClient remoteClient;

// State management variables
char state = 'n';
long current_state_duration = 0;
long previous_state_update_time = 0;
long max_state_duration = 2000;
unsigned long current_millis = 0;
const long movement_interval = 200;
unsigned long previous_interval_time = 0;
int step_count = 1;

void setup() {
  hiro.begin();
  
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  neutralStance(hiro);
  rgbRandom();
  delay(500);

  // Initialize Wi-Fi
  if (ap) {
    Serial.print("Creating access point...");
    WiFi.softAP(ap_ssid, ap_password);
    wifiSerialServer.begin();
    Serial.println("Wi-Fi Initialized");
    Serial.print("Network Name: ");
    Serial.println(ap_ssid);
    Serial.print("IP Address to connect to: ");
    Serial.println(WiFi.softAPIP()); // Defaults to 192.168.4.1
  } else {
    Serial.print("Connecting to network: ");
    Serial.println(home_ssid);

    WiFi.begin(home_ssid, home_password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print("Connecting...\n");
    }
    wifiSerialServer.begin();
    Serial.println("\nWiFi Connected Successfully!");
    Serial.print("IP Address to connect to: ");
    Serial.println(WiFi.localIP()); // Defaults to 192.168.1.168
    wave(hiro);
  }

  Serial.println("Hello! My name is H.I.R.O!");
}

void loop() {
  current_millis = millis();

  // Accept incoming Wi-Fi client connections
  if (!remoteClient || !remoteClient.connected()) {
    remoteClient = wifiSerialServer.accept();
    if (remoteClient) {
      remoteClient.println("--- Connected to H.I.R.O Keyboard Control ---");
      remoteClient.println("Controls: 'w' (forward), 's' (backward), 'a' (left), 'd' (right), 'n' (neutral)");
      remoteClient.println("Settings: 'c' + number (step count), 'm' + number (max duration), 'g' (get state)");
    }
  }

  // Auto-reset movement to neutral if max duration is reached
  if (current_state_duration >= max_state_duration) {
    state = 'n';
    current_state_duration = 0;
    if (remoteClient && remoteClient.connected()) {
      Serial.println("State Updated to neutral");
    }
  }

  // Parse incoming commands over Wi-Fi
  if (remoteClient && remoteClient.available()) {
    int input = remoteClient.read();

    // Ignore newlines and carriage returns
    if (input != '\n' && input != '\r') {
      if (input == 'c') {
        step_count = remoteClient.parseInt();
        Serial.print("Step count set to: ");
        Serial.println(step_count);
      } else if (input == 'm') {
        max_state_duration = remoteClient.parseInt();
        Serial.print("Max duration set to: ");
        Serial.println(max_state_duration);
      } else if (input == 'g') {
        Serial.print("Current State: ");
        Serial.println((char)state);
      } else {
        state = (char)input;
        current_state_duration = 0;
        Serial.print("Received: ");
        Serial.println((char)state);
      }
    }
  }

  // Execute motion based on active state at defined intervals
  if (current_millis - previous_interval_time >= movement_interval) {
    current_state_duration += (current_millis - previous_state_update_time);
    previous_state_update_time = current_millis;
    previous_interval_time = current_millis;

    switch (state) {
      case 'w':
        easyForward(hiro, step_count, 2);
        break;
      case 's':
        rgbRandom();
        easyBackward(hiro, step_count, 2);
        break;
      case 'a':
        rgbRandom();
        easyCounterClockwise(hiro, step_count, 2);
        break;
      case 'd':
        rgbRandom();
        easyClockwise(hiro, step_count, 2);
        break;
      case 'n':
      default:
        neutralStance(hiro);
        break;
    }
  }
}

void rgbRandom() {
  analogWrite(hiro.red, random(0, 10));
  analogWrite(hiro.green, random(0, 10));
  analogWrite(hiro.blue, random(0, 10));
}