/*
    This sketch sends a message to a TCP server

*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>


#include <Wire.h>
#include <SPI.h>
#include "Adafruit_PN532.h"

#include "Shifty.h"

#include "sha256.h"

#define PN532_SCK  (13)
#define PN532_MOSI (5)
#define PN532_SS   (2)
#define PN532_MISO (14)

#define SH_DATA   (12)
#define SH_LATCH  (16)
#define SH_CLK    (4)
#define SH_IN     (0)

#define BUZZER  (15)

#define TEXT_HASH_SIZE (2*SHA256_BLOCK_SIZE+1)

ESP8266WiFiMulti WiFiMulti;

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

Shifty shift;

char userList[100][TEXT_HASH_SIZE];
uint16_t userListLen = 2;

void hashByte(byte* textByte, int len, byte* hash) {
  Sha256* hasher = new Sha256();
  hasher->update(textByte, len);
  hasher->final(hash);
}

void hashHex(byte* textByte, int len, char* texthash) {
  byte hash[SHA256_BLOCK_SIZE];

  hashByte(textByte, len, hash);

  for (int i = 0; i < SHA256_BLOCK_SIZE; ++i)
    sprintf(texthash + 2 * i, "%02X", hash[i]);
}

void sendLogToServer() {

}

void loginToServer() {
  // push message to mqtt

}


void updateUserList() {
  // mqtt connection check for update
  // json parsing
  char blub[TEXT_HASH_SIZE] = "4FF11748DAC46085D934C9BAB6D9435E2F0631FBF1A8C4E99ED84EDF2A16631E";
  char blub2[TEXT_HASH_SIZE] = "4FF11748DAC46085D9D5C9BAB6D9435E2F0631FBF1A8C4E99ED84EDF2A16631E";
  for (uint16_t i = 0; i < TEXT_HASH_SIZE; i++) {
    userList[0][i] = blub[i];
  }
  for (uint16_t i = 0; i < TEXT_HASH_SIZE; i++) {
    userList[1][i] = blub2[i];
  }
}

bool checkIfUserValid(char* userHash) {
  Serial.println(userHash);
  for (uint16_t i = 0; i < userListLen; i++) {
    for (uint16_t j = 0; j < TEXT_HASH_SIZE; j++) {

      Serial.print(userHash[j]);
      Serial.println(userList[i][j]);
      if (userHash[j] != userList[i][j]) {
        break;
      } else if (j == TEXT_HASH_SIZE - 1) {
        return true;
      }
    }
  }
  Serial.println("false");
  tone(BUZZER, 1500, 1500);
  return false;
}

uint8_t readCard(char* retVal) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");

    if (uidLength == 4)
    {
      // We probably have a Mifare Classic card ...
      Serial.println("Seems to be a Mifare Classic card (4 byte UID)");

      // Now we need to try to authenticate it for read/write access
      // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
      Serial.println("Trying to authenticate block 4 with default KEYA value");
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

      // Start with block 4 (the first block of sector 1) since sector 0
      // contains the manufacturer data and it's probably better just
      // to leave it alone unless you know what you're doing
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);

      if (success)
      {
        Serial.println("Sector 1 (Blocks 4..7) has been authenticated");
        uint8_t data[16];

        // If you want to write something to block 4 to test with, uncomment
        // the following line and this text should be read back in a minute
        //memcpy(data, (const uint8_t[]){ 'a', 'd', 'a', 'f', 'r', 'u', 'i', 't', '.', 'c', 'o', 'm', 0, 0, 0, 0 }, sizeof data);
        // success = nfc.mifareclassic_WriteDataBlock (4, data);

        // Try to read the contents of block 4
        success = nfc.mifareclassic_ReadDataBlock(4, data);

        if (success)
        {
          // Data seems to have been read ... spit it out
          Serial.println("Reading Block 4:");
          nfc.PrintHexChar(data, 16);
          Serial.println("");

          for (uint8_t i = 0; i < (uidLength + 16); i++) {
            if (i < uidLength) {
              retVal[i] = uid[i];
            } else {
              retVal[i] = data[i - uidLength];
            }
          }

          nfc.PrintHexChar(data, uidLength + 16);
          Serial.println("");

          return uidLength + 16;
        }
        else
        {
          Serial.println("Ooops ... unable to read the requested block.  Try another key?");
        }
      }
      else
      {
        Serial.println("Ooops ... authentication failed: Try another key?");
      }
    }

    //    if (uidLength == 7)
    //    {
    //      // We probably have a Mifare Ultralight card ...
    //      Serial.println("Seems to be a Mifare Ultralight tag (7 byte UID)");
    //
    //      // Try to read the first general-purpose user page (#4)
    //      Serial.println("Reading page 4");
    //      uint8_t data[32];
    //      success = nfc.mifareultralight_ReadPage (4, data);
    //      if (success)
    //      {
    //        // Data seems to have been read ... spit it out
    //        nfc.PrintHexChar(data, 4);
    //        Serial.println("");
    //
    //        // Wait a bit before reading the card again
    //        openDoor();
    //        delay(1000);
    //      }
    //      else
    //      {
    //        Serial.println("Ooops ... unable to read the requested page!?");
    //      }
    //    }
  }

  return -1;
}

uint8_t readKeyPad(char * pin) {
  // activate keypad and some signal or something
  tone(BUZZER, 2000, 200);

  // read key till end signal
  pin[0] = '1';
  pin[1] = '1';
  pin[2] = '1';

  delay(1500);

  return 3;
}

bool authentificateCard() {
  char cache[1000];
  char pin[20];
  char result[2 * SHA256_BLOCK_SIZE + 1];
  // read card
  uint8_t cardResult = readCard(cache);

  if (cardResult > 0) {
    // card read, ask user for pin
    uint8_t pinResult = readKeyPad(pin);

    if (pinResult > 0) {
      // got pieces in place --> hash it
      hashByte((byte*) pin, pinResult, (byte*) (cache + cardResult));
      hashHex((byte*) cache, cardResult + SHA256_BLOCK_SIZE, result);

      // check list for validity
      return checkIfUserValid(result);
    }
  }
  return false;
}


void openDoor() {
  tone(BUZZER, 3000, 150);
  delay(300);
  tone(BUZZER, 3000, 150);
  shift.batchWriteBegin();
  shift.writeBit(1, HIGH);
  shift.writeBit(2, HIGH);
  shift.writeBit(3, HIGH);
  shift.writeBit(4, HIGH);
  shift.writeBit(5, HIGH);
  shift.writeBit(6, HIGH);
  shift.writeBit(7, HIGH);
  shift.writeBit(8, HIGH);
  shift.batchWriteEnd();
  delay(500);
  shift.writeBit(0, LOW);
  delay(500);
  shift.writeBit(0, HIGH);
  delay(5000);

  shift.batchWriteBegin();
  shift.writeBit(1, LOW);
  shift.writeBit(2, LOW);
  shift.writeBit(3, LOW);
  shift.writeBit(4, LOW);
  shift.writeBit(5, LOW);
  shift.writeBit(6, LOW);
  shift.writeBit(7, LOW);
  shift.writeBit(8, LOW);
  shift.batchWriteEnd();
  delay(500);
  shift.writeBit(0, LOW);
  delay(500);
  shift.writeBit(0, HIGH);
  delay(500);
}


void setup() {
  updateUserList();
  // Serieal Interface
  Serial.begin(115200);
  delay(10);
  Serial.println("");

  // Buzzer
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, HIGH);

  // Shift Register
  shift.setBitCount(16);
  shift.setPins(SH_DATA, SH_CLK, SH_LATCH, SH_IN);

  shift.batchWriteBegin();
  shift.writeBit(0, HIGH);

  shift.writeBit(1, LOW);
  shift.writeBit(2, LOW);
  shift.writeBit(3, LOW);
  shift.writeBit(4, LOW);
  shift.writeBit(5, LOW);
  shift.writeBit(6, LOW);
  shift.writeBit(7, LOW);
  shift.writeBit(8, LOW);
  shift.writeBit(9, LOW);
  shift.writeBit(10, LOW);
  shift.writeBit(11, LOW);
  shift.writeBit(12, LOW);
  shift.writeBit(13, LOW);
  shift.writeBit(14, LOW);
  shift.writeBit(15, LOW);
  shift.batchWriteEnd();

  // NFC
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A Card ...");

  // WIFI connection
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("JuLi_HomeNet", "derkonsumgehtumher");

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}


uint16_t loopCounter = 0;
void loop() {
  //    const uint16_t port = 80;
  //    const char * host = "192.168.1.1"; // ip or dns
  //
  //
  //
  //    Serial.print("connecting to ");
  //    Serial.println(host);
  //
  //    // Use WiFiClient class to create TCP connections
  //    WiFiClient client;
  //
  //    if (!client.connect(host, port)) {
  //        Serial.println("connection failed");
  //        Serial.println("wait 5 sec...");
  //        delay(5000);
  //        return;
  //    }
  //
  //    // This will send the request to the server
  //    client.println("Send this data to server");
  //
  //    //read back one line from server
  //    String line = client.readStringUntil('\r');
  //    Serial.println(line);
  //
  //    Serial.println("closing connection");
  //    client.stop();
  //
  //    Serial.println("wait 5 sec...");
  //    delay(5000);
  if (authentificateCard()) {
    openDoor();
  }
  //  else {
  //    loopCounter++;
  //    if (loopCounter > 7200){ // 1 hour
  //      // send live update with mqtt
  //      loginToServer();
  //    }
  //    // schedule list update
  //    updateUserList();
  //    delay(500);
  //  }

  delay(1000);
}

