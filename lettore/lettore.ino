#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <string.h>
#include <stdlib.h>
#include <Servo.h>
#include "funzioni.h"



int posizioneCorrente = 90;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  servo.attach(3);
  servo.write(90);
  Serial.begin(9600);
  SPI.begin();
  posizioneCorrente = apri_chiudi(posizioneCorrente);
  mfrc522.PCD_Init();

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Avvicina tag nfc");
}

void loop() {
  lcd.clear();
  lcd.print("Avvicina tag nfc");

  // Verifica se c'è un nuovo tag presente
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Leggi i dati dal tag
    String tagContent = leggiTag();
    Serial.println("Letto tag con: ");
    Serial.println(tagContent);

    // Controlla se il contenuto del tag è una password valida, se si apri/chiudi
    if (pswdValida(tagContent.c_str())) {
      Serial.println("Tag autorizzato");
      lcd.clear();
      lcd.print("Autorizzato!");
      lcd.setCursor(0, 1);
      lcd.print("Riavvicina tag");
      posizioneCorrente = apri_chiudi(posizioneCorrente);
      delay(1000);
      lcd.clear();
    } else {
      lcd.clear();
      lcd.print("Non autorizzato.");
    }

    //interrompi la connessione nfc
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(1000);
  }
}
