 
#include <LiquidCrystal_I2C.h>//LCD i2c display library
#include <SPI.h>//SPI library
#include <MFRC522.h>//RFID library


//pins and variables
#define RST_PIN 9
#define SS_PIN  10
byte readCard[4];
byte a = 0;

//"lcd" This is the name of the object created from the LiquidCrystal_I2C class
//"0x27" This is the I2C address of the LCD display
//"16,2" These numbers indicate that the display has 16 columns and 2 rows 
LiquidCrystal_I2C lcd(0x27, 16, 2);
//"mfrc522" This is the name of the object created to interact with the RFID reader
//"SS_PIN and RST_PIN" These are Arduino pins connected to the RFID reader
MFRC522 mfrc522(SS_PIN, RST_PIN);


void setup() {
  Serial.begin(9600);
  lcd.init();//initialize the display
  lcd.backlight();//initialize the display
  while (!Serial);
  SPI.begin();//initializes the SPI (Serial Peripheral Interface) communication protocol on the Arduino
  mfrc522.PCD_Init();//initialize the RFID
  delay(4);//delay
  mfrc522.PCD_DumpVersionToSerial();
  lcd.setCursor(0, 0);//Set the cursors on the screen to place the letters
  lcd.print("PONGA SU TARJETA");//Draw on the screen
  lcd.setCursor(4, 1);//Set the cursors on the screen to place the letters
  lcd.print("PORFAVOR");//Draw on the screen
}


void loop() {
  //This part of the code detects whether a new RFID card is present, reads its UID (Unique Identifier), and displays a message on both the LCD and the Serial Monitor. 
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }

  lcd.clear();//clear the screen
  lcd.setCursor(0, 0);
  lcd.print("Escaneando UID");
  a = 0;
  Serial.println(F("PICC's UID Escaneado:"));//write to the Arduino Serial Monitor
  //
  for ( uint8_t i = 0; i < 4; i++) {  
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i], HEX);
    Serial.print(" ");
    lcd.setCursor(a, 1);
    lcd.print(readCard[i], HEX);
    lcd.print(" ");
    delay(700);
    a += 3;
  }
  delay(4000);
  lcd.clear();
  lcd.print("PONGA SU TARJETA");
  lcd.setCursor(4, 1);
  lcd.print("PORFAVOR");
  Serial.println("");
  mfrc522.PICC_HaltA();
  return 1;

}
