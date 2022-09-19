/*
    TestesBasicosESP.ino - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include <time.h> 
#include <ESP32Time.h>

#define BOTAO 25
#define LED 26

ESP32Time rtc(-10800); // GMT -3h em Segundos

volatile bool led_state;

void IRAM_ATTR onTimer(){
    if(led_state){
        digitalWrite(LED, LOW);
        led_state = false;
        return;
    }
    digitalWrite(LED, HIGH);
    led_state = true;
}

void setup(){
     Serial.begin(115200);
     rtc.setTime(0, 0, 20, 18, 9, 2022); // Data: 18/09/2022 às 17:00:00 (BRT) (20:00:00 no Horário GMT)

     hw_timer_t* timer = NULL;
     unsigned int timerfactor = 1000000;
     
     timer = timerBegin(0,80,true); //80MHz / 80 = 1MHz
     timerAttachInterrupt(timer, &onTimer, true);
     timerAlarmWrite(timer, timerfactor, true);
     timerAlarmEnable(timer);

     pinMode(LED, OUTPUT);

     led_state = false;
     digitalWrite(LED, LOW);
}

void loop(){
    Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));
    delay(3000);
}
