/*
    DHTCentral.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include "DHTCentral.hpp"

#include <DHT.h>
#include "defineESPCentral.hpp"

DHT dht(DHT11_DATA, DHTTYPE)

void atualizarDHT(float* umidade, float* temperatura){
    Serial.println("Atualizar DHT");

    (*umidade) = dht.readHumidity();
    (*temperatura) = dht.readTemperature();

    if(isnan(umidade) || isnan(temperatura)){
        Serial.println("Falha na leitura do Sensor DHT");
        return;
    }

    Serial.println("Temperatura: " + temperatura);
    Serial.println("Umidade %: " + umidade);
}