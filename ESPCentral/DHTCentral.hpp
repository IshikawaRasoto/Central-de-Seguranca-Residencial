/*
    DHTCentral.hpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#pragma once

#define DHTTYPE DHT11

void atualizarDHT(float* umidade, float* temperatura);
void inicializaDHT();
void atualizarDHTTelegram();
