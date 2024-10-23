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
  // Leer el canal 0 del ADC
  uint16_t adc_val = read_adc(0);

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

// Función para leer del ADC (comunicación SPI)
uint16_t read_adc(uint8_t channel) {
  // Comando para seleccionar el canal (ajusta según el formato de comando del ADS7953DBT)
  uint8_t command = 0b00000001 | (channel & 0x03); // Cambia esto según sea necesario

  // Bajamos CS para iniciar la comunicación
  digitalWrite(CS_PIN, LOW);

  // Iniciamos la transacción SPI a 1 MHz
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

  // Enviamos el comando al ADC
  SPI.transfer(command); // Envía el comando para leer el canal

  // Recibir el valor del ADC (12 bits)
  uint16_t adc_value = SPI.transfer(0) << 4; // Recibir los 4 bits más significativos
  adc_value |= (SPI.transfer(0) >> 4); // Recibir los 8 bits menos significativos

  // Subimos CS para finalizar la comunicación
  digitalWrite(CS_PIN, HIGH);

  // Finalizamos la transacción SPI
  SPI.endTransaction();

  return adc_value;
}
