/*
This is code for LCD1602 Display with I2C module
 * watch the video for this code https://youtu.be/q9YC_GVHy5A
 
 * Permission granted to share this code given that this
 * note is kept with the code.
 * Disclaimer: this code is "AS IS" and for educational purpose only.
 * This library is based on work done by DFROBOT (www.dfrobot.com).
 */
/*
 *  This code has been modefied from the Arduino library
 *  Updated by Ahmad Nejrabi on Jan 20, 2018 at 11:09
 *  in Ajax, Ontario, Canada
 *  for Robojax.com
 *  
 *  This is code for LCD1602 Display with I2C module
 *  which can display text on the screen.
*/

#include <dht.h>  // dht 11 library
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>  // lcd library

dht DHT;

#define ledPinR 12 // led pin number 12 for rain led output
#define ledPinSW 4 // led pin number 4 for soil sensor good output
#define ledPinSD 2 // for soil sensor dry output
#define DHT11_PIN 7

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

int data = 0;

void setup() {
    pinMode(A0, INPUT); // set up analog pin 0 to be input for Rain sensor
    pinMode(A2, INPUT); // set up analog pin 2 to be input for Soil-moisture sensor
    
    pinMode(ledPinR, OUTPUT);
    digitalWrite(ledPinR, LOW);
    
    pinMode(ledPinSW, OUTPUT);
    digitalWrite(ledPinSW, LOW);
    
    pinMode(ledPinSD, OUTPUT);
    digitalWrite(ledPinSD, LOW);
    
    Serial.begin(9600);

    // Robojax code for LCD with I2C
    // initialize the LCD, 
    lcd.begin();

    // Turn on the blacklightmessage.
    lcd.backlight();
    // Robojax code for LCD with I2C
}

void loop() {
    // all the technical data will be displayed on the lcd monitor. Detailed data will be displayed on bluetooth device
    // dht part (displaying them on the lcd. Later to be displayed on the bluetooth device as well)
    int chk = DHT.read11(DHT11_PIN);
    lcd.clear();
    lcd.print("T: ");
    lcd.print(DHT.temperature);
    lcd.print(" H: ");
    lcd.print(DHT.humidity);

    //rain sensor part
    // read the input on analog pin A0;
    int rainSensorValue = analogRead(A0);
    //print out the value I read;
    lcd.setCursor(0,1);
    lcd.print("RV: ");
    lcd.print(rainSensorValue);

    // soil moisture sensor part
    int soilSensorValue = analogRead(A2); // take a sample
    lcd.print(" S: ");
    lcd.print(soilSensorValue);
    
    // sending the data to bluetooth
    Serial.print("Temperature: ");
    Serial.println(DHT.temperature);
    Serial.print("Humidity: ");
    Serial.println(DHT.humidity);
    
    if(rainSensorValue<700) {
        Serial.println("It has rained! :)");
        digitalWrite(ledPinR, HIGH);
    }
    else {
        Serial.println("It's not raining :(");
        digitalWrite(ledPinR, LOW);
    }
    
    if(soilSensorValue >= 800) {
      Serial.println("Soil is DRY. Please water the plants.");
      digitalWrite(ledPinSD, HIGH);
      digitalWrite(ledPinSW, LOW);
    }
    else if(soilSensorValue >= 500) {
      Serial.println("Soil is WET. No need to water the plants now.");
      digitalWrite(ledPinSW, HIGH);
      digitalWrite(ledPinSD, LOW);
    }
    else {
      Serial.println("Soil is in HIGH MOISTURE. No need to water the plants now.");
      digitalWrite(ledPinSW, HIGH);
      digitalWrite(ledPinSD, LOW);
    }
    delay(500);
    Serial.println("--------------$$$$$$$$$$$$---------------");
}
