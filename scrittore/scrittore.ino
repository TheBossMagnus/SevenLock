#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key keyA;

bool authenticateWithKey(byte keyType, MFRC522::MIFARE_Key *key);
bool writeBlock(byte *data);
String leggiSerial();

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  for (byte i = 0; i < 6; i++) {
    keyA.keyByte[i] = 0xFF;
  }

  Serial.println("Avvicina un tag NFC al lettore per scrivere 'pswd'...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  if (writeTagContent(leggiSerial())) {
    Serial.println("pswd scritta con successo!");
  } else {
    Serial.println("Errore durante la scrittura della pswd.");
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

bool writeTagContent(String content) {
  byte buffer[16];
  content.toCharArray((char *)buffer, 16);

  if (authenticateWithKey(MFRC522::PICC_CMD_MF_AUTH_KEY_A, &keyA)) {
    if (writeBlock(buffer)) {
      return true;
    }
  }

  return false;
}

bool authenticateWithKey(byte keyType, MFRC522::MIFARE_Key *key) {
  byte blockNumber = 8;

  MFRC522::StatusCode status = mfrc522.PCD_Authenticate(
    keyType,
    blockNumber,
    key,
    &(mfrc522.uid)
  );

  if (status == MFRC522::STATUS_OK) {
    return true;
  } else {
    Serial.print("Errore di autenticazione: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
}

bool writeBlock(byte *data) {
  byte blockNumber = 8;

  MFRC522::StatusCode status = mfrc522.MIFARE_Write(blockNumber, data, 16);

  if (status == MFRC522::STATUS_OK) {
    return true;
  } else {
    Serial.print("Errore durante la scrittura: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
}

String leggiSerial() {
  String password = "";
  while (password.length() == 0) {
    if (Serial.available() > 0) {
      password = Serial.readString();
    }
  }
  return password;
}