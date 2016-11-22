void setup() {
  log_init();
  pin_setup();
}

void loop() {
  mqtt_read();
  temp_read();
  air_read();
  temp_automatic_loop();
  air_automatic_loop();
  mqtt_write();
  
}

void pin_setup(){}
void mqtt_read(){}
void temp_read(){}
void air_read(){}
void temp_automatic_loop(){}
void air_automatic_loop(){}
void mqtt_write(){}
