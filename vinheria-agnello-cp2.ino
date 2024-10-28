#include <DHT.h>                     // Inclui a biblioteca para o sensor DHT
#include <Wire.h>                    // Inclui a biblioteca Wire para comunicação I2C
#include <LiquidCrystal_I2C.h>       // Inclui a biblioteca para controle do display LCD via I2C
#include <EEPROM.h>                  // Inclui a biblioteca para leitura e escrita na EEPROM
#include <RTClib.h>                  // Inclui a biblioteca para controle do relógio em tempo real (RTC)

#define DHTPIN 3                     // Define o pino do sensor DHT (DHT22)
#define LDRPIN A0                    // Define o pino do sensor de luminosidade (LDR)
#define RED_LED 10                   // Define o pino para o LED vermelho
#define YELLOW_LED 11                // Define o pino para o LED amarelo
#define GREEN_LED 12                 // Define o pino para o LED verde
#define BUZZER_PIN 7                 // Define o pino para o buzzer

#define DHTTYPE DHT22                 // Define o tipo de sensor DHT como DHT22
DHT dht(DHTPIN, DHTTYPE);             // Cria um objeto DHT com o pino e tipo especificados

LiquidCrystal_I2C lcd(0x27, 16, 2);   // Cria um objeto LCD com o endereço I2C, 16 colunas e 2 linhas

RTC_DS1307 RTC;                       // Cria um objeto RTC para controle do relógio em tempo real

// Define limites de alerta e máximos para temperatura, umidade e luminosidade
const float TEMP_ALERT = 15.0;       // Limite de alerta para temperatura 
const float HUM_ALERT = 90.0;        // Limite de alerta para umidade 
const float LUM_ALERT = 30.0;       // Limite de alerta para luminosidade 
const float TEMP_MAX = 20.0;         // Limite máximo para temperatura 
const float HUM_MAX = 95.0;          // Limite máximo para umidade 
const float LUM_MAX = 40.0;         // Limite máximo para luminosidade 

// Definições dos bytes para os ícones
// Gota para Umidade
byte gota1x1[] = { B00001, B00001, B00001, B00001, B00001, B00011, B00110, B11100 }; // Parte superior da gota
byte gota0x0[] = { B00000, B00001, B00001, B00011, B00010, B00110, B01100, B11000 }; // Parte inferior da gota
byte gota0x1[] = { B00000, B10000, B10000, B11000, B01000, B01100, B00110, B00011 }; // Parte direita da gota
byte gota1x0[] = { B10000, B10100, B10100, B10100, B10010, B11000, B01100, B00111 }; // Parte esquerda da gota
 
// Termômetro para Temperatura
byte temp1x1[] = { B01000, B01000, B00100, B00010, B00010, B00010, B00100, B11000 }; // Parte superior do termômetro
byte temp0x0[] = { B00000, B00001, B00010, B00010, B00010, B00010, B00010, B00010 }; // Parte do tubo do termômetro
byte temp0x1[] = { B00001, B10010, B01001, B01000, B01000, B01000, B01000, B01000 }; // Parte inferior do termômetro
byte temp1x0[] = { B00010, B00010, B00100, B01000, B01000, B01000, B00100, B00011 }; // Base do termômetro

// Luminosidade
byte lumino0x1[] = { B11000, B00100, B11010, B00001, B00101, B00101, B01001, B01010 }; // Parte superior do ícone de luminosidade
byte lumino0x0[] = { B00011, B00100, B01000, B10000, B10100, B10100, B10010, B01010 }; // Parte inferior do ícone de luminosidade
byte lumino1x0[] = { B01010, B00100, B00111, B00100, B01000, B01001, B00100, B00111 }; // Parte direita do ícone de luminosidade
byte lumino1x1[] = { B01010, B00100, B11100, B00100, B00010, B11110, B00100, B11100 }; // Parte esquerda do ícone de luminosidade

// Logo
byte logo1x1[] = { B01010, B00010, B10001, B01001, B00001, B00001, B00010, B11100 }; // Parte superior do logo
byte logo0x0[] = { B00000, B00000, B00000, B00000, B00000, B00010, B00101, B00111 }; // Parte inferior do logo
byte logo0x1[] = { B00000, B00000, B00000, B00000, B00001, B01010, B10101, B11110 }; // Parte direita do logo
byte logo1x0[] = { B01010, B01000, B10001, B10010, B10000, B10000, B01000, B00111 }; // Parte esquerda do logo

// Funções para carregar ícones

// Umidade
void loadHumidityIcons() {
    lcd.createChar(1, gota0x0); // Cria o primeiro ícone de gota
    lcd.createChar(2, gota0x1); // Cria o segundo ícone de gota
    lcd.createChar(3, gota1x0); // Cria o terceiro ícone de gota
    lcd.createChar(4, gota1x1); // Cria o quarto ícone de gota
}

