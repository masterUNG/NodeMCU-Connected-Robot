//นี่คือโค้ด ฝั่ง nodeMCU

// Add Libraey
#include <ESP8266WiFi.h>  // Connected WiFi
#include <Servo.h>        // Use Servo

#define SERVO_PIN D2  //define pin D2 on board
#define SERVO_PIN2 D3 //define pin D3 on board
#define SERVO_PIN3 D4 //define pin D4 on board
#define SERVO_PIN4 D5 //define pin D5 on board


const char* ssid     = "shareEWTC";     // Set SSID
const char* password = "12345abcde"; // Set password
const char* host = "dweet.io";          // Set host 

// ประกาศ object ของการใช้ Servo
Servo myservo, myservo2, myservo3, myservo4;  
int analogValue, intServo;

// ##############################################################################
// setup
// ##############################################################################

void setup() 
{
  //กำหนดขาควบคุม Servo
  myservo.attach(SERVO_PIN);
  myservo2.attach(SERVO_PIN2);
  myservo3.attach(SERVO_PIN3);
  myservo4.attach(SERVO_PIN4);
  
  Serial.begin(115200);                 // Print setting message
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");                  // Print WiFi status
  }
  Serial.println("");
  Serial.println("WiFi connected");     // Print connect status
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());       // Print IP address
} // setup

// ##############################################################################
// loop
// ##############################################################################


void loop() 
{
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;              // Set HTTP Port
  if (!client.connect(host, httpPort))  // Test connection
  {
    Serial.println("connection failed");  // Print connection fail messag
    return;
  }

  // ##############################################################################
  // https://dweet.io/get/latest/dweet/for/SuperMaster
  // ##############################################################################
  
  client.print(String("GET /get/latest/dweet/for/SuperMaster HTTP/1.1\r\n") +
               "Host: " + host + "\r\n" +
               "Connection: keep-alive\r\n" +
               "Cache-Control: max-age=0\r\n\r\n");
  delay(3000);
  
  while (client.available()) 
  {
    String strJSON = client.readStringUntil('\r');
    Serial.println(strJSON);  // Display All JSON

    String strAnalog = strJSON.substring(139, 142);
    Serial.println(strAnalog);  // Display only Value from Dweet

    String strServo =  strJSON.substring(136, 137);
    intServo = strServo.toInt();
    Serial.println();
    Serial.print("Servo ==> ");
    Serial.println(strServo);

    analogValue = strAnalog.toInt(); // Change String to int

    //การกำหนด องศา ของ Servo (ตัวแปร, ค่าเริ่มต้น, 0, 179)
    //analogValue = map(analogValue, 0, 179, 0, 179);

    Serial.println();
    Serial.println();
    Serial.print("andlogValue ==> ");
    Serial.println(analogValue);
    
    myservo.write(analogValue);
  
  } // while

switch (intServo) {
    case 0:
    Serial.println(analogValue);
    analogValue = map(analogValue, 0, 100, 129, 160); 
      myservo.write(analogValue);
      break;
    case 1:
    Serial.println(analogValue);
    analogValue = map(analogValue, 0, 100, 30, 180); 
      myservo2.write(analogValue);
      break;
      case 2:
      Serial.println(analogValue); 
      analogValue = map(analogValue, 0, 100, 53, 103);
      myservo3.write(analogValue);
      break;
    case 3:
     Serial.println(analogValue);
    analogValue = map(analogValue, 0, 100, 54, 104); 
      myservo4.write(analogValue);
      break;
    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
  }
  
  
} // loop
