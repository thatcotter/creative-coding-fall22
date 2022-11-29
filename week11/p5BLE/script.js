
// The serviceUuid must match the serviceUuid of the device you would like to connect
// https://www.uuidgenerator.net/
const serviceUuid = "5db0217f-6715-4e63-b3d8-656d050233d6";
let myBLE;
let isConnected = false;

let buttonCharacteristic;
let buttonValue;

let potCharacteristic;
let potValue;

function setup() {
  // Create a p5ble class
  myBLE = new p5ble();

  createCanvas(200, 200);
  textSize(20);
  textAlign(CENTER, CENTER);

  // Create a 'Connect' button
  const connectButton = createButton('Connect')
  connectButton.mousePressed(connectToBle);

  // Create a 'Disconnect' button
  const disconnectButton = createButton('Disconnect')
  disconnectButton.mousePressed(disconnectToBle);
}

function connectToBle() {
  // Connect to a device by passing the service UUID
  myBLE.connect(serviceUuid, gotCharacteristics);
}

function disconnectToBle() {
  // Disonnect to the device
  myBLE.disconnect();
  // Check if myBLE is connected
  isConnected = myBLE.isConnected();
}

function onDisconnected() {
  console.log('Device got disconnected.');
  isConnected = false;
}

// A function that will be called once got characteristics
function gotCharacteristics(error, characteristics) {
  if (error) console.log('error: ', error);
  console.log('characteristics: ', characteristics);

  // Check if myBLE is connected
  isConnected = myBLE.isConnected();

  // Add a event handler when the device is disconnected
  myBLE.onDisconnected(onDisconnected)

  // assign characteristic and value read callback
  // buttonCharacteristic = characteristics[0];
  for (let i = 0; i < characteristics.length; i++) {
    if(characteristics[i].uuid === "71d1847d-412d-4554-aef0-7cf5210d9504") {
      buttonCharacteristic = characteristics[i]
    } else if (characteristics[i].uuid === "1e1011ce-a6a9-4bde-b4dc-ed561745c55a") {
      potCharacteristic = characteristics[i]
    }
  }

  // Read the value of the first characteristic
  myBLE.read(buttonCharacteristic, gotButtonValue);
  myBLE.read(potCharacteristic, gotPotValue);
}

// A function that will be called once got values
function gotButtonValue(error, value) {
  if (error) console.log('error: ', error);
  console.log('button value: ', value);
  buttonValue = value;
  // After getting a value, call p5ble.read() again to get the value again
  myBLE.read(buttonCharacteristic, gotButtonValue);
  // You can also pass in the dataType
  // Options: 'unit8', 'uint16', 'uint32', 'int8', 'int16', 'int32', 'float32', 'float64', 'string'
  // myBLE.read(myCharacteristic, 'string', gotValue);
}

function gotPotValue(error, value) {
  if (error) console.log('error: ', error)
  console.log('pot value: ', value)
  potValue = value
  myBLE.read(potCharacteristic, gotPotValue)
}

function draw() {
  if (isConnected) {
    background(0, 255, 0);
    text('Connected!', 100, 10);
  } else {
    background(255, 0, 0);
    text('Disconnected :/', 100, 10);
  }

  if(buttonValue || buttonValue === 0){
    


    if (buttonValue == 1) {
      ellipse(100, 100, potValue, potValue)
    } else {
      rectMode(CENTER)
      rect(100, 100, potValue, potValue)
    }

    // Write value on the canvas
    text(buttonValue, 100, 100);
  }


}