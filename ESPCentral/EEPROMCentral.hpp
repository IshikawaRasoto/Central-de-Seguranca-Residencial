/*
    EEPROMCentral.hpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#pragma once

#include <Arduino.h>

#include "defineESPCentral.hpp"

void inicializaEEPROM();
void limparMemoria();
void carregarDados(String* ssid, String* senha, String* token, String* chatID);
bool testeCredenciais();
bool escreverNaMemoria (String ssid, String senha, String token, String chatID);
