#define WIFI_SSID "JuLi_HomeNet"
#define WIFI_PW "derkonsumgehtumher"
static const IPAddress WIFI_IP(192, 168, 100, 27);
static const IPAddress WIFI_NET(255, 255, 255, 0);
static const IPAddress WIFI_GW(192, 168, 100, 99);

#define MQTT_SERVER "192.168.100.44"
#define MQTT_PORT 1883
//#define MQTT_USR "your_username"
//#define MQTT_PW "your_password"
#define MQTT_CLIENT_NAME "ESP8266Client"

#define MQTT_TOPIC_SW1 "home/esp/sw/1"
#define MQTT_TOPIC_SW2 "home/esp/sw/2"
#define MQTT_TOPIC_SW3 "home/esp/sw/3"
#define MQTT_TOPIC_SW4 "home/esp/sw/4"
#define MQTT_TOPIC_SW5 "home/esp/sw/5"

static WiFiClient espWiFiClient;
static PubSubClient client(espWiFiClient);

static inline void wifi_init() {
  WiFi.config(WIFI_IP, WIFI_GW, WIFI_NET);
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
    if (client.connect("ESP8266Client")) {} else {
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

static inline void mqtt_send_values() {
  if (!client.connected()) {
    mqtt_reconnect();
  }

  // send switch values
  if (sw_1_update()) {
    client.publish(MQTT_TOPIC_SW1, sw_convert_value(sw_1_state()), false);
    log_debug("Push State 1: %d", sw_1_state());
  }
  if (sw_2_update()) {
    client.publish(MQTT_TOPIC_SW2, sw_convert_value(sw_2_state()), false);
    log_verbose("Push State 1: %d", sw_2_state());
  }
  if (sw_3_update()) {
    client.publish(MQTT_TOPIC_SW3, sw_convert_value(sw_3_state()), false);
    log_verbose("Push State 1: %d", sw_3_state());
  }
  if (sw_4_update()) {
    client.publish(MQTT_TOPIC_SW4, sw_convert_value(sw_4_state()), false);
    log_verbose("Push State 1: %d", sw_4_state());
  }
  if (sw_5_update()) {
    client.publish(MQTT_TOPIC_SW5, sw_convert_value(sw_5_state()), false);
    log_verbose("Push State 1: %d", sw_5_state());
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