// Temperatura
void loadTemperatureIcons() {
    lcd.createChar(5, temp0x0); // Cria o primeiro ícone de termômetro
    lcd.createChar(6, temp0x1); // Cria o segundo ícone de termômetro
    lcd.createChar(7, temp1x0); // Cria o terceiro ícone de termômetro
    lcd.createChar(8, temp1x1); // Cria o quarto ícone de termômetro
}

// Luminosidade
void loadLuminosityIcons() {
    lcd.createChar(1, lumino0x0); // Cria o primeiro ícone de luminosidade
    lcd.createChar(2, lumino0x1); // Cria o segundo ícone de luminosidade
    lcd.createChar(3, lumino1x0); // Cria o terceiro ícone de luminosidade
    lcd.createChar(4, lumino1x1); // Cria o quarto ícone de luminosidade
}

// Logo
void loadLogo() {
    lcd.createChar(4, logo1x1); // Cria a primeira parte do logo
    lcd.createChar(1, logo0x0); // Cria a segunda parte do logo
    lcd.createChar(2, logo0x1); // Cria a terceira parte do logo
    lcd.createChar(3, logo1x0); // Cria a quarta parte do logo
}

void setup() {
    dht.begin();                   // Inicializa o sensor DHT
    Serial.begin(9600);           // Inicializa a comunicação serial a 9600 bps
    lcd.init();                   // Inicializa o display LCD
    lcd.backlight();              // Liga a luz de fundo do LCD
    RTC.begin();                  // Inicializa o RTC
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__))); // Ajusta o RTC para a data e hora de compilação
    EEPROM.begin();               // Inicializa a EEPROM
  
    pinMode(RED_LED, OUTPUT);     // Define o pino do LED vermelho como saída
    pinMode(YELLOW_LED, OUTPUT);  // Define o pino do LED amarelo como saída
    pinMode(GREEN_LED, OUTPUT);   // Define o pino do LED verde como saída
    pinMode(BUZZER_PIN, OUTPUT);  // Define o pino do buzzer como saída
    loadHumidityIcons();          // Carrega os ícones de umidade
    loadTemperatureIcons();       // Carrega os ícones de temperatura
    loadLuminosityIcons();        // Carrega os ícones de luminosidade
    loadLogo();                   // Carrega os ícones do logo
}

