#define LOG_LEVEL_NOOUTPUT 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL_VERBOSE 4

// Configuration
//#define LOG_LEVEL LOG_LEVEL_NOOUTPUT
#define LOG_LEVEL LOG_LEVEL_VERBOSE
#define LOG_BAUD_RATE 115200

#define CR "\r\n"

static inline void log_init() {
#if (LOG_LEVEL > LOG_LEVEL_NOOUTPUT)
  Serial.begin(LOG_BAUD_RATE);
#endif
}


static inline void log_error(char* msg, ...) {
#if (LOG_LEVEL >= LOG_LEVEL_ERROR)
  va_list args;
  va_start(args, msg);
  log_print(msg, args);
#endif
}



static inline void log_info(char* msg, ...) {
#if (LOG_LEVEL >= LOG_LEVEL_INFO)
  va_list args;
  va_start(args, msg);
  log_print(msg, args);
#endif
}

static inline void log_debug(char* msg, ...) {
#if (LOG_LEVEL >= LOG_LEVEL_DEBUG)
  va_list args;
  va_start(args, msg);
  log_print(msg, args);
#endif
}


static inline void log_verbose(char* msg, ...) {
#if (LOG_LEVEL >= LOG_LEVEL_VERBOSE)
  va_list args;
  va_start(args, msg);
  log_print(msg, args);
#endif
}

static inline void log_convert_ip(char* output, IPAddress input) {
#if (LOG_LEVEL > LOG_LEVEL_NOOUTPUT)
  byte oct1 = input[0];
  byte oct2 = input[1];
  byte oct3 = input[2];
  byte oct4 = input[3];
  sprintf(output, "%d.%d.%d.%d", oct1, oct2, oct3, oct4);
#endif
}

void log_print(const char *format, va_list args) {
  //
  // loop through format string
  for (; *format != 0; ++format) {
    if (*format == '%') {
      ++format;
      if (*format == '\0') break;
      if (*format == '%') {
        Serial.print(*format);
        continue;
      }
      if ( *format == 's' ) {
        register char *s = (char *)va_arg( args, int );
        Serial.print(s);
        continue;
      }
      if ( *format == 'd' || *format == 'i') {
        Serial.print(va_arg( args, int ), DEC);
        continue;
      }
      if ( *format == 'x' ) {
        Serial.print(va_arg( args, int ), HEX);
        continue;
      }
      if ( *format == 'X' ) {
        Serial.print("0x");
        Serial.print(va_arg( args, int ), HEX);
        continue;
      }
      if ( *format == 'b' ) {
        Serial.print(va_arg( args, int ), BIN);
        continue;
      }
      if ( *format == 'B' ) {
        Serial.print("0b");
        Serial.print(va_arg( args, int ), BIN);
        continue;
      }
      if ( *format == 'l' ) {
        Serial.print(va_arg( args, long ), DEC);
        continue;
      }

      if ( *format == 'c' ) {
        Serial.print(va_arg( args, int ));
        continue;
      }
      if ( *format == 't' ) {
        if (va_arg( args, int ) == 1) {
          Serial.print("T");
        }
        else {
          Serial.print("F");
        }
        continue;
      }
      if ( *format == 'T' ) {
        if (va_arg( args, int ) == 1) {
          Serial.print("true");
        }
        else {
          Serial.print("false");
        }
        continue;
      }

    }
    Serial.print(*format);
  }
  Serial.println("");
}
