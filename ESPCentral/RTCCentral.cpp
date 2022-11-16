/*
    RTCCentral.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include "RTCCentral.hpp"

#include <ESP32Time.h>

ESP32Time rtc(0); // A princípio o parâmetro determina o fuso horário, mas como não vamos pegar o horário da internet usamos como o fuso padrão 0.

void inicializaRTC(){
    rtc.setTime(0, 0, 0, 1, 1, 2022); //01/01/2022 às 00:00:00
}

void setHorario(const int dia, const int mes, const int ano, const int h, const int m, const int s){
    rtc.setTime(s, m, h, dia, mes, ano + 2000);
    Serial.println(dia);
    Serial.println(mes);
    Serial.println(ano);
    Serial.println(h);
    Serial.println(m);
    Serial.println(s);
}

String getHorario(){
    return rtc.getTime("%T");
}

String getData(){
    return rtc.getTime("%D");
}
