#include <SPI.h>
#include <MFRC522.h>

// Definizione dei pin
#define SS_PIN 10  // Pin per SDA del modulo RC522
#define RST_PIN 9  // Pin per RESET del modulo RC522

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Creazione dell'oggetto MFRC522
MFRC522::MIFARE_Key keyA;

char pswd[64]={"1234"};

void setup() {
  Serial.begin(9600);  // Inizializza la comunicazione seriale
  SPI.begin();          // Inizializza il bus SPI
  mfrc522.PCD_Init();   // Inizializza il modulo RC522

  // Inizializza la chiave di default a 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
  for (byte i = 0; i < 6; i++) {
    keyA.keyByte[i] = 0xFF;  // Chiave di default per Key A
  }

  Serial.println("Avvicina un tag NFC al lettore per scrivere 'pswd'...");
}

void loop() {
  // Verifica se c'è un nuovo tag presente
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Verifica se possiamo leggere il tag
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Tentativo di scrivere la parola "pswd" sul tag
  if (writeTagContent(pswd)) {

    Serial.println("Parola 'pswd' scritta con successo!");

  } else {
    Serial.println("Errore durante la scrittura della parola 'pswd'.");
  }


  // Ferma la comunicazione con il tag
  mfrc522.PICC_HaltA();
}

// Funzione per scrivere il contenuto sul tag NFC con tentativi di entrambe le autenticazioni e senza autenticazione
bool writeTagContent(String content) {
  byte buffer[16];  // Buffer per contenere i dati da scrivere (16 byte max)
  content.toCharArray((char *)buffer, 16);  // Converte la stringa in array di caratteri

  // Tentativo di autenticazione con Key A
  if (authenticateWithKey(MFRC522::PICC_CMD_MF_AUTH_KEY_A, &keyA)) {
    // Se l'autenticazione con Key A ha successo, scrivi il blocco
    if (writeBlock(buffer)) {
      return true;
    }
  }

  // Se nessuna delle opzioni ha successo
  return false;
}

// Funzione per tentare l'autenticazione con una chiave specifica
bool authenticateWithKey(byte keyType, MFRC522::MIFARE_Key *key) {
  byte blockNumber = 8;  // Blocco su cui scrivere (modifica se necessario)

  MFRC522::StatusCode status = mfrc522.PCD_Authenticate(
    keyType,         // Tipo di chiave (A o B)
    blockNumber,     // Blocco che vuoi scrivere
    key,             // Chiave (A o B)
    &(mfrc522.uid)   // UID del tag
  );

  if (status == MFRC522::STATUS_OK) {
    return true;
  } else {
    Serial.print("Errore di autenticazione (");
    Serial.print((keyType == MFRC522::PICC_CMD_MF_AUTH_KEY_A) ? "Key A" : "Key B");
    Serial.print("): ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
}

// Funzione per scrivere su un blocco specifico
bool writeBlock(byte *data) {
  byte blockNumber = 8;  // Blocco su cui scrivere (modifica se necessario)
  
  // Scrivi il blocco del tag
  MFRC522::StatusCode status = mfrc522.MIFARE_Write(blockNumber, data, 16);
  
  if (status == MFRC522::STATUS_OK) {
    return true;
  } else {
    Serial.print("Errore durante la scrittura: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
}

// Funzione per tentare la scrittura senza autenticazione
bool writeBlockWithoutAuth(byte *data) {
  byte blockNumber = 8;  // Blocco su cui scrivere (modifica se necessario)

  Serial.println("Tentativo di scrittura senza autenticazione...");

  // Scrivi direttamente sul blocco senza autenticazione
  MFRC522::StatusCode status = mfrc522.MIFARE_Write(blockNumber, data, 16);

  if (status == MFRC522::STATUS_OK) {
    Serial.println("Scrittura senza autenticazione riuscita.");
    return true;
  } else {
    Serial.print("Errore durante la scrittura senza autenticazione: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
}