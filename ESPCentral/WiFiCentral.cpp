/*
    WiFiCentral.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include "WiFiCentral.hpp"
#include "defineESPCentral.hpp"
#include "EEPROMCentral.hpp"
#include "HTML.h"

#include <WiFi.h>
#include <ESP32Ping.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include <WebServer.h>

WebServer server(80);

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

bool enviaFormulario(){
    const char* ssid_AP = "Central_De_Seguranca";
    const char* senha_AP = "CentralSEG!2022";

    WiFi.softAP(ssid_AP, senha_AP);

    IPAddress IP = WiFi.softAPIP();

    server.on("/", handleRoot);
    server.onNotFound(handleNotFound);
    server.begin();

    Serial.println("Servidor HTTP iniciado");

    String ssidAux = EEPROM.readString(EEPROM_SSID);
    String senhaAux = EEPROM.readString(EEPROM_SENHA);
    String tokenAux = EEPROM.readString(EEPROM_TOKEN);
    String chatIDAux = EEPROM.readString(EEPROM_CHATID);

    while(ssidAux.length() <= 0 || senhaAux.length() <= 0 || tokenAux.length() <= 0 || chatIDAux.length() <= 0){
        server.handleClient();
        delay(100);
    }
    return false;
}

//Função da Página Home
void handleRoot(){
    if(server.hasArg("ssid") && server.hasArg("password") && server.hasArg("token") && server.hasArg("chatID"))
        handleSubmit();
    else
        server.send(200, "text/html", INDEX_HTML);
}

// Falha no envido do formulário
void handleNotFound(){
    String message = "Arquivo não encontrado \n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMétodo:";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments:";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++){
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void handleSubmit(){
    String sucesso_resposta = "<h1> Sucesso! </h1>";
    sucesso_resposta += "<h2> O dispositivo irá reiniciar em 3 segundos ... </h2>";

    String erro_resposta = "<h1> Erro! </h1>";
    erro_resposta += "<a href='/'>Volte</a>para tentar novamente!";

    if((escreverNaMemoria(String(server.arg("ssid")), String(server.arg("password")), String(server.arg("token")), String(server.arg("chatID"))))){
        server.send(200, "text/html", sucesso_resposta);
        EEPROM.commit();
        delay(3000);
        ESP.restart();
    }else
        server.send(200, "text/html", erro_resposta); 
}
