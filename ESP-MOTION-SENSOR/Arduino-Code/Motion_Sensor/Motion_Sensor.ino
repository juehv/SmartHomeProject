#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#define OTA 0
#define PIR_1 12
#define PIR_2 13
#define COUNTER_TH_LIGHT 8
#define COUNTER_TH_DEEP 16

uint8_t pirVal1 = -1; //right
uint8_t pirVal2 = -1; //left
uint8_t pirAll = 0;

uint8_t pirVal1History = 0;
uint8_t pirVal2History = 0;

uint8_t noChangeCounter = 0;

bool connectionIsInit = false;
bool comeFromDeepSleep = true;

static inline void connection_init() {
  if (connectionIsInit) {
    return;
  }

  wifi_connect();
  mqtt_init();
  connectionIsInit = true;
}

static inline uint8_t read_sensor_values() {
  // PIR
  pirVal1History = pirVal1;
  pirVal2History = pirVal2;
  pirVal1 = digitalRead(PIR_1);
  pirVal2 = digitalRead(PIR_2);
  log_debug("Read PIR: %d, %d", pirVal1, pirVal2);
  if (pirVal1History == pirVal1 &&
      pirVal2History == pirVal2) {
    noChangeCounter++;
  } else {
    noChangeCounter = 0;
  }
  log_debug("No change since %d cycles", noChangeCounter);

  return pirVal1 + pirVal2;
}

void setup() {
  pinMode(PIR_1, INPUT);
  pinMode(PIR_2, INPUT);
  pinMode(OTA, INPUT);

  log_init();
  wifi_init();
}

void loop() {
  pirAll = read_sensor_values();

  // OTA Check
  if (digitalRead(OTA) == LOW) {
    // start Update mode
    if (!connectionIsInit) {
      wifi_connect();
    }
    ota_init();

    while (true) {
      ota_schedule();
    }
  }

  // check if sth changes
  if ((!comeFromDeepSleep && noChangeCounter == 0) ||
      (comeFromDeepSleep && pirAll > 0)) {
    // need update
    connection_init();
    mqtt_send_motion_update(pirAll > 1 ? 1 : pirAll,
                            pirVal1, pirVal2);
  } else if (comeFromDeepSleep) {
    // fast forward back to deepsleep
    pm_deep_sleep(250);
  }
  comeFromDeepSleep = false;

  // Power Management
  if (noChangeCounter > COUNTER_TH_LIGHT) {
    // --> turn off WiFi to save powers
    pm_light_sleep();
    connectionIsInit = false;
  }

  if (pirAll == 0 && noChangeCounter > COUNTER_TH_DEEP)
    // --> deep sleep
    pm_deep_sleep(250);

  delay(250);
}




