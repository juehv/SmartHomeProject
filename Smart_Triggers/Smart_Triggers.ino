#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#define LED_INFO 13

void setup() {
  sw_init();
  log_init();

  if (sw_read() == false) {
    log_verbose("No switch pressed..");
    pm_deep_sleep_def();
  } else {
    // key pressed; boot up and push values.

    // Led Setup
    pinMode(LED_INFO, OUTPUT);
    digitalWrite(LED_INFO, HIGH);
    
    wifi_init();
    mqtt_init();
    mqtt_send_values();
  }
  pm_reset();
}

void loop() {
  // Since we do a loop in the setup we start with energy saving stuff
  pm_light_sleep();
  pm_schedule();
  delay (100);

  if (sw_read() == true) {
    pm_reset();
    // push data
    mqtt_send_values();
  }
}




