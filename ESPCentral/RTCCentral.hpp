/*
    RTCCentral.hpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#pragma once

#include <Arduino.h>
#include <time.h> //Necessária no Arquivo Principal

void inicializaRTC();
void setHorario(const int dia, const int mes, const int ano, const int h, const int m, const int s);

String getHorario();
String getData();