/*
    TestesBasicosESP.ino - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include <time.h> 
#include <ESP32Time.h>

ESP32Time rtc(-10800); // GMT -3h em Segundos

void setup(){
     Serial.begin(115200);
     rtc.setTime(0, 0, 17, 18, 9, 2022); // Data: 18/09/2022 às 17:00:00
}

void loop(){
    Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));
    delay(1000);
}
