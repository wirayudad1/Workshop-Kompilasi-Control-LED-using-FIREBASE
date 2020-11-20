#include <ESP8266WiFi.h>                                                // esp8266 library
#include <FirebaseArduino.h>                                             // firebase library  
#include "DHT.h"
#define DHTPIN D2 
#define DHTTYPE DHT11

#define FIREBASE_HOST "webserver-722a2.firebaseio.com"             // the project name address from firebase id
#define FIREBASE_AUTH "FG7FFgRbyK18BzMoa5zQN5yxcapLI4gI6eoL0jmU"       // the secret key generated from firebase

#define WIFI_SSID "seven_play"                                             // input your home or public wifi name 
#define WIFI_PASSWORD "berbahaya03"

#define led D1 //define led pin

DHT dht(DHTPIN, DHTTYPE);
// defines variables
long duration;
int n;

int trig = D2;           // membuat varibel trig yang di set ke-pin 3
int echo = D3;           // membuat variabel echo yang di set ke-pin 2
long durasi, jarak;     // membuat variabel durasi dan jarak
void setup() {
  pinMode(led, OUTPUT);
  pinMode(trig, OUTPUT);    // set pin trig menjadi OUTPUT
  pinMode(echo, INPUT);     // set pin echo menjadi INPUT
  Serial.begin(9600); // Starts the serial communication
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                      //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                                      //print local IP address
  Firebase.begin(FIREBASE_HOST);                                       // connect to firebase
  Firebase.setInt("LED_STATUS", 0);                                          //send initial string of led status
}

void Led(){
  // put your main code here, to run repeatedly:
   n = Firebase.getInt("LED_STATUS");                                      // get ld status input from firebase

   //handle status
  if (n == 1) {                                                          // compare the input of led status received from firebase
    Serial.println("Led Turned ON");                         
    digitalWrite(led, HIGH);                                                  // make bultin led ON
    //digitalWrite(led, HIGH);                                                         // make external led ON
  } 

  else{
    Serial.println("Led Turned OFF");
    digitalWrite(led, LOW);  
  }
}
void ultrasonik()
{
  // program dibawah ini agar trigger memancarakan suara ultrasonic
  digitalWrite(trig, LOW);
  delayMicroseconds(8);
  digitalWrite(trig, HIGH);
  delayMicroseconds(8);
  digitalWrite(trig, LOW);
  delayMicroseconds(8);

  durasi = pulseIn(echo, HIGH); // menerima suara ultrasonic
  jarak = (durasi / 2) / 29.1;  // mengubah durasi menjadi jarak (cm)
  Serial.println(jarak);        // menampilkan jarak pada Serial Monitor
  String dist=String(jarak) + String(" Cm");
  Firebase.setString("dht/temperature", dist);                                  
  delay(500);
}
void dht11(){
 float h = dht.readHumidity();
  float t = dht.readTemperature();
  String suhu=String(t) + String("°C ");
  Serial.print(F("  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
  Firebase.setString("dht/temperature", suhu);                                  
  delay(50);
}

void loop() {
  dht11();
  //ultrasonik();
  Led();
}
