
#include <Servo.h>//Servo library
#include <LiquidCrystal_I2C.h>//Lcd i2c display library
#include <SPI.h>//SPI library
#include <MFRC522.h>//RFID library


//Define pins and variables
#define SS_PIN 10
#define RST_PIN 9
String UID = "BA A3 C5 B0";//It's the UID that has access.
byte lock = 0;
#define red 2
#define green 7

//Creates an object named servo using the Servo library in Arduino
Servo servo;
//"lcd" This is the name of the object created from the LiquidCrystal_I2C class
//"0x27" This is the I2C address of the LCD display
//"16,2" These numbers indicate that the display has 16 columns and 2 rows 
LiquidCrystal_I2C lcd(0x27, 16, 2);
//"mfrc522" This is the name of the object created to interact with the RFID reader
//"SS_PIN and RST_PIN" These are Arduino pins connected to the RFID reader
MFRC522 rfid(SS_PIN, RST_PIN);


void setup() {
  Serial.begin(9600);
  servo.write(90);//initial position of the servo
  lcd.init();//initialize the display
  lcd.backlight();//initialize the display
  servo.attach(3);//initialize the servo
  SPI.begin();//initializes the SPI (Serial Peripheral Interface) communication protocol on the Arduino
  rfid.PCD_Init();//initialize the RFID
  pinMode(red, OUTPUT);//declare the red LED
  pinMode(green, OUTPUT);//declare the green LED
}

void loop() {
  digitalWrite(red, HIGH);//turn the LED on (HIGH is the voltage level)                     //
  digitalWrite(green, LOW);//turn the LED off by making the voltage LOW 
  lcd.setCursor(4, 0);//Set the cursors on the screen to place the letters
  lcd.print("BIENVENIDO");//Draw on the screen
  lcd.setCursor(0, 1);//Set the cursors on the screen to place the letters.
  lcd.print("PONGA SU TARJETA");//Draw on the screen

  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  
  lcd.clear();//clear the screen
  lcd.setCursor(0, 0);
  lcd.print("ESCANEANDO");
  Serial.print("NUID TAG IS :");//write to the Arduino Serial Monitor
  String ID = "";
  //This part of code reads an RFID card’s unique identifier (UID), processes it, and displays it on an LCD screen. 
  for (byte i = 0; i < rfid.uid.size; i++) {
    lcd.print(".");
    ID.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(rfid.uid.uidByte[i], HEX));
    delay(300);
  }
  ID.toUpperCase();

  if (ID.substring(1) == UID) {
    servo.write(-90);//the servo rotates 90 degrees.
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PUERTA ABIERTA");
    digitalWrite(green, HIGH);                      
    digitalWrite(red, LOW);
    delay(4000); //add a delay so that the barrier does not hit the car
    lcd.clear();
    servo.write(100); //the servo returns to the initial position
    digitalWrite(red, HIGH);                      
    digitalWrite(green, LOW);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACCESO DENEGADO");
    delay(1500);
    lcd.clear();
  }
}
