const int LED_RED = 13;
const int LED_GREEN = 12;
const int LED_YELLOW = 11;
const int BUTTON = 7;

int CONT = 0;
bool cicloCompletado = false;
bool statusRedLed = false;

bool intermitenceRed = false;
bool intermitenceGreen = false;
bool intermitenceYellow = false;

bool blinkState = false;
unsigned long lastPressTime = 0;
unsigned long lastBlink = 0;
const unsigned long pressInterval = 150;
const unsigned long BLINK_INTERVAL = 1000;

void setup() {
  Serial.begin(9600);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  apagarTodos("");
  mostrarMenu();
}

void loop() {
  unsigned long currentTime = millis();

  if (Serial.available() > 0) {
    conditional(Serial.read());
    if (cicloCompletado) {
      CONT = 0;
      cicloCompletado = false;
    }
  }

  if (digitalRead(BUTTON) == LOW && currentTime - lastPressTime > pressInterval) {
    ejecutarEstado(CONT++);
    if (CONT > 4) {
      CONT = 0;
      cicloCompletado = true;
    }
    lastPressTime = currentTime;
  }

  if (currentTime - lastBlink >= BLINK_INTERVAL) {
    lastBlink = currentTime;
    blinkState = !blinkState;
    if (intermitenceRed) digitalWrite(LED_RED, blinkState);
    if (intermitenceGreen) digitalWrite(LED_GREEN, blinkState);
    if (intermitenceYellow) digitalWrite(LED_YELLOW, blinkState);
  }
}

void ejecutarEstado(int est) {
  switch (est) {
    case 0: encenderLed(LED_RED, "Pulsador: LED Rojo encendido"); break;
    case 1: encenderLed(LED_GREEN, "Pulsador: LED Verde encendido"); break;
    case 2: encenderLed(LED_YELLOW, "Pulsador: LED Amarillo encendido"); break;
    case 3: apagarTodos("Pulsador: Todos apagados"); break;
    case 4: encenderTodos("Pulsador: Todos encendidos"); break;
    case 5: encenderLed(LED_RED, "Pulsador: Regreso al LED Rojo"); break;
  }
}

void conditional(char opt) {
  switch (opt) {
    case '1': statusRedLed = true; encenderLed(LED_RED, "Serial: LED Rojo encendido"); break;
    case '2': statusRedLed = false; apagarLed(LED_RED, "Serial: LED Rojo apagado"); break;
    case '3': encenderLed(LED_GREEN, "Serial: LED Verde encendido"); break;
    case '4': apagarLed(LED_GREEN, "Serial: LED Verde apagado"); break;
    case '5': encenderLed(LED_YELLOW, "Serial: LED Amarillo encendido"); break;
    case '6': apagarLed(LED_YELLOW, "Serial: LED Amarillo apagado"); break;
    case '7': encenderTodos("Serial: Todos encendidos"); break;
    case '8': apagarTodos("Serial: Todos apagados"); break;
    case '9':
      intermitenceRed = true;
      intermitenceGreen = true;
      intermitenceYellow = true;
      break;
    case '0':
      statusRedLed = !statusRedLed;
      statusRedLed ? encenderLed(LED_RED, "Serial: LED Rojo encendido") : apagarLed(LED_RED, "Serial: LED Rojo apagado");
      break;
    default: Serial.println("Selecciona un número del 1 al 9"); break;
  }
  mostrarMenu();
}

void encenderLed(int pin, const char* mensaje) {
  actualizarIntermitencia(pin, false);
  digitalWrite(pin, HIGH);
  Serial.println(mensaje);
}

void apagarLed(int pin, const char* mensaje) {
  actualizarIntermitencia(pin, false);
  digitalWrite(pin, LOW);
  Serial.println(mensaje);
}

void actualizarIntermitencia(int pin, bool estado) {
  if (pin == LED_RED) intermitenceRed = estado;
  else if (pin == LED_GREEN) intermitenceGreen = estado;
  else if (pin == LED_YELLOW) intermitenceYellow = estado;
}

void encenderTodos(const char* mensaje) {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_YELLOW, HIGH);
  if (mensaje[0]) Serial.println(mensaje);
}

void apagarTodos(const char* mensaje) {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  if (mensaje[0]) Serial.println(mensaje);
}

void mostrarMenu() {
  Serial.println("-------- Menú Principal --------");
  Serial.println("[1] Encender LED Rojo");
  Serial.println("[2] Apagar LED Rojo");
  Serial.println("[3] Encender LED Verde");
  Serial.println("[4] Apagar LED Verde");
  Serial.println("[5] Encender LED Amarillo");
  Serial.println("[6] Apagar LED Amarillo");
  Serial.println("[7] Encender todos los LEDs");
  Serial.println("[8] Apagar todos los LEDs");
  Serial.println("[9] Intermitencia (fade in/out)");
  Serial.println("[0] Turn on/off LED Rojo");
}