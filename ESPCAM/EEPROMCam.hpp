/*
    EEPROMCam.hpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#pragma once

#include "defineESPCam.hpp"

void inicializaEEPROM();
void limparMemoria();
void carregarDados();
bool testeCredenciais();
bool escreverNaMemoria (String ssid, String senha, String token, String chatID);
