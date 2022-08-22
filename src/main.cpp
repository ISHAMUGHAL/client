#include <BLEDevice.h>
#include <iostream> 
#include <string> 
#include <arduino.h> 
using namespace std;

static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b"); 
static BLEUUID    charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8"); 

string My_BLE_Address = "0c:b8:15:c3:1b:fa";
static BLERemoteCharacteristic* pRemoteCharacteristic;


BLEScan* pBLEScan; 

BLEScanResults foundDevices;


static BLEAddress *Server_BLE_Address;

string Scaned_BLE_Address;


bool paired = false; 


 


bool connectToserver (BLEAddress pAddress){

    

    BLEClient*  pClient  = BLEDevice::createClient();

    Serial.println(" - Created client");


    pClient->connect(pAddress);

    Serial.println(" - Connected to fitnessband");



    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);

    if (pRemoteService != nullptr)

    {

      Serial.println("Found our service");

      return true;

    }

    else

    return false;


 

pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);

if (pRemoteCharacteristic != nullptr)
 Serial.println(" - Found our characteristic");


return true;

}


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks 

{

    void onResult(BLEAdvertisedDevice advertisedDevice) {

      Serial.printf("Scan Result: %s \n", advertisedDevice.toString().c_str());

      Server_BLE_Address = new BLEAddress(advertisedDevice.getAddress());

      

      Scaned_BLE_Address = Server_BLE_Address->toString().c_str();

      

    }

};



void setup() {

    Serial.begin(115200); 

    Serial.println("ESP32 BLE Server program"); 


    BLEDevice::init("");

    pBLEScan = BLEDevice::getScan(); 

    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());

    pBLEScan->setActiveScan(true);


    pinMode (13,OUTPUT); 

}


void loop() {


  foundDevices = pBLEScan->start(3); 



  while (foundDevices.getCount() >= 1)

  {

    if (Scaned_BLE_Address == My_BLE_Address && paired == true)

    {

      Serial.println("connecting to Server as client");

       if (connectToserver(*Server_BLE_Address))

      {
     Serial.println("BLE Server into the range");

      paired = true;

      Serial.println("LED_ON");

      digitalWrite (13,HIGH);

      break;

      }

      else

      {

      Serial.println("Pairing failed");

      break;

      }

    }

    

    if (Scaned_BLE_Address == My_BLE_Address && paired ==false)

    {

      Serial.println("BLE Server out of range");

      paired = false;

      Serial.println("LED_OFF");

      digitalWrite (13,LOW);

      ESP.restart();

      break;

    }

    else

    {

    Serial.println("error");

    break;

    }

  } 

}

