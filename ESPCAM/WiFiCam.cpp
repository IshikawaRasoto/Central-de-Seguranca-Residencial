/*
    WiFiCam.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include "defineESPCAM.hpp"
#include "WiFiCam.hpp"
#include "EEPROMCam.hpp"

#include <WiFi.h>
#include <ESP32Ping.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include <WebServer.h>

void conectaWiFi(){
    String ssid = EEPROM.readString(EEPROM_SSID);
    String senha = EEPROM.readString(EEPROM_SENHA);

    char char_ssid[TAMANHO_STRING];
    char char_senha[TAMANHO_STRING];

    ssid.toCharArray(char_ssid, TAMANHO_STRING);
    senha.toCharArray(char_senha, TAMANHO_STRING);

    WiFi.begin(char_ssid, char_senha);

    delay(3000);
}

void testeConexao(volatile char* statusWiFi){
    bool ping = Ping.ping("www.google.com", 3);
    if(ping){
        (*statusWiFi) = NET_OK;
        return;
    }
    if(WiFi.status() == WL_CONNECTED){
        (*statusWiFi) = SEM_NET;
        return;
    }
    (*statusWiFi) = SEM_WIFI;
    conectaWiFi();
}

void conectaTelegram(){
    String resposta;

    while(resposta != "Ok");
}