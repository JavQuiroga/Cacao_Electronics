#include <SPI.h>

// Definimos los pines
const int CS_PIN = 17;  // Pin Chip Select (GP17)
const int clockPin = 18; // GP18 - Pin de reloj
const int MISO_PIN = 16; // Pin MISO (GP16)

// Configuración inicial
void setup() {
  // Configuramos el pin CS como salida y lo ponemos en alto
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

  // Configuramos el pin GP18 como salida
  pinMode(clockPin, OUTPUT);

  // Iniciamos el SPI
  SPI.begin();
  Serial.begin(115200);
}

void loop() {
  // Leer solo el canal 1 del ADC
  uint16_t adc_val = read_adc();

  // Convertir el valor ADC a voltaje (suponiendo referencia de 3.3V)
  float voltage = (adc_val / 4095.0) * 3.3;

  // Mostrar el valor en el monitor serial
  Serial.print("ADC Value: ");
  Serial.print(adc_val);
  Serial.print(" | Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  // Esperar 1 segundo antes de la siguiente lectura
  delay(1000);
}

// Función para leer del canal 1 del ADC (comunicación SPI)
uint16_t read_adc() {
  // Comando para seleccionar el canal 1 en modo Manual
  uint16_t command = 0b0001000100000000;  // Modo Manual y canal 1 (DI15-DI7) #El codigo que hay actualmente es el del canal 0
  // Comandos para leer cada canal en modo Manual en el ADS7953
//uint16_t command_ch0 =  0b0001000000000000;  // Canal 0
//uint16_t command_ch1 =  0b0001000100000000;  // Canal 1
//uint16_t command_ch2 =  0b0001001000000000;  // Canal 2
//uint16_t command_ch3 =  0b0001001100000000;  // Canal 3
//uint16_t command_ch4 =  0b0001010000000000;  // Canal 4
//uint16_t command_ch5 =  0b0001010100000000;  // Canal 5
//uint16_t command_ch6 =  0b0001011000000000;  // Canal 6
//uint16_t command_ch7 =  0b0001011100000000;  // Canal 7
//uint16_t command_ch8 =  0b0001100000000000;  // Canal 8
//uint16_t command_ch9 =  0b0001100100000000;  // Canal 9
//uint16_t command_ch10 = 0b0001101000000000;  // Canal 10
//uint16_t command_ch11 = 0b0001101100000000;  // Canal 11
//uint16_t command_ch12 = 0b0001110000000000;  // Canal 12
//uint16_t command_ch13 = 0b0001110100000000;  // Canal 13
//uint16_t command_ch14 = 0b0001111000000000;  // Canal 14
//uint16_t command_ch15 = 0b0001111100000000;  // Canal 15


  // Bajamos CS para iniciar la comunicación
  digitalWrite(CS_PIN, LOW);

  // Iniciamos la transacción SPI a 1 MHz
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

  // Enviamos el comando al ADC
  SPI.transfer16(command);                 // Enviar comando para leer el canal 1

  // Recibir el valor del ADC (12 bits)
  uint16_t adc_value = SPI.transfer16(0);  // Leer el resultado ADC de 16 bits

  // Subimos CS para finalizar la comunicación
  digitalWrite(CS_PIN, HIGH);

  // Finalizamos la transacción SPI
  SPI.endTransaction();

  // El valor ADC está en los 12 bits menos significativos
  return adc_value & 0x0FFF;
}
