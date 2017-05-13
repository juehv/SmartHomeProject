#define WIFI_SSID "JuLi_HomeNet"
#define WIFI_PW "derkonsumgehtumher"
static const IPAddress WIFI_IP(192, 168, 100, 27);
static const IPAddress WIFI_DNS(192, 168, 100, 99);
static const IPAddress WIFI_NET(255, 255, 255, 0);
static const IPAddress WIFI_GW(192, 168, 100, 99);

#define MQTT_SERVER "192.168.100.39"
#define MQTT_PORT 1883
//#define MQTT_USR "your_username"
//#define MQTT_PW "your_password"
#define MQTT_CLIENT_NAME "EspMotionSensor"

#define MQTT_TOPIC_MOTION_ALL   "jhit/shp/motion/recognition/all"
#define MQTT_TOPIC_MOTION_R   "jhit/shp/motion/recognition/right"
#define MQTT_TOPIC_MOTION_L   "jhit/shp/motion/recognition/left"
#define MQTT_TOPIC_PATTERN  "jhit/shp/motion/pattern"

static WiFiClient espWiFiClient;
static PubSubClient client(espWiFiClient);

static inline void wifi_init() {
  WiFi.config(WIFI_IP, WIFI_DNS, WIFI_GW, WIFI_NET);
}

static inline void wifi_connect(){
  WiFi.begin(WIFI_SSID, WIFI_PW);

  // Wait for connection
  log_verbose("Setup WiFi connection...");
  uint16_t maxwait = 300; // 30 sec
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);

    maxwait--;
    if (maxwait == 0) {
      log_debug ("WiFi connection NOT established!");
      delay(10000); // wait 10s before shutdown
      pm_deep_sleep_def();
    }
  }
  char tmpAddr[16];
  log_convert_ip(tmpAddr, WiFi.localIP());
  log_debug ("WiFi connection established to %s with ip %s",
             WIFI_SSID, tmpAddr);
}

static inline void mqtt_init() {
  client.setServer(MQTT_SERVER, MQTT_PORT);
  log_debug ("Set MQTT server to %s:%d",
             MQTT_SERVER, MQTT_PORT);
}

// Possible values for client.state()
//#define MQTT_CONNECTION_TIMEOUT     -4
//#define MQTT_CONNECTION_LOST        -3
//#define MQTT_CONNECT_FAILED         -2
//#define MQTT_DISCONNECTED           -1
//#define MQTT_CONNECTED               0
//#define MQTT_CONNECT_BAD_PROTOCOL    1
//#define MQTT_CONNECT_BAD_CLIENT_ID   2
//#define MQTT_CONNECT_UNAVAILABLE     3
//#define MQTT_CONNECT_BAD_CREDENTIALS 4
//#define MQTT_CONNECT_UNAUTHORIZED    5

static inline void mqtt_reconnect() {
  //Loop until we're reconnected
  log_verbose("Attempting MQTT connection...");
  uint16_t maxtries = 10;
  while (!client.connected()) {
    maxtries--;
    // If you do not want to use a username and password, change next line to
    if (client.connect(MQTT_CLIENT_NAME)) {} else {
      //if (!client.connect(MQTT_CLIENT_NAME, MQTT_USR, MQTT_PW)) {
      if (maxtries == 0) {
        log_debug("Failed to connect to mqtt server %s after 10 tries with state %d.", MQTT_SERVER);
        delay(10000); // wait 10s before shutdown
        pm_deep_sleep_def();
      }
      log_verbose("MQTT connection failed with state \"%d\". Try again in 5 seconds", client.state());
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  log_debug ("MQTT connection established as %s to %s:%d", MQTT_CLIENT_NAME, MQTT_SERVER, MQTT_PORT);
}

uint8_t motionHistoryAllMqtt;
uint8_t motionHistoryRightMqtt;
uint8_t motionHistoryLeftMqtt;
uint8_t patternHistoryMqtt;
static inline void mqtt_send_motion_update(uint8_t motionAll, uint8_t motionRight, uint8_t motionLeft) {
  uint8_t pattern = 0;
  if (!client.connected()) {
    mqtt_reconnect();
  }
  char buf[2];

  if (motionAll != motionHistoryAllMqtt || pattern != patternHistoryMqtt ||
      motionRight != motionHistoryRightMqtt || motionLeft != motionHistoryLeftMqtt) {
    if (!client.publish(MQTT_TOPIC_MOTION_ALL, utoa(motionAll, buf, 2), false)) {
      log_debug("Publishing failed");
    } else {
      motionHistoryAllMqtt = motionAll;
    }
    if (!client.publish(MQTT_TOPIC_MOTION_R, utoa(motionRight, buf, 2), false)) {
      log_debug("Publishing failed");
    } else {
      motionHistoryRightMqtt = motionRight;
    }
    if (!client.publish(MQTT_TOPIC_MOTION_L, utoa(motionLeft, buf, 2), false)) {
      log_debug("Publishing failed");
    } else {
      motionHistoryLeftMqtt = motionLeft;
    }

    //    if (!client.publish(MQTT_TOPIC_PATTERN, utoa(pattern, buf, 2), false)) {
    //      log_debug("Publishing failed");
    //    } else {
    //      patternHistoryMqtt = pattern;
    //    }
    //    log_debug("Push motion: %d with pattern %d", motionAll, pattern);
  } else {
    log_debug("No new value, skip push");
  }
}


//void write_sensor_to_rtc_ram() {
//  // always use 4 byte
//  // http://www.esp8266.com/viewtopic.php?f=8&t=3505
//  // http://esp8266-re.foogod.com/wiki/System_rtc_mem_read_%28IoT_RTOS_SDK_0.9.9%29
//  byte rtcStore[4];
//  system_rtc_mem_read(64, rtcStore, 4);
//  system_rtc_mem_write(64, rtcStore, 4);
//}

//#########################################################################################################
// Web update
//#########################################################################################################
/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

const char* host = "esp8266-webupdate";

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void ota_init() {
  log_debug("Setup OTA Server");

  MDNS.begin(host);

  httpUpdater.setup(&httpServer);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  IPAddress localIPAddr = WiFi.localIP();
  log_debug("HTTPUpdateServer ready! Open \"http://%d.%d.%d.%d/update\" in your browser\n",
            localIPAddr[0], localIPAddr[1], localIPAddr[2], localIPAddr[3]);
}

void ota_schedule() {
  httpServer.handleClient();
}
