/*
    ESPCentral.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

volatile bool modoAP;

volatile char statusWiFi;

bool statusComunicacao;

float temperatura;
float umidade;

#include "defineESPCentral.hpp"
#include "ESPCentral.hpp"
#include "EEPROMCentral.hpp"
#include "WiFiCentral.hpp"
#include "DHTCentral.hpp"

void configuracao(){
    serialConfig();
}

void executar(){
    testeESPComunicacao();
    operacoes();
    verificaConexao();
    verificaModoAP();
}

void operacoes(){
    atualizarDHT();
}

void serialConfig(){
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, RX_CAM, TX_CAM);
}

void inicializarVariaveis(){
    modoAP = false;
}

void verificaConexao(){
    testeConexao(&statusWiFi);
}

void verificaModoAP(){
    if(modoAP){
        limparMemoria();
        Serial1.print("ModoAP");
    }
    if(testeCredenciais())
        return;
    modoAP = true;
    while(enviaFormulario());
}

void testeESPComunicacao(){
    Serial1.print("TesteComunicacao");
    delay(250);
    while(Serial1.available()){
        String mensagem = Serial1.readString();
        if(mensagem == "TesteComunicacao"){
            statusComunicacao = true;
            Serial.println("Comunicação Serial OK");
            return;
        }
        statusComunicacao = false;
        Serial.println("Comunicação Serial NOK");
    }
}

void setModoAP(volatile bool ap){
   modoAP = ap;
}