void loop() {
    float temperature = dht.readTemperature(); // Lê a temperatura do sensor DHT
    float humidity = dht.readHumidity();       // Lê a umidade do sensor DHT
    int rawLuminosity = analogRead(LDRPIN);    // Lê o valor bruto do sensor de luminosidade
    int luminosity = map(rawLuminosity, 0, 1023, 0, 100); // Mapeia o valor para 0-100 %

    // Armazena os valores lidos na EEPROM
    EEPROM.put(0, temperature);     // Armazena a temperatura na posição 0 da EEPROM
    EEPROM.put(4, humidity);        // Armazena a umidade na posição 4 da EEPROM
    EEPROM.put(8, luminosity);      // Armazena a luminosidade na posição 8 da EEPROM

    // Lógica para LED e Buzzer
  if (temperature > TEMP_MAX || humidity > HUM_MAX || luminosity > LUM_MAX) {
    digitalWrite(RED_LED, HIGH); 
    tone(BUZZER_PIN, 1000); // Liga o buzzer
    digitalWrite(YELLOW_LED, LOW); 
    digitalWrite(GREEN_LED, LOW); 
}   else if (temperature > TEMP_ALERT || humidity > HUM_ALERT || luminosity > LUM_ALERT) {
    digitalWrite(YELLOW_LED, HIGH); 
    tone(BUZZER_PIN, 500); // Liga o buzzer
    digitalWrite(GREEN_LED, LOW); 
    digitalWrite(RED_LED, LOW); 
}   else {
    digitalWrite(GREEN_LED, HIGH); 
    digitalWrite(YELLOW_LED, LOW); 
    digitalWrite(RED_LED, LOW); 
    noTone(BUZZER_PIN); // Para o som do buzzer completamente
}

    // Carrega e posiciona a primeira parte do logo
    loadLogo();                    // Chama a função para carregar os ícones do logo
    lcd.clear();                  // Limpa o display LCD
    lcd.setCursor(0, 0);          // Define o cursor na posição (0, 0)
    lcd.write(1);                 // Escreve a primeira parte do logo
    lcd.write(2);                 // Escreve a segunda parte do logo
    lcd.setCursor(0, 1);          // Define o cursor na segunda linha (0, 1)
    lcd.write(3);                 // Escreve a terceira parte do logo
    lcd.write(4);                 // Escreve a quarta parte do logo
    lcd.setCursor(6, 0);          // Define o cursor na primeira linha, sexta coluna
    lcd.print("StacK");           // Exibe "StacK" no display
    lcd.setCursor(5, 1);          // Define o cursor na segunda linha, quinta coluna
    lcd.print("SocietY");         // Exibe "SocietY" no display
    delay(5000);                  // Aguarda 5 segundos

    // Exibir umidade com ícones de gota
    loadHumidityIcons();           // Chama a função para carregar os ícones de umidade
    lcd.clear();                  // Limpa o display LCD
    lcd.setCursor(0, 0);          // Define o cursor na primeira linha
    lcd.write(1);                 // Escreve o primeiro ícone de gota
    lcd.write(2);                 // Escreve o segundo ícone de gota
    lcd.setCursor(0, 1);          // Define o cursor na segunda linha
    lcd.write(3);                 // Escreve o terceiro ícone de gota
    lcd.write(4);                 // Escreve o quarto ícone de gota
    lcd.setCursor(3, 0);          // Define o cursor na primeira linha, terceira coluna
    lcd.print(humidity);          // Exibe o valor da umidade
    lcd.print("%");                // Exibe o símbolo de porcentagem
    lcd.setCursor(3, 1);          // Define o cursor na segunda linha, terceira coluna
    lcd.print(getStatus(humidity, HUM_ALERT, HUM_MAX)); // Exibe o status da umidade
    delay(2000);                  // Aguarda 2 segundos

    // Exibir temperatura com ícones de termômetro
    loadTemperatureIcons();        // Chama a função para carregar os ícones de temperatura
    lcd.clear();                  // Limpa o display LCD
    lcd.setCursor(0, 0);          // Define o cursor na primeira linha
    lcd.write(5);                 // Escreve o primeiro ícone de termômetro
    lcd.write(6);                 // Escreve o segundo ícone de termômetro
    lcd.setCursor(0, 1);          // Define o cursor na segunda linha
    lcd.write(7);                 // Escreve o terceiro ícone de termômetro
    lcd.write(8);                 // Escreve o quarto ícone de termômetro
    lcd.setCursor(3, 0);          // Define o cursor na primeira linha, terceira coluna
    lcd.print(temperature);        // Exibe o valor da temperatura
    lcd.print("C");                // Exibe o símbolo de graus Celsius
    lcd.setCursor(3, 1);          // Define o cursor na segunda linha, terceira coluna
    lcd.print(getStatus(temperature, TEMP_ALERT, TEMP_MAX)); // Exibe o status da temperatura
    delay(2000);                  // Aguarda 2 segundos 

    // Exibir luminosidade com ícones de luz
    loadLuminosityIcons();         // Chama a função para carregar os ícones de luminosidade
    lcd.clear();                  // Limpa o display LCD
    lcd.setCursor(0, 0);          // Define o cursor na primeira linha
    lcd.write(1);                 // Escreve o primeiro ícone de luminosidade
    lcd.write(2);                 // Escreve o segundo ícone de luminosidade
    lcd.setCursor(0, 1);          // Define o cursor na segunda linha
    lcd.write(3);                 // Escreve o terceiro ícone de luminosidade
    lcd.write(4);                 // Escreve o quarto ícone de luminosidade
    lcd.setCursor(3, 0);          // Define o cursor na primeira linha, terceira coluna
    lcd.print(luminosity);        // Exibe o valor da luminosidade
    lcd.print("%");                // Exibe o símbolo de porcentagem
    lcd.setCursor(3, 1);          // Define o cursor na segunda linha, terceira coluna
    lcd.print(getLightStatus(luminosity, LUM_ALERT, LUM_MAX)); // Exibe o status da luminosidade
    delay(2000);                  // Aguarda 2 segundos
}

// Funções de status
// Função para determinar o status da umidade e temperatura
const char* getStatus(float value, float alertLimit, float maxLimit) {
    if (value > maxLimit) return "Ruim";        // Se o valor é maior que o limite máximo, retorna "Ruim"
    else if (value > alertLimit) return "Media"; // Se o valor é maior que o limite de alerta, retorna "Media"
    else return "Otimo";                           // Caso contrário, retorna "Ok"
}

// Função para determinar o status da luminosidade
const char* getLightStatus(int Lumvalue, float alertLumLimit, float maxLumLimit) {
    if (Lumvalue > maxLumLimit) return "Ruim";        // Se o valor é maior que o limite máximo, retorna "Ruim"
    else if (Lumvalue > alertLumLimit) return "Media"; // Se o valor é maior que o limite de alerta, retorna "Media"
    else return "Otimo";                             // Caso contrário, retorna "Otimo"
}
