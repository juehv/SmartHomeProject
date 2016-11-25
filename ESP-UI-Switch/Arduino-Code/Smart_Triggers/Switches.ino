// PIN Setup; comment out if you don't use it
#define SW1 4
#define SW2 5
#define SW3 12
//#define SW4 13
#define SW5 14
// static configuration
#define SW_LONG (uint8_t)2
#define SW_NOT (uint8_t)1
#define SW_PRESSED (uint8_t)0
#define SW_LONG_DELAY (uint16_t)1800 //ms

static bool sw_update_needed = false;

#ifdef SW1
static uint8_t state_sw_1 = SW_NOT;
static uint32_t mills_sw_1;
static uint8_t update_sw_1;
#endif
#ifdef SW2
static uint8_t state_sw_2 = SW_NOT;
static uint32_t mills_sw_2;
static uint8_t update_sw_2;
#endif
#ifdef SW3
static uint8_t state_sw_3 = SW_NOT;
static uint32_t mills_sw_3;
static uint8_t update_sw_3;
#endif
#ifdef SW4
static uint8_t state_sw_4 = SW_NOT;
static uint32_t mills_sw_4;
static uint8_t update_sw_4;
#endif
#ifdef SW5
static uint8_t state_sw_5 = SW_NOT;
static uint32_t mills_sw_5;
static uint8_t update_sw_5;
#endif

void sw_init() {
  // Switch Setup
#ifdef SW1
  pinMode(SW1, INPUT);
#endif
#ifdef SW2
  pinMode(SW2, INPUT);
#endif
#ifdef SW3
  pinMode(SW3, INPUT);
#endif
#ifdef SW4
  pinMode(SW4, INPUT);
#endif
#ifdef SW5
  pinMode(SW5, INPUT);
#endif
}

bool sw_calculate_switch_status(uint8_t* value, uint8_t* state, uint32_t* state_time) {
  log_debug("read: %d, old_state: %d, state_time: %d", *value, *state, *state_time);
  if (*value != *state && (*state != SW_LONG || *value == SW_NOT)) {
    // pressed -> unpressed
    // unpressed -> pressed
    // long -> unpressed
    *state = *value;
    if (*state == SW_PRESSED) {
      *state_time = millis();
    }
    log_verbose("--> UPDATE state");
    return true;
    // code for long press registration is not needed
    //  } else if (*value == SW_PRESSED && *state == SW_PRESSED
    //             && (millis() - *state_time) > SW_LONG_DELAY) {
    //    // pressed -> long
    //    // happens just once because of *state == SW_PRESSED
    //    *state = SW_LONG;
    //    log_verbose("--> UPDATE to long");
    //    return true;
  }
  return false;
}

static inline bool sw_read() {
  bool has_interaction = false;
  uint8_t read_value;
#ifdef SW1
  read_value = digitalRead(SW1);
  update_sw_1 = sw_calculate_switch_status(&read_value, &state_sw_1, &mills_sw_1);
  sw_update_needed |= update_sw_1;
  has_interaction |= (read_value == SW_PRESSED) || update_sw_1;
  log_verbose("Switch 1: %d, %d, %T", state_sw_1, mills_sw_1, update_sw_1);
#endif

#ifdef SW2
  read_value = digitalRead(SW2);
  update_sw_2 = sw_calculate_switch_status(&read_value, &state_sw_2, &mills_sw_2);
  sw_update_needed |= update_sw_2;
  has_interaction |= (read_value == SW_PRESSED) || update_sw_2;
  log_verbose("Switch 2: %d, %d, %T", state_sw_2, mills_sw_2, update_sw_2);
#endif

#ifdef SW3
  read_value = digitalRead(SW3);
  update_sw_3 = sw_calculate_switch_status(&read_value, &state_sw_3, &mills_sw_3);
  sw_update_needed |= update_sw_3;
  has_interaction |= (read_value == SW_PRESSED) || update_sw_3;
  log_verbose("Switch 3: %d, %d, %T", state_sw_3, mills_sw_3, update_sw_3);
#endif

#ifdef SW4
  read_value = digitalRead(SW4);
  update_sw_4 = sw_calculate_switch_status(&read_value, &state_sw_4, &mills_sw_4);
  sw_update_needed |= update_sw_4;
  has_interaction |= (read_value == SW_PRESSED) || update_sw_4;
  log_verbose("Switch 4: %d, %d, %T", state_sw_4, mills_sw_4, update_sw_4);
#endif

#ifdef SW5
  read_value = digitalRead(SW5);
  update_sw_5 = sw_calculate_switch_status(&read_value, &state_sw_5, &mills_sw_5);
  sw_update_needed |= update_sw_5;
  has_interaction |= (read_value == SW_PRESSED) || update_sw_5;
  log_verbose("Switch 5: %d, %d, %T", state_sw_5, mills_sw_5, update_sw_5);
#endif

  return has_interaction;
}

static inline bool sw_1_update() {
#ifdef SW1
  return update_sw_1;
#else
  return false;
#endif
}

static inline bool sw_2_update() {
#ifdef SW2
  return update_sw_2;
#else
  return false;
#endif
}

static inline bool sw_3_update() {
#ifdef SW3
  return update_sw_3;
#else
  return false;
#endif
}

static inline bool sw_4_update() {
#ifdef SW4
  return update_sw_4;
#else
  return false;
#endif
}

static inline bool sw_5_update() {
#ifdef SW5
  return update_sw_5;
#else
  return false;
#endif
}

static inline uint8_t* sw_1_state() {
#ifdef SW1
  return &state_sw_1;
#else
  return 0;
#endif
}

static inline uint8_t* sw_2_state() {
#ifdef SW2
  return &state_sw_2;
#else
  return 0;
#endif
}

static inline uint8_t* sw_3_state() {
#ifdef SW3
  return &state_sw_3;
#else
  return 0;
#endif
}

static inline uint8_t* sw_4_state() {
#ifdef SW4
  return &state_sw_4;
#else
  return 0;
#endif
}

static inline uint8_t* sw_5_state() {
#ifdef SW5
  return &state_sw_5;
#else
  return 0;
#endif
}

static inline char* sw_convert_value(uint8_t* value) {
  if (*value == 0) {
    return "ON";
  } else if (*value == 2) {
    return "LONG";
  } else {
    return "OFF";
  }
}

