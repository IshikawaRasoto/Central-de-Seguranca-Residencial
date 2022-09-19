# Conexão e compilação

## ESP32-Cam usando Arduino Uno:
<img src="https://github.com/IshikawaRasoto/Central-de-Seguranca-Residencial/blob/main/Teste/ESP32-Cam/assets/ESP32CAM-ARDUINO_UNO.png" width = 700>


## ESP32-Cam usando FTDI:
<img src="https://github.com/IshikawaRasoto/Central-de-Seguranca-Residencial/blob/main/Teste/ESP32-Cam/assets/ESP32CAM-FTDI.png" width = 700>
<img src="https://github.com/IshikawaRasoto/Central-de-Seguranca-Residencial/blob/main/Teste/ESP32-Cam/assets/Pinos-FTDI.jpg" width = 700>


# Configurações no Arduino IDE

## Vá até Ferramentas nas configurações da IDE
#### •Placa: "ESP32 Wrover Module"
#### •Upload Speed: "115200"
#### •Flash Frequency: "40MHz"
#### •Flash Mode: "QIO"
#### •Partition Scheme: "Huge APP (3MB No OTA/1MB SPIFFS")

> #### Se certifique que a porta configurada é a conectada
> #### Se certificque que todas as bibliotecas estão instaladas
> #### Configure a rede wi-fi 
> #### Configure o BOTtoken
> #### Configure o CHAT_ID


# Instruções para compilação correta na ESP32-Cam

### 1. Compile 
### 2. Após compilar, desfaça a conexão entre o GPIO 0 e o GND. 
> #### Explicação: O curto-circuito entre o GPIO 0 e o GND faz com que a ESP32-Cam entre em seu modo programável. Desfazendo a conexão, ela volta para o modo funcional. 
### 3. Resete a placa. 
> #### Explicação: Resetando você faz com que a ESP funcione de acordo com o último código compilado.
