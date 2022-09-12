/*
    ModoAP.ino - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include <WiFi.h>
#include <ESP32Ping.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include <WebServer.h>

volatile bool statusModoAP;

Webserver server (80);

void conectaWiFi(){
  
}

void IRAM_ATTR sensorDePresenca(){
   statusModoAP = true;
}

void enviarFormulario(){

}

void setup(){
    Serial.begin(115200);

    attachInterrupt(PINO_SENSOR_PRESENCA, sensorDePresenca, RISING);
}

void loop (){
    
}
