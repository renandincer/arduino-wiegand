# arduino-wiegand
A very simple Arduino Library to read data from HID RFID readers.

## Usage
    #include <ArduinoWiegand.h>
    ArduinoWiegand a;
    void setup() {
        Serial.begin(9600);  
        a.begin();
    }
    void loop() {
        if(a.read())
        {
            Serial.print(a.getFacilityCode());
            Serial.print(a.getCardCode()); 
        }
    }

This prints the facility code and the card code.

## Todo
[ ] Tag Checking (Defining known tags and checking for them)