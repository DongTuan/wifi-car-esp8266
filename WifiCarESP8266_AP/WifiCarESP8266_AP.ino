/**
  @file    WifiCarESP8266_AP.ino
  @brief   Simple example of Wifi Car controlled by a web server in AP Mode. See also :
           http://www.instructables.com/id/A-very-cheap-ESP8266-WiFi-smart-car-controlled-by-/

           List of commands to control the car :
           - http://<YourIP>:<YourPort>/?State=F (Forward)
           - http://<YourIP>:<YourPort>/?State=B (Backward)
           - http://<YourIP>:<YourPort>/?State=R (TurnRight)
           - http://<YourIP>:<YourPort>/?State=L (TurnLeft)
           - http://<YourIP>:<YourPort>/?State=S (Stop)

  @author  LACOUR Vincent
  @date    2018-01
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "Wifi_Car_ESP8266"; // SSID Name
const char *password = "wificarpassword"; // SSID Password : Between 8 and 32 carateres

IPAddress ip(192, 168, 4, 1); // IP Address
IPAddress netmask(255, 255, 255, 0); // Netmask
const int port = 80; // Port
ESP8266WebServer server(port);

// Motors pins
static const uint8_t pwm_A = 5 ;
static const uint8_t pwm_B = 4;
static const uint8_t dir_A = 0;
static const uint8_t dir_B = 2;

// Motor speed = [0-1024]
int motor_speed = 1024;

void setup() {
  WiFi.mode(WIFI_AP); //Only Access point
  WiFi.softAPConfig(ip, ip, netmask);
  WiFi.softAP(ssid, password);

  // Declaration of motors
  pinMode(pwm_A, OUTPUT); // PMW A
  pinMode(pwm_B, OUTPUT); // PMW B
  pinMode(dir_A, OUTPUT); // DIR A
  pinMode(dir_B, OUTPUT); // DIR B

  // Start Server
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}


void handleRoot() {
  if (server.hasArg("State")) {
    String command = server.arg("State");
    if (command.equals("F")) {
      forward();
      server.send(200, "text / plain", "Forward");
    }
    else  if (command.equals("B")) {
      backward();
      server.send(200, "text / plain", "Backward");
    }
    else  if (command.equals("L")) {
      turn_left();
      server.send(200, "text / plain", "Turn Left");
    }
    else  if (command.equals("R")) {
      turn_right();
      server.send(200, "text / plain", "Turn Right");
    }
    else  if (command.equals("S")) {
      stop_motors();
      server.send(200, "text / plain", "Stop");
    }
  }
}

void stop_motors() {
  analogWrite(pwm_A, 0);
  analogWrite(pwm_B, 0);
}

void backward() {
  analogWrite(pwm_A, motor_speed);
  analogWrite(pwm_B, motor_speed);
  digitalWrite(dir_A, LOW);
  digitalWrite(dir_B, HIGH);
}

void forward() {
  analogWrite(pwm_A, motor_speed);
  analogWrite(pwm_B, motor_speed);
  digitalWrite(dir_A, HIGH);
  digitalWrite(dir_B, LOW);
}

void turn_left() {
  analogWrite(pwm_A, motor_speed);
  analogWrite(pwm_B, motor_speed);
  digitalWrite(dir_A, HIGH);
  digitalWrite(dir_B, HIGH);
}

void turn_right() {
  analogWrite(pwm_A, motor_speed);
  analogWrite(pwm_B, motor_speed);
  digitalWrite(dir_A, LOW);
  digitalWrite(dir_B, LOW);
}

