/*
    RFIDCentral.hpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#pragma once

#include <Arduino.h>
#include <SPI.h>
//#include<mySD.h>
#include <SD.h>

void configuraRFID_SD();
void verificaRFID();
void verificaTXT();
void verificaTAG(String tipo);
void salvaTAG(const int x);
void cadastraTAG();
void cadastraTAG_SD();
void jaCadastrado();
void acessoLiberado();
void acessoNegado();
