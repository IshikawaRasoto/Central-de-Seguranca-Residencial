/*
    RFIDCentral.hpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <mySD.h>

void configuraRFID();
void verificaRFID();
void verificaTXT();
void verificaTAG();
void salvaTAG();
void cadastraTAG();
void cadastraTAG_SD();
void jaCadastrado();
void ativaRFID();
void ativaSD();
void acessoLiberado();
void acessoNegado();