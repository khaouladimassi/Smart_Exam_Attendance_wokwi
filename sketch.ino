#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9

#define LED_GREEN 6
#define LED_RED 7
#define BUZZER 8

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------- USER ----------------
struct User {
  String uid;
  String name;
  String role;
  int presenceCount;
  bool isInside;
  int scanCount;
};

User users[] = {
  {"01020304", "khaoula", "STUDENT", 0, false, 0},
  {"11223344", "khouloud", "STUDENT", 0, false, 0},
  {"55667788", "Mr.Wissem", "PROF", 0, false, 0}
};

const int userCount = sizeof(users) / sizeof(users[0]);

int totalStudents = 0;

// ---------------- ANTI SPAM ----------------
String lastUID = "";
unsigned long lastScanTime = 0;
bool cardActive = false;

// ---------------- UID ----------------
String getUID() {
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();
  return uid;
}

// ---------------- FIND USER ----------------
int findUser(String uid) {
  for (int i = 0; i < userCount; i++) {
    if (users[i].uid == uid) return i;
  }
  return -1;
}

// ---------------- EFFECTS ----------------
void success() {
  digitalWrite(LED_GREEN, HIGH);
  tone(BUZZER, 1000, 200);
  delay(200);
  digitalWrite(LED_GREEN, LOW);
}

void error() {
  digitalWrite(LED_RED, HIGH);
  tone(BUZZER, 200, 400);
  delay(400);
  digitalWrite(LED_RED, LOW);
}

// ---------------- LCD ----------------
void showLCD(String l1, String l2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(l1);
  lcd.setCursor(0, 1);
  lcd.print(l2);
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();

  showLCD("SMART RFID", "READY");
  Serial.println("SMART RFID SYSTEM STARTED");
}

// ---------------- LOOP ----------------
void loop() {

  if (!rfid.PICC_IsNewCardPresent()) {
    cardActive = false;
    return;
  }

  if (!rfid.PICC_ReadCardSerial()) return;

  if (cardActive) return;
  cardActive = true;

  String uid = getUID();
  String timeStr = String(millis() / 1000) + "s";

  int index = findUser(uid);

  Serial.println("\n===============================");
  Serial.println("📡 RFID SCAN");

  Serial.print("🆔 UID: ");
  Serial.println(uid);

  Serial.print("⏱ TIME: ");
  Serial.println(timeStr);

  // ---------------- UNKNOWN ----------------
  if (index == -1) {
    Serial.println("❌ ACCESS DENIED");
    showLCD("ACCESS DENIED", "UNKNOWN CARD");
    error();
    rfid.PICC_HaltA();
    return;
  }

  User &u = users[index];

  Serial.print("🧑 ROLE: ");
  Serial.println(u.role);

  Serial.print("👤 NAME: ");
  Serial.println(u.name);

  // ================= PROF =================
  if (u.role == "PROF") {

    if (u.scanCount >= 1) {
      Serial.println("⛔ PROF BLOCKED");
      showLCD("PROF BLOCKED", u.name);
      error();
      rfid.PICC_HaltA();
      return;
    }

    u.scanCount = 1;

    Serial.println("👨‍🏫 ACCESS GRANTED");
    showLCD("PROF OK", u.name);

    success();
    rfid.PICC_HaltA();
    return;
  }

  // ================= STUDENT =================
  if (u.scanCount >= 3) {
    Serial.println("⛔ STUDENT BLOCKED");
    showLCD("BLOCKED", u.name);
    error();
    rfid.PICC_HaltA();
    return;
  }

  u.scanCount++;

  Serial.print("📊 PRESENCE: ");
  Serial.println(u.presenceCount);

  if (u.scanCount == 1) {
    u.isInside = true;
    u.presenceCount = 1;
    totalStudents++;

    Serial.println("🟢 ACTION: ENTRY");

    showLCD("ENTRY", u.name);
  }

  else if (u.scanCount == 2) {
    u.isInside = false;
    u.presenceCount = 0;

    Serial.println("🔴 ACTION: EXIT");

    showLCD("EXIT", u.name);
  }

  else if (u.scanCount == 3) {
    Serial.println("⛔ ACTION: BLOCKED");
    showLCD("LIMIT REACHED", u.name);
    error();
  }

  Serial.print("📈 TOTAL STUDENTS: ");
  Serial.println(totalStudents);

  Serial.println("===============================");

  success();

  rfid.PICC_HaltA();

  delay(1000);
}