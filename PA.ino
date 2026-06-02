#define BLYNK_TEMPLATE_ID "TMPL2WIyxwlhq"
#define BLYNK_TEMPLATE_NAME "Smart Door"
#define BLYNK_AUTH_TOKEN "NYRN7Alk1BdHa7Qggd7D4pdsSoeOiIg0"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include <Preferences.h>

// ===== Blynk =====
char auth[] = "NYRN7Alk1BdHa7Qggd7D4pdsSoeOiIg0";
char ssid[] = "allaA73";
char pass[] = "117H128A";

// ===== Fingerprint =====
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// ===== LCD =====
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== Servo =====
Servo servo;
int servoPin = 18;

// ===== LEDs =====
int greenLED = 23;
int redLED = 4;

// ===== Ultrasonic =====
int trigPin = 5;
int echoPin = 19;

// ===== Preferences =====
Preferences prefs;

int angle = 90;

// ================= ultrasonic function =================
long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  long distance = duration * 0.034 / 2;

  return distance;
}

// ================= get name =================
String getName(int id) {
  return prefs.getString(String(id).c_str(), "Unknown");
}

// ===== fast servo function =====
void openDoorFast() {
  servo.write(40);
  delay(2000);
  servo.write(90);
}

// ================= Blynk Button  =================
BLYNK_WRITE(V3) {
  int value = param.asInt();
  if (value == 1) {
    openDoorFast();
  }
}

// ================= setup =================
void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("System Ready");
  lcd.setCursor(0,1);
  lcd.print("Put finger");

  Blynk.virtualWrite(V0, "System Ready");
  Blynk.virtualWrite(V1, "Put finger");

  servo.attach(servoPin);
  servo.write(angle);

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  mySerial.begin(57600, SERIAL_8N1, 16, 17);

  if (!finger.verifyPassword()) {
    lcd.clear();
    lcd.print("FP ERROR");
    Blynk.virtualWrite(V1, "FP ERROR");
    Serial.println("Fingerprint sensor ERROR!");
    while (1);
  }

  prefs.begin("names", false);

  Blynk.begin(auth, ssid, pass);

  delay(2000);
  lcd.clear();
}

// ================= loop =================
void loop() {
  Blynk.run();

  // continuously update distance on app
  long dist = getDistance();
  Blynk.virtualWrite(V2, dist);

  if (finger.getImage() != FINGERPRINT_OK) return;
  if (finger.image2Tz() != FINGERPRINT_OK) return;

  if (finger.fingerSearch() != FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("ERROR");
    Blynk.virtualWrite(V1, "ERROR");

    digitalWrite(redLED, HIGH);
    digitalWrite(greenLED, LOW);

    Blynk.virtualWrite(V0, "Access Denied");

    Serial.println("Fingerprint not recognized!");

    delay(2000);
    lcd.clear();
    lcd.print("Put finger");
    Blynk.virtualWrite(V1, "Put finger");
    return;
  }

  // ===== SUCCESS =====
  int id = finger.fingerID;
  String name = getName(id);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hello ");
  lcd.print(name);
  Blynk.virtualWrite(V1, "Hello " + name);

  lcd.setCursor(0,1);
  lcd.print("D:");
  lcd.print(dist);
  lcd.print("cm");

  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, LOW);

  Blynk.virtualWrite(V0, "Access Granted");

  Serial.print("Access Granted for: ");
  Serial.println(name);
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.println(" cm");

  delay(1000);

  openDoorFast();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Put finger");
  Blynk.virtualWrite(V1, "Put finger");
}
