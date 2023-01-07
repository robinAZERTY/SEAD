/*
#include <Adafruit_BMP085.h>
#include "MPU9250.h"
#include <TinyGPSPlus.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire,0x68);
Adafruit_BMP085 bmp;
TinyGPSPlus gps;


BluetoothSerial SerialBT;

int MPU_connection_test(bool explicit_print)
{
  // start communication with IMU 
  int status;
  status = IMU.begin();
  if (status < 0 ) {
    if (explicit_print)
    {
      Serial.println("IMU initialization unsuccessful");
      Serial.println("Check IMU wiring or try cycling power");
      Serial.print("Status: ");
      Serial.println(status);
    }
    return 0;
  }
  else
  {
    if (explicit_print)
    {
      Serial.println("IMU initialization successful!");
    }
    return 1;
  }  
}

int BMP_connection_test(bool explicit_print)
{
  if (!bmp.begin()) {
    if (explicit_print)
    {
      Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    }
    return 0;
  }
  else
  {
    if (explicit_print)
    {
      Serial.println("BMP085 sensor found!");
    }
    return 1;
  }
}



int GPS_connection_test(bool explicit_print)
{
 Serial2.begin(9600, SERIAL_8N1, 16, 17);
 while (!Serial2){};
  return 1;
}

void updateSerial(){
  while (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  while (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  while (Serial.available())  {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (Serial2.available())  {
    char c = Serial2.read();
    Serial.write(c);//Forward what Software Serial received to Serial Port
    SerialBT.write(c);
  }
}

void setup() {
  // serial to display data
  Serial.begin(115200);
  while(!Serial) {}
  SerialBT.begin("ESP32test"); //Bluetooth device name
  while(!SerialBT.connected()) {}
  Serial.println();
  Serial.println("The device started, now you can pair it with bluetooth!");
  Serial.println("Starting connection test");
  Serial.println("Press any key to start");
  Serial.println("Starting MPU9250 test");
  MPU_connection_test(true);

  Serial.println("Starting BMP180 test");
  BMP_connection_test(true);
  GPS_connection_test(true);
}

void loop() {
updateSerial();
delay(100);
}

*/

/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <Arduino.h>
#include <TinyGPSPlus.h>
/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"




TinyGPSPlus gps;
void setup() {
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("Long name works now");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                       BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setValue("Hello World says Neil");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
  int count=0;
  while(true)
  {
    if(Serial2.available())//Check if Software Serial received any data
    {
    //check if the client has disconnected
    if (pServer->getConnectedCount() == 0) {
      Serial.println("Client disconnected");
      break;
    }
    else{
    String str="";
    while (Serial2.available())  
    {
    str += Serial2.read();
    }
    Serial.println(str);//Forward what Software Serial received to Serial Port
// convert str to char array
    char char_array[str.length() + 1];
    strcpy(char_array, str.c_str());
    pCharacteristic->setValue(count);
    count++;
    pCharacteristic->notify();
    delay(1000);
  }
    }
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}