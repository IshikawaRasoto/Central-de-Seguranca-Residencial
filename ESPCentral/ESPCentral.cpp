/*
    ESPCentral.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include "defineESPCentral.hpp"
#include "ESPCentral.hpp"
#include "EEPROMCentral.hpp"
#include "WiFiCentral.hpp"
#include "DHTCentral.hpp"
#include "RFIDCentral.hpp"

//Variáveis de Wi-Fi
String ssid;
String senha;
String chatID;
String token;


//Variáveis do Sistema
volatile bool modoAP;
bool alarme;
bool disparaAlarme;
volatile char statusWiFi;
bool statusComunicacao;

//Variáveis DHT11
float temperatura;
float umidade;

// Configuração

void configuracao(){
    serialConfig();
    inicializaEEPROM();
    carregarDados(&ssid, &senha, &token, &chatID);
    enviaDadosWiFi();
    inicializarVariaveis();
    configPinos();
    //configModulos();

    /*while(statusWiFi == SEM_WIFI){
        conectaWiFi();
        testeConexao(&statusWiFi);
    }*/

    //conectaTelegram();
}

void serialConfig(){
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, RX_CAM, TX_CAM);
}

void inicializarVariaveis(){
    modoAP              = false;
    statusComunicacao   = true;
    alarme              = false;
    disparaAlarme       = false;
    statusWiFi          = SEM_WIFI;
}

void configPinos(){
    pinMode(BOTAO_AP, INPUT);
}

void configModulos(){
    SPI.begin();
    configuraRFID_SD();
    verificaTXT();
    inicializaDHT();
}

void enviaDadosWiFi(){
    delay(6000);
    Serial.println("SSID enviado: " + ssid);
    Serial1.print(ssid);
    delay(2000);
    Serial.println("Senha Enviada: " + senha);
    Serial1.print(senha);
    delay(2000);
    Serial.println("Token enviado: " + token);
    Serial1.print(token);
    delay(5000);
    Serial.println("ChatID Enviado: " + chatID);
    Serial1.print(chatID);
    delay(2000);
}

// Execução
void executar(){
    verificacoes();
    operacoes();
    
}

void operacoes(){
    atualizarDHT(&umidade, &temperatura);
    verificaRFID();
}

void verificacoes(){
    verificaModoAP();
    verificaConexao();
    verificaComunicacao();
    verificaAlarme();
}

void verificaConexao(){
    testeConexao(&statusWiFi);
}

void verificaModoAP(){
    if(modoAP)
        limparMemoria();
    if(testeCredenciais())
        return;
    modoAP = true;
    while(enviaFormulario());
}

void verificaComunicacao(){
    
    Serial1.print("TesteComunicacao");

    while(!Serial1.available());
    String mensagem = Serial1.readString();
    Serial.println(mensagem);
    if(mensagem == "TesteComunicacao"){
        statusComunicacao = true;
        Serial.println("Comunicação Serial OK");
        return;
    }
    statusComunicacao = false;
    Serial.println("Comunicação Serial NOK");

    while(Serial.available()){
        String mensagemSerial = Serial.readString();
        Serial.println("Mensagem Serial Recebida: " + mensagemSerial);
        if(mensagemSerial == "MODOAP"){
            setModoAP(true);
        }
    }
    
}

void verificaAlarme(){
    if(!alarme)
        return;
    if(!statusComunicacao)
        disparaAlarme = true;
}

void movimentoDetectado(){
    if(alarme)
        disparaAlarme = true;
}

void setModoAP(volatile bool ap){modoAP = ap;}
