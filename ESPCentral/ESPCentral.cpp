/*
    ESPCentral.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

volatile bool modoAP;

volatile char statusWiFi;

HardwareSerial EspCam (1);

#include "ESPCentral.hpp"

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
    
}

void serialConfig(){
    Serial.begin(115200);
    EspCam.begin(115200, SERIAL_8N1, RX_CAM, TX_CAM);
}

void inicializarVariaveis(){
    modoAP = false;
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

void testeESPComunicacao(){
    EspCam.print("TesteComunicacao");
    delay(100);
    if(EspCam.available()){
        String mensagem = EspCam.readString();
        if(mensagem == "TesteComunicacao"){
            statusComunicacao = true;
            return;
        }
        statusComunicacao = false;
    }
}