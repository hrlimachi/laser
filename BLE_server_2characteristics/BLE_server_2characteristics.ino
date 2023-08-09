#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Initialize all pointers
BLEServer* pServer = NULL;                        // Pointer to the server
BLECharacteristic* pCharacteristic_1 = NULL;      // Pointer to Characteristic 1
BLEDescriptor *pDescr_1;                          // Pointer to Descriptor of Characteristic 1
BLE2902* pBLE2902_1;                              // Pointer to BLE2902 of Characteristic 1
// variables to virify laser
bool readyFlag = false;
bool waitFlag = false;
bool funFlag = false;
int readyCount = 0;

// Some variables to keep track on device connected
bool deviceConnected = false;
bool oldDeviceConnected = false;

// Variable that will continuously be increased and written to the client
uint32_t value = 1;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
// UUIDs used in this example:
#define SERVICE_UUID          "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID_1 "beb5483e-36e1-4688-b7f5-ea07361b26a8"
//#define CHARACTERISTIC_UUID_2 "1c95d5e3-d8f7-413a-bf3d-7a2e5d7be87e"

// variable used by buttonFun
int pinButton = 19;
bool statPrevBut = LOW;
unsigned long millisTime = 0;

#define DEEP_SLEEP_TIME 1
//sleep
void goToDeepSleep(){
  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME * 60 * 1000000);
  esp_deep_sleep_start();
}
// Button with millis function
bool botonFun(int pin, bool* statePreviousButton, unsigned long* lastTime ) {
  bool stateCurrentButton = digitalRead(pin);
  const int delayMillis = 200;
  bool flag = false;
  if (*statePreviousButton != stateCurrentButton) {
    if (HIGH == stateCurrentButton) {
      if ((millis() - *lastTime) > delayMillis) {
        //inicio
        flag = true;
        //final
        *statePreviousButton = stateCurrentButton;
        *lastTime = millis();
      }
    }
    else {
      *statePreviousButton = LOW;
    }
  }
  return flag;
}


// Callback function that is called whenever a client is connected or disconnected
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("un dispositivo conectado");
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      Serial.println("un dispositivo desconectado");
      deviceConnected = false;
    }
};

void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic_1 = pService->createCharacteristic(
                        CHARACTERISTIC_UUID_1,
                        BLECharacteristic::PROPERTY_NOTIFY
                      );

  // Create a BLE Descriptor
  pDescr_1 = new BLEDescriptor((uint16_t)0x2901);
  pDescr_1->setValue("A very interesting variable");
  pCharacteristic_1->addDescriptor(pDescr_1);

  // Add the BLE2902 Descriptor because we are using "PROPERTY_NOTIFY"
//  pBLE2902_1 = new BLE2902();
//  pBLE2902_1->setNotifications(true);
//  pCharacteristic_1->addDescriptor(pBLE2902_1);

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
  pinMode(pinButton, INPUT);
}

void loop() {
  // notify changed value
  if (deviceConnected) {
    if (readyFlag) {
      readyCount = !digitalRead(pinButton) ? readyCount +1 : 0;
      Serial.println("readyCount esta en: "+ String(readyCount));
      funFlag = readyCount >=50;
      readyFlag = !funFlag;
      delay(100);
    }
    else if (funFlag) {
      if (botonFun(pinButton, &statPrevBut, &millisTime)) {
        Serial.println("enviando numero");
        pCharacteristic_1->setValue(value);
        pCharacteristic_1->notify();
        waitFlag = true;
        funFlag=false;
      }
    }
    else if(waitFlag){
      delay(100);
      goToDeepSleep();
    }
  }

  if (!deviceConnected && oldDeviceConnected) {
    delay(1000); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // Connecting
  if (deviceConnected && !oldDeviceConnected) {
    readyFlag = true;
    funFlag = false;
    oldDeviceConnected = deviceConnected;
  }
}
