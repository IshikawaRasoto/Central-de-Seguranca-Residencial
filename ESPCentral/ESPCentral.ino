/*
    ESPCentral.ino - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include "ESPCentral.hpp"
#include "defineESPCentral.hpp"

/*************************************************************
Rotinas de Interrupção
*************************************************************/

void IRAM_ATTR botaoAcessoAP(){
    setModoAP(true);
}

void IRAM_ATTR sensorMovimento(){
    movimentoDetectado();
}

/*************************************************************
Funções Principais
*************************************************************/

void setup(){
    //attachInterrupt(BOTAO_AP, botaoAcessoAP, RISING);
    configuracao();
    pinMode(BOTAO_AP, INPUT);
    attachInterrupt(PINO_MOVIMETO, sensorMovimento, RISING);
}

void loop(){
    if (BOTAO_AP == HIGH){
      setModoAP(true);
    }
    executar();
}
