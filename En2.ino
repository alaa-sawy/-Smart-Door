#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <Preferences.h>

HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

Preferences prefs;

uint8_t getFingerprintEnroll(int id);

bool running = true;

void setup() {

  Serial.begin(115200);
  mySerial.begin(57600, SERIAL_8N1, 16, 17);

  delay(2000);

  prefs.begin("names", false);

  Serial.println("Initializing fingerprint sensor...");

  if (finger.verifyPassword()) {
    Serial.println("Fingerprint sensor detected!");
  } else {
    Serial.println("Sensor NOT found!");
    while (1);
  }

  Serial.println("Press 'x' to stop program");
}

void loop() {

  if (Serial.available()) {

    char c = Serial.read();

    if (c == 'x' || c == 'X') {
      Serial.println("done");
      while (1);
    }
  }

  Serial.println("\nEnter name:");
  while (!Serial.available());

  String name = Serial.readStringUntil('\n');
  name.trim();

  Serial.println("Place finger to enroll...");

  int id = prefs.getUInt("last_id", 0) + 1;

  if (getFingerprintEnroll(id)) {

    prefs.putString(String(id).c_str(), name);
    prefs.putUInt("last_id", id);

    Serial.print("Enrolled ID: ");
    Serial.print(id);
    Serial.print(" | Saved Name: ");
    Serial.println(name);

  } else {
    Serial.println("Enrollment failed");
  }

  delay(3000);
}

uint8_t getFingerprintEnroll(int id) {

  int p = -1;

  Serial.println("Place finger...");
  delay(1000);

  while ((p = finger.getImage()) != FINGERPRINT_OK) {
    delay(200);
  }

  if (finger.image2Tz(1) != FINGERPRINT_OK) {
    Serial.println("Error: image1");
    return 0;
  }

  Serial.println("Remove finger completely...");

  while (finger.getImage() != FINGERPRINT_NOFINGER) {
    delay(200);
  }

  delay(2000);

  Serial.println("Place SAME finger again...");

  p = -1;
  while ((p = finger.getImage()) != FINGERPRINT_OK) {
    delay(200);
  }

  if (finger.image2Tz(2) != FINGERPRINT_OK) {
    Serial.println("Error: image2");
    return 0;
  }

  if (finger.createModel() != FINGERPRINT_OK) {
    Serial.println("Model creation failed");
    return 0;
  }

  if (finger.storeModel(id) != FINGERPRINT_OK) {
    Serial.println("Storage failed");
    return 0;
  }

  Serial.println("Fingerprint stored successfully!");
  return 1;
}

