/*
    WiFiCentral.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include "WiFiCentral.hpp"
#include "defineESPCentral.hpp"
#include "EEPROMCentral.hpp"
#include "HTML.h"
#include "ESPCentral.hpp"

#include <WiFi.h>
#include <ESP32Ping.h>
#include <EEPROM.h>
#include <Ethernet.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

WiFiClientSecure client;
UniversalTelegramBot* bot;

String ssid_Telegram;
String senha_Telegram;
String chatID_Telegram;
String botToken_Telegram;
String chat_id; // Chat verificado pelo BOT, comparamos com o que temos cadastrado na EEPROM chatID

/*
// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = 2;
bool ledState = LOW;
*/


WebServer server(80);

void conectaWiFi(){
    ssid_Telegram = EEPROM.readString(EEPROM_SSID);
    senha_Telegram = EEPROM.readString(EEPROM_SENHA);

    char char_ssid[TAMANHO_STRING];
    char char_senha[TAMANHO_STRING];

    ssid_Telegram.toCharArray(char_ssid, TAMANHO_STRING);
    senha_Telegram.toCharArray(char_senha, TAMANHO_STRING);

    WiFi.mode(WIFI_STA);
    WiFi.begin(char_ssid, char_senha);
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
    delay(3000);
}

void testeConexao(volatile char* statusWiFi){
    bool ping = Ping.ping("www.google.com", 3);
    if(ping){
        Serial.println("NET OK");
        (*statusWiFi) = NET_OK;
        return;
    }
    if(WiFi.status() == WL_CONNECTED){
        Serial.println("NET NOK");   
        (*statusWiFi) = SEM_NET;
        return;
    }
    Serial.println("sem WIFI");
    (*statusWiFi) = SEM_WIFI;
    conectaWiFi();
}

void conectaTelegram(){
  Serial.println("Conecta Telegram");
  chatID_Telegram = EEPROM.readString(EEPROM_CHATID);
  botToken_Telegram = EEPROM.readString(EEPROM_TOKEN);

  bot = new UniversalTelegramBot(botToken_Telegram, client);
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

    String ssidAux    = EEPROM.readString(EEPROM_SSID);
    String senhaAux   = EEPROM.readString(EEPROM_SENHA);
    String tokenAux   = EEPROM.readString(EEPROM_TOKEN);
    String chatIDAux  = EEPROM.readString(EEPROM_CHATID);

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
    sucesso_resposta += "<h2> O dispositivo irá reiniciar em alguns segundos ... </h2>";

    String erro_resposta = "<h1> Erro! </h1>";
    erro_resposta += "<a href='/'>Volte</a>para tentar novamente!";

    if((escreverNaMemoria(String(server.arg("ssid")), String(server.arg("password")), String(server.arg("token")), String(server.arg("chatID"))))){
        server.send(200, "text/html", sucesso_resposta);

        String ssid_buffer    = EEPROM.readString(EEPROM_SSID);
        String senha_buffer   = EEPROM.readString(EEPROM_SENHA);
        String token_buffer   = EEPROM.readString(EEPROM_TOKEN);
        String chatID_buffer  = EEPROM.readString(EEPROM_CHATID);

        Serial1.print("RESTART");

        delay(3000);
        ESP.restart();
    }else
        server.send(200, "text/html", erro_resposta); 
}


/* *************************************************** */
/* Código para o Telegram */
/* *************************************************** */

void mensagemTelegram(){
    int numNewMessages = bot->getUpdates(bot->last_message_received + 1);
    while(numNewMessages){
        Serial.println("got response");
        handleNewMessages(numNewMessages);
        numNewMessages = bot->getUpdates(bot->last_message_received + 1);
    }
}

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    chat_id = String(bot->messages[i].chat_id);
    if (chat_id != chatID_Telegram){
      bot->sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String texto = bot->messages[i].text;
    Serial.println(texto);

    String nome = bot->messages[i].from_name;

    comandosTelegram(texto, nome);
    
  }
}


void comandosTelegram(String texto, String nome){
    if (texto == "/iniciar") {
      String welcome = "Bem-Vindo, " + nome + ".\n";
      welcome += "/led_on to turn GPIO ON \n";
      welcome += "/led_off to turn GPIO OFF \n";
      welcome += "/state to request current GPIO state \n";
      bot->sendMessage(chat_id, welcome, "");
    }
    else if(texto == "teste"){
      bot->sendMessage(chat_id, "Funcionou", "");
    }
}
