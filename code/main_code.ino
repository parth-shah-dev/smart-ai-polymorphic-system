//#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266_ISR_Servo.h>


#define REFERENCE_URL "xyz"
#define FIREBASE_AUTH "xyz"
#define WIFI_SSID "xyz"
#define WIFI_PASSWORD "xyz"
#define DUSTBIN_LENGTH 22
const int ULTRASONIC_TRIG_PIN = D7;
const int ULTRASONIC_ECHO_PIN = D8;
double distanceVal = 0;

#define IR_SENSOR_PIN D3
#define SERVO_PIN D4

//ISR_Servo lidServo;
void setup(){
  pinMode(ULTRASONIC_TRIG_PIN,OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN,INPUT);
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  while(WiFi.status()!=WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  Serial.println(DUSTBIN_LENGTH);
  //Firebase.begin(REFRENCE_URL,FIREBASE_AUTH);
  
  //pinMode(D5,OUTPUT);
  //pinMode(D6,OUTPUT);
  //if(Firebase.failed()){
    //Serial.print("Firebase connection failed: ");
    //serial.println(Firebase.error());
  //}
  /*else{
    Serial.println("Firebase Connected");
    Firebase.setString("/waste_Management/distance","0");
    Firebase.setString("/waste_Management/Remaining","0");
  }
  */

}

pinMode(IR_SENSOR_PIN,INPUT);
lidServo.attach(SERVO_PIN);

void loop(){
  measureDistance();

  int remainingSpace = (DUSTBIN_LENGTH -distenceVal);

  Serial.print("Distence: ");
  serial.println(distanceVal);

  Serial.print("Garbage level: ");
  Serial.println(remainingSpace);

  if(remainingSpace <=8){
    Serial.println("Empty");
  }
  
  else if(remainingSpace <=16){
    Serial.println("Half Full");
  }
  
  else if(remainingSpace >=17){
    Serial.println("Full");
  }

  if(motionDetected){
    openLid();
    delay(5000);

  }
  delay(1000);
  //Firebase.setInt("Remaining: ",remainingSpace);
  delay(1000);
}

void measureDistance(){
  degitalWrite(ULTRASONIC_TRIG_PIN,LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN,HIGH);
  delayMicroseconds(10);
  degitalWrite(ULTRASONIC_TRIG_PIN,LOW);
  
  long duration = pulseIn(ULTRASONIC_ECHO_PIN,HIGH);
  distanceVal = duration * 0.034 / 2;

  //Firebase.setFloat("distance",distanceVal);
}

void openLid(){
  lidServo.write(90);
  delay(1000);
  lidServo.write(0);
}




/* **********************
FireBase code
************************ */
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>  // Use the latest Firebase library for ESP8266

float randNumber1;

#define FIREBASE_HOST "test-7bed8-default-rtdb.firebaseio.com"  // Host without https:// and the trailing "/"
#define FIREBASE_AUTH "6o4dChpzi8MzEKKV6DIFEQVDqUM15XQLu3IbhxmP"  // Replace with your database secret
#define WIFI_SSID "Mital 5g"
#define WIFI_PASSWORD "Mital@123"

// Firebase configuration and data objects
FirebaseConfig config;
FirebaseAuth auth;
FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to Wi-Fi, IP: ");
  Serial.println(WiFi.localIP());

  // Set Firebase configuration
  config.host = FIREBASE_HOST;
  config.database_url = "https://" FIREBASE_HOST;  // Full database URL
  config.signer.tokens.legacy_token = FIREBASE_AUTH;  // Using legacy database secret for authentication

  // Initialize Firebase connection
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);  // Reconnect Wi-Fi automatically if disconnected

  Serial.println("Firebase initialized");
}

void loop() {
  Serial.println("Sending data...");

  randNumber1 = random(10);

  // Set float value to Firebase Realtime Database
  if (Firebase.setFloat(firebaseData, "/Smoke", randNumber1)) {
    Serial.println("Data sent successfully");
  } else {
    Serial.println("Failed to send data");
    Serial.println(firebaseData.errorReason());
  }

  delay(10000);  // Delay for 10 seconds before sending next data
}



/* ********************
Blynk code
********************** */
#define BLYNK_TEMPLATE_ID "TMPL3G1XKsCtb"
#define BLYNK_TEMPLATE_NAME "motor"
#define BLYNK_AUTH_TOKEN "idXml8kETAO1bnQKk8R1bvSpp_bFv_6b"

#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Parth";
char pass[] = "Parth@123";

#define MOTOR_PIN D4  // Motor connected to pin D4

void setup() {
    Blynk.begin(auth, ssid, pass);
    pinMode(MOTOR_PIN, OUTPUT);  // Set the motor pin as output
    digitalWrite(MOTOR_PIN, LOW);  // Start with motor off
}

void loop() {
    Blynk.run();
}

// Function to run motor when button is pressed in the app
BLYNK_WRITE(V1) {
    int buttonState = param.asInt();  // Read button state
    digitalWrite(MOTOR_PIN, buttonState);
}

