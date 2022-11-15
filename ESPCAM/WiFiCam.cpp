/*
    WiFiCam.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include "defineESPCAM.hpp"
#include "WiFiCam.hpp"

#include <WiFi.h>
#include <ESP32Ping.h>
#include <Ethernet.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"

WiFiClientSecure clientTCP;
UniversalTelegramBot* bot;

String ssid_Telegram;
String senha_Telegram;
String token_Telegram;
String chatID_Telegram;
String chat_id_recebido;

void conectaWiFi()
{
    char char_ssid[TAMANHO_STRING];
    char char_senha[TAMANHO_STRING];

    ssid_Telegram.toCharArray(char_ssid, TAMANHO_STRING);
    senha_Telegram.toCharArray(char_senha, TAMANHO_STRING);

    WiFi.begin(char_ssid, char_senha);

    delay(3000);
}

void testeConexao(volatile char* statusWiFi)
{
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
    Serial.println("WiFi NOK");
    (*statusWiFi) = SEM_WIFI;
    conectaWiFi();
}

void conectaTelegram(){
    Serial.println("Conecta Telegram");
    
    bot = new UniversalTelegramBot(token_Telegram, clientTCP);

    String mensagem = "ESPCAM conectada com sucesso.";
    bot->sendMessage(chatID_Telegram, mensagem, "");
}

void informacoesWiFi(){
    int i = 0;
    while(i < 4){
        while(!Serial1.available());
        String recebido = Serial1.readString();
        Serial.println("String recebida: " + recebido);
        switch(i){
            case 0:
                ssid_Telegram = recebido;
                break;
            case 1:
                senha_Telegram = recebido;
                break;
            case 2:
                token_Telegram = recebido;
                break;
            case 3:
                chatID_Telegram = recebido;
                break;
        }
        i++;
    }
}

String sendPhotoTelegram()
{
    const char* myDomain = "api.telegram.org";
    String getAll = "";
    String getBody = "";

    camera_fb_t * fb = NULL;
    fb = esp_camera_fb_get();
    esp_camera_fb_return(fb);
    fb = esp_camera_fb_get();
   
  
    if(!fb) 
    {
        Serial.println("Camera capture failed");
        delay(1000);
        ESP.restart();
        return "Camera capture failed";
    }  
  
    Serial.println("Connect to " + String(myDomain));

    if (clientTCP.connect(myDomain, 443)) 
    {
        Serial.println("Connection successful");
    
        String head = "--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"chat_id\"; \r\n\r\n" + chatID_Telegram + "\r\n--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
        String tail = "\r\n--RandomNerdTutorials--\r\n";

        uint16_t imageLen = fb->len;
        uint16_t extraLen = head.length() + tail.length();
        uint16_t totalLen = imageLen + extraLen;
  
        clientTCP.println("POST /bot"+token_Telegram+"/sendPhoto HTTP/1.1");
        clientTCP.println("Host: " + String(myDomain));
        clientTCP.println("Content-Length: " + String(totalLen));
        clientTCP.println("Content-Type: multipart/form-data; boundary=RandomNerdTutorials");
        clientTCP.println();
        clientTCP.print(head);
    
        uint8_t *fbBuf = fb->buf;
        size_t fbLen = fb->len;
        for (size_t n=0;n<fbLen;n=n+1024) 
        {
            if (n+1024<fbLen) 
            {
                clientTCP.write(fbBuf, 1024);
                fbBuf += 1024;
            }
            
            else if (fbLen%1024>0) 
            {
                size_t remainder = fbLen%1024;
                clientTCP.write(fbBuf, remainder);
            }
        }  
    
        clientTCP.print(tail);
        esp_camera_fb_return(fb);
    
        int waitTime = 10000;   // timeout 10 seconds
        long startTimer = millis();
        boolean state = false;
        
        while ((startTimer + waitTime) > millis())
        {
            Serial.print(".");
            delay(100);      
            while (clientTCP.available()) 
            {
                char c = clientTCP.read();
                if (state==true) getBody += String(c);        
                if (c == '\n') 
                {
                    if (getAll.length()==0) state=true; 
                    getAll = "";
                }

                else if (c != '\r')
                getAll += String(c);
                startTimer = millis();
            }
            if (getBody.length()>0) break;
        }

        clientTCP.stop();
        Serial.println(getBody);
    }

    else 
    {
        getBody="Connected to api.telegram.org failed.";
        Serial.println("Connected to api.telegram.org failed.");
    }

    return getBody;
}
