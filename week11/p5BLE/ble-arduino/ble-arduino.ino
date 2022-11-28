#include <ArduinoBLE.h>

const int buttonPin = 4; // set buttonPin to digital pin 4

BLEService nanoService("add your service uuid here"); // create service

// create button characteristic and allow remote device to get notifications
BLEIntCharacteristic buttonCharacteristic("add your characteristic uuid here", BLERead | BLENotify);

int sensorValue = 255;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(ledPin, OUTPUT); // use the LED as an output
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

  // add the service
  BLE.addService(nanoService);

  buttonCharacteristic.writeValue(0);

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

    delay(1);
  }

}
