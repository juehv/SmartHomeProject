#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#define PIR_1 12
#define PIR_2 13
#define PIR_3 5

// Motion Pattern
// If you look like the detector, in which direction would the motion go
#define PATTERN_NONE  0
#define PATTERN_RIGHT 1
#define PATTERN_LEFT  2

uint8_t pirVal1 = 0; //right
uint8_t pirVal2 = 0; //middle
uint8_t pirVal3 = 0; //left

uint8_t pirVal1History = 0;
uint8_t pirVal3History = 0;

uint8_t pattern = 0;
uint8_t patternHistory = 0;

unsigned long timestampStart = 0;
unsigned long timestampStop = 0;
long timestampDiff = 0;

void setup() {
  log_init();
  wifi_init();
  mqtt_init();

  //pm_reset();

  pinMode(PIR_1, INPUT);
  pinMode(PIR_2, INPUT);
  pinMode(PIR_3, INPUT);
}

void loop() {
  timestampStart = millis();
  // Since we do a loop in the setup we start with energy saving stuff
  //  pm_light_sleep();
  //  pm_schedule();
  //  delay (100);
  //
  //  if (sw_read() == true) {
  //    pm_reset();
  //    // push data
  //    mqtt_send_values();
  //  }

  // read new values
  pirVal1 = digitalRead(PIR_1);
  pirVal2 = digitalRead(PIR_2);
  pirVal3 = digitalRead(PIR_3);
  log_debug("Read PIR: %d, %d, %d", pirVal1, pirVal2, pirVal3);

  if ((pirVal1 + pirVal2 + pirVal3) > 0) {
    // detect patterns
    if (pirVal1History == 0 && pirVal1 == 1 //switch from off to on
        && pirVal3History == 1 // other side was active
       ) {
      // motion from left to right sensor --> pattern right
      pattern = PATTERN_RIGHT;
    } else if (pirVal3History == 0 && pirVal3 == 1 //switch from off to on
               && pirVal1History == 1 // other side was active
              ) {
      // motion from right to left sensor --> pattern left
      pattern = PATTERN_LEFT;
    } else if (pirVal1History == 1 && pirVal1 == 0 ||
               pirVal3History == 1 && pirVal3 == 0 ) {
      // some edge value switched from on to off --> pattern over
      pattern = PATTERN_NONE;
    } else if (patternHistory == PATTERN_NONE) {
      // no pattern found and no pattern active
      pattern = PATTERN_NONE;
    }

    // update history
    pirVal1History = pirVal1;
    pirVal3History = pirVal3;
    patternHistory = pattern;

    // send values
    mqtt_send_motion_update(1, pirVal1, pirVal3, pattern);
  } else {
    mqtt_send_motion_update(0, pirVal1, pirVal3, PATTERN_NONE);
  }


  timestampStop = millis();
  timestampDiff = timestampStop - timestampStart;
  if (timestampDiff > 0 && timestampDiff < 200) {
    delay(200 - timestampDiff);
  } else {
    delay(200);
  }
}




