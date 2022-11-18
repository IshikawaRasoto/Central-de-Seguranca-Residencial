/*
    DHTCentral.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include "DHTCentral.hpp"
#include "WiFiCentral.hpp"

#include <DHT.h>
#include "defineESPCentral.hpp"

DHT dht(DHT11_DATA, DHTTYPE);

void inicializaDHT(){
    Serial.println("Inicializar DHT");
    dht.begin();
}

void atualizarDHT(float* umidade, float* temperatura){
    Serial.println("Atualizar DHT");

    (*umidade) = dht.readHumidity();
    (*temperatura) = dht.readTemperature();

    if(isnan(*umidade) || isnan(*temperatura)){
        Serial.println("Falha na leitura do Sensor DHT");
        return;
    }

    float t = *temperatura;
    float u = *umidade;

    String mensagem = "Temperatura: " + String (t) + "\n";
    mensagem +=  "Umidade: %" + String(u) + "\n";
    
    Serial.print("Temperatura: " + String(t));
    Serial.println("Umidade %: " + String(u));
}

void atualizarDHTTelegram () {
    float u = dht.readHumidity();
    float t = dht.readTemperature();

    if(isnan(u) || isnan(t)){
        mensagemParaTelegram("Falha na leitura do Sensor DHT\n");
        return;
    }

    String mensagem = "Temperatura: " + String (t) + "°C\n";
    mensagem +=  "Umidade: " + String(u) + "%\n";

    mensagemParaTelegram(mensagem);

}
