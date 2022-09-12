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

#include "HTML.h"

String ssid;
String senha;
String token;
String chatID;

volatile bool statusModoAP;

WebServer server (80);

void limparMemoria(){
    for(unsigned int i = 0; i < 250; i++){
        EEPROM.writeByte(i, 0);
    }
    EEPROM.commit();
}

bool testeCredenciais(){
    String ssid_teste = EEPROM.readString(0);
    String senha_teste = EEPROM.readString(50);
    String token_teste = EEPROM.readString(150);
    String chatID_teste = EEPROM.readString(100);

    if(ssid_teste.length() > 0 && senha_teste.length() > 0 && token_teste.length() > 0 && chatID_teste.length() > 0)
        return true;
    return false;
}

void conectaWiFi(){
    ssid = EEPROM.readString(0);
    senha = EEPROM.readString(50);

    char char_ssid [30];
    char char_senha [30];

    ssid.toCharArray(char_ssid, 30);
    ssid.toCharArray(char_senha, 30);

    WiFi.begin(char_ssid, char_senha);

    delay(3000);
}

void testeConexao(){
    bool ping = Ping.ping("www.google.com", 3);
    if(ping){
        Serial.println("Internet OK");
        return;
    }

    if(WiFi.status() == WL_CONNECTED){
        Serial.println("WiFi OK");
        return;
    }

    Serial.println("WiFi NOK");
}

bool escreverNaMemoria(String ssidAUX, String senhaAUX, String tokenAUX, String chatIDAUX){
    char ssid_buffer [30];
    char senha_buffer [30];
    char token_buffer [60];
    char chat_buffer [30];

    ssidAUX.toCharArray(ssid_buffer, 30);
    senhaAUX.toCharArray(senha_buffer, 30);
    tokenAUX.toCharArray(token_buffer, 60);
    chatIDAUX.toCharArray(chat_buffer, 30);

    EEPROM.writeString(0, ssid_buffer);
    EEPROM.writeString(50, senha_buffer);
    EEPROM.writeString(100, chat_buffer);
    EEPROM.writeString(150, token_buffer);

    EEPROM.commit();

    String ssid_ver = EEPROM.readString(0);
    String senha_ver = EEPROM.readString(50);
    String token_ver = EEPROM.readString(150);
    String chat_ver = EEPROM.readString(100);

    if(ssidAUX == ssid_ver && senhaAUX == senha_ver && tokenAUX == token_ver && chatIDAUX == chat_ver)
        return true;
    return false;
}

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

void handleRoot(){
    if(server.hasArg("ssid") && server.hasArg("password") && server.hasArg("token") && server.hasArg("chatID"))
        handleSubmit();
    else
        server.send(200, "text/html", INDEX_HTML);
}

void enviarFormulario(){
    const char* ssid_AP = "Central_De_Seguranca";
    const char* senha_AP = "CentralSEG!2022";

    WiFi.softAP(ssid_AP, senha_AP);

    IPAddress IP = WiFi.softAPIP();

    server.on("/", handleRoot);
    server.onNotFound(handleNotFound);
    server.begin();

    Serial.println("Servidor AP Iniciado");
  
    while(true){
        server.handleClient();
        delay(100);
    }
}

void IRAM_ATTR sensorDePresenca(){
   statusModoAP = true;
}



void setup(){
    Serial.begin(115200);
    EEPROM.begin(400);

    statusModoAP = false;

    attachInterrupt(25, sensorDePresenca, RISING);
}

void loop (){
    if(statusModoAP)
        limparMemoria();
    if(!testeCredenciais()){
        Serial.println("modoAP");
        enviarFormulario();
    }
}
