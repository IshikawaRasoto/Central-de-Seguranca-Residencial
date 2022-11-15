/*
    WiFiCam.hpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#pragma once

#include <Arduino.h>

void conectaWiFi();
void testeConexao(volatile char* statusWiFi);
void conectaTelegram();
void informacoesWiFi();
String sendPhotoTelegram();
