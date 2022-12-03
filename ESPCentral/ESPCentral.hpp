/*
    ESPCentral.hpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#pragma once

#include <Arduino.h>

void configuracao();
void executar();


void enviaDadosWiFi();
void verificacoes();
void operacoes();
void serialConfig();
void pingConfig();
void inicializarVariaveis();
void testeDisp();
void verificaDadosWiFi();
void verificaConexao();
void verificaComunicacao();
void verificaModoAP();
void verificaAlarme();
void disparaAlarmeComunicacao();
void disparaAlarmeSensor();
void configPinos();
void configModulos();
void verificaEscala();
void verificaCampainha();
void verificaTemperatura();
void atualizaDisplay();

void movimentoDetectado();
void campainhaAcionada();

void setModoAP(volatile bool ap);
void configAlarme(const bool alm);

void setFlagAlarmeDisplay(const bool almdisp);