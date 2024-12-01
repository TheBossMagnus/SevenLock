// Pin nfc
#define SS_PIN 10
#define RST_PIN 9
#define NUMERO_PASSWORD 1

const byte blockNumber = 8;
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
char elencoPasswordValide[NUMERO_PASSWORD][9] = { "1234" };

String leggiTag();
bool autenticaTagNfc(byte keyType, MFRC522::MIFARE_Key *key);
int apri_chiudi(int pos);
bool pswdValida(const char passwordInput[]);
Servo servo;

// legggi il contnuto del tag nfc
String leggiTag() {
  byte buffer[18];
  byte size = sizeof(buffer);

  if (autenticaTagNfc
  (MFRC522::PICC_CMD_MF_AUTH_KEY_A, &key)) {
    if (mfrc522.MIFARE_Read(blockNumber, buffer, &size) == MFRC522::STATUS_OK) {
      String content = "";
      for (byte i = 0; i < size; i++) {
        if (buffer[i] >= 32 && buffer[i] <= 126) {
          content += (char)buffer[i];
        }
      }
      return content;
    } else {
      Serial.println("Errore Lettura");
    }
  }
  return "";
}


//Autrenticazione al tag nfc
bool autenticaTagNfc
(byte keyType, MFRC522::MIFARE_Key *key) {
  MFRC522::StatusCode status = mfrc522.PCD_Authenticate( keyType, blockNumber, key, &(mfrc522.uid));

  if (status == MFRC522::STATUS_OK) {
    return true;
  } else {
    Serial.println("Errore autenticazione");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
}

//funzione per aprire e chiudere il coperchio
int apri_chiudi(int pos) {
  if (pos == 180) {
    pos = 90;
    servo.write(90);
  } else {
    pos = 180;
    servo.write(180);
  }
  delay(500);
  return pos;
}

// Funzione per verificare se la password è valida
bool pswdValida(const char passwordInput[]) {
  for (int i = 0; i < NUMERO_PASSWORD; i++) {
    if (strncmp(passwordInput, elencoPasswordValide[i], strlen(elencoPasswordValide[i])) == 0) {
      return true;
    }
  }
  return false;
}
