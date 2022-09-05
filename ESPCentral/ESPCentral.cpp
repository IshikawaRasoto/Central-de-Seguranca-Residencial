/*
    ESPCentral.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

volatile bool modoAP;

volatile char statusWiFi;

#include "ESPCentral.hpp"

void configuracao(){
    serialConfig();
}

void executar(){
    verificaDadosWiFi();
    operacoes();
    verificaConexao();
    verificaModoAP();
}

void operacoes(){
    
}

void serialConfig(){
    Serial.begin(115200);
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