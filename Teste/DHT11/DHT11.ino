/*
    DHT11.h - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include <DHT.h>

#define DHTTYPE DHT11
#define DHTPIN 25

DHT dht(DHTPIN, DHTTYPE);

void setup(){
    Serial.begin(115200);
    dht.begin();
}

void loop(){
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print("Humidity %: ");
  Serial.println(h);
  Serial.print("Temperature: ");
  Serial.println(t);

  delay(5000);
}
