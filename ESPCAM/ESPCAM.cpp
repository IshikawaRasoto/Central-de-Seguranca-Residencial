/*
    ESPCAM.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

volatile bool modoAP;
volatile bool statusComunicacao;

volatile char statusWiFi;

HardwareSerial EspCentral (1);

#include "ESPCentral.hpp"

void configuracao(){
    serialConfig();
}

void executar(){
    verificaDadosWiFi();
    operacoes();
    verificaConexao();
    verificaModoAP();
    testeESPComunicacao();
}

void operacoes(){
    
}

void serialConfig(){
    Serial.begin(115200);
    EspCentral.begin(115200, SERIAL_8N1, RX_CENTRAL, TX_CENTRAL);
}

void inicializarVariaveis(){
    modoAP = false;
    statusComunicacao = false;
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
    if(ESPCentral.available()){
        String mensagem = ESPCentral.readString();
        
        if(modoAP){
                
        }
        
        switch(mensagem){
            case "TesteComunicacao":
                ESPCentral.print(mensagem);
                break;
            case "modoAP":
                limparMemoria();
                modoAP = true;
                break;
        }
    }
}
