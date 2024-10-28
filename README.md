![CP2 Vinheria Agnello](Vinheria-agnello-cp2.png)

# Monitoramento Ambiental com Display LCD, Sensor DHT22, LDR e RTC

Este projeto é um sistema de monitoramento ambiental que utiliza um Arduino para monitorar e exibir temperatura, umidade e luminosidade em um display LCD. Dependendo das leituras dos sensores, LEDs de diferentes cores e um buzzer indicam o status do ambiente. Este sistema é útil para monitorar ambientes onde temperatura, umidade e luz precisam estar em níveis controlados.

## Funcionalidades

- **Monitoramento de Temperatura e Umidade**: Utiliza o sensor DHT22 para obter leituras de temperatura e umidade.
- **Monitoramento de Luminosidade**: O sensor de luminosidade (LDR) lê a intensidade da luz e a converte para uma escala de 0 a 100%.
- **Indicadores Visuais e Sonoros**:
  - **LEDs**:
    - Verde: indica valores dentro dos níveis normais.
    - Amarelo: indica valores que estão em alerta.
    - Vermelho: indica valores acima dos limites aceitáveis.
  - **Buzzer**: emite sons de alerta em diferentes frequências conforme o nível de atenção (média ou ruim).
- **Armazenamento de Dados**: Os valores lidos são salvos na EEPROM para persistência dos dados.
- **Exibição no Display LCD**: Valores de temperatura, umidade e luminosidade são mostrados no display com ícones customizados e mensagens de status.
- **Relógio em Tempo Real (RTC)**: Mantém o registro da data e hora das medições.

## Componentes Necessários

- Arduino (UNO, Mega, etc.)
- Sensor DHT22 para temperatura e umidade
- Sensor de luminosidade (LDR) com um resistor de 10kΩ
- Módulo de display LCD I2C 16x2
- Módulo RTC (DS1307 ou DS3231)
- LEDs (vermelho, amarelo e verde)
- Buzzer
- Protoboard e jumpers
- Resistores de 220Ω para LEDs e resistores de pull-up/down para o LDR (10kΩ)

## Dependências

As seguintes bibliotecas devem ser instaladas para o funcionamento correto do código:

- `DHT.h` - Para comunicação com o sensor DHT22.
- `Wire.h` - Para comunicação I2C.
- `LiquidCrystal_I2C.h` - Para controlar o display LCD via I2C.
- `EEPROM.h` - Para leitura e escrita na memória EEPROM do Arduino.
- `RTClib.h` - Para comunicação com o módulo RTC.

Para instalar estas bibliotecas, vá em **Sketch > Incluir Biblioteca > Gerenciar Bibliotecas** no Arduino IDE e procure pelo nome de cada biblioteca.

## Montagem do Circuito

### Sensor DHT22:
- Conecte o pino de dados ao pino 3 do Arduino.
- Conecte o pino VCC ao 5V e o GND ao GND do Arduino.

### Sensor LDR:
- Conecte um lado do LDR ao VCC (5V).
- Conecte o outro lado ao pino A0 do Arduino e ao resistor de 10kΩ, que deve estar conectado ao GND.

### LEDs e Buzzer:
- Conecte os LEDs nos pinos 10 (vermelho), 11 (amarelo) e 12 (verde).
- Conecte o buzzer ao pino 7 do Arduino.
- Use resistores de 220Ω em série com cada LED.

### Display LCD I2C:
- Conecte os pinos SDA e SCL do módulo I2C aos pinos correspondentes no Arduino (A4 e A5 no UNO).

### RTC:
- Conecte o módulo RTC aos pinos SDA e SCL (A4 e A5 no Arduino UNO).

## Como Reproduzir

1. **Monte o Circuito**: Siga as instruções de montagem descritas acima.
2. **Instale as Bibliotecas**: Certifique-se de que todas as bibliotecas necessárias estão instaladas na IDE do Arduino.
3. **Carregue o Código**: Abra o código no Arduino IDE e faça o upload para a placa.
4. **Observação dos Indicadores**:
   - Ao inicializar, o sistema exibirá o logo e, em seguida, alternará entre as leituras de umidade, temperatura e luminosidade.
   - LEDs e buzzer indicarão níveis de alerta para cada leitura.
   - O display LCD mostrará as leituras com ícones específicos para cada métrica e exibe um status textual ("Ótimo", "Média" ou "Ruim").

## Exemplo de Configuração e Funcionamento

No display, você verá:

- **Temperatura**: Exibe a leitura atual em °C e um termômetro ao lado.
- **Umidade**: Mostra o valor de umidade com um ícone de gota d'água.
- **Luminosidade**: Apresenta a porcentagem de luminosidade com um ícone de luz.

O sistema mudará o LED de acordo com os níveis:

- **Verde** para condições ótimas.
- **Amarelo** para condições de alerta.
- **Vermelho** e **buzzer** para condições críticas.

Este projeto pode ser adaptado para monitorar e ajustar parâmetros ambientais em diferentes aplicações, como estufas, armazéns e até mesmo residências.

## Autores

- Vitor de Lima Domingues.
- Giovanni Romano Provazi.
- João Pedro Vieira de Morais.
- Enzo Galhardo.