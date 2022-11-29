#include <ArduinoBLE.h>

const int buttonPin = 4; // set buttonPin to digital pin 4
const int potPin = A0;

BLEService nanoService("5db0217f-6715-4e63-b3d8-656d050233d6"); // create service

// create button characteristic and allow remote device to get notifications
BLEIntCharacteristic buttonCharacteristic("71d1847d-412d-4554-aef0-7cf5210d9504", BLERead | BLENotify);
BLEIntCharacteristic potCharacteristic("1e1011ce-a6a9-4bde-b4dc-ed561745c55a", BLERead | BLENotify);

int sensorValue = 255;

void setup() {
  Serial.begin(9600);
//  while (!Serial);

//  pinMode(ledPin, OUTPUT); // use the LED as an output
  pinMode(buttonPin, INPUT); // use button pin as an input
  pinMode(potPin, INPUT);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("Arduino Nano");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(nanoService);

  // add the characteristics to the service
  nanoService.addCharacteristic(buttonCharacteristic);
  nanoService.addCharacteristic(potCharacteristic);

  // add the service
  BLE.addService(nanoService);

  buttonCharacteristic.writeValue(0);
  potCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // poll for BLE events
  BLE.poll();

  if (millis() % 100 == 0) {
    sensorValue = digitalRead(buttonPin);
    Serial.println(sensorValue);
    buttonCharacteristic.writeValue(sensorValue);

    int potValue = analogRead(potPin);
    Serial.println(potValue);
    potCharacteristic.writeValue(potValue);

    delay(1);
  }

}
