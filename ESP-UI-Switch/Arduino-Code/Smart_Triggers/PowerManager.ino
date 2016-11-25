extern "C" {
#include "user_interface.h"
}

#define PM_WAKE_TIMEOUT 10000 // in ms
#define PM_TIMEOUT_TIME 150 // in ms

uint32_t last_reset = millis();

static inline void pm_deep_sleep(uint8_t time_in_ms) {
  log_debug("start deep sleep for %d ms", time_in_ms);
  ESP.deepSleep(time_in_ms * 1000);
  delay(100);
}

static inline void pm_deep_sleep_def() {
  log_debug("start deep sleep for %d ms", PM_TIMEOUT_TIME);
  ESP.deepSleep(PM_TIMEOUT_TIME * 1000);
  delay(100);
}

static inline void pm_light_sleep() {
  wifi_set_sleep_type (LIGHT_SLEEP_T );
  log_debug("start light sleep");
}

static inline void pm_schedule() {
  log_verbose ("Now: %d - Last Reset: %d > %d ?", millis(), last_reset, PM_WAKE_TIMEOUT);
  if ((millis() - last_reset) > PM_WAKE_TIMEOUT) {
    pm_deep_sleep(PM_TIMEOUT_TIME);
  }
}

static inline void pm_reset() {
  last_reset = millis();
}

