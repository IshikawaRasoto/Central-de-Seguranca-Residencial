/*
    SensorDePresenca.ino - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#define PINO_SENSOR_PRESENCA 25

void IRAM_ATTR sensorDePresenca(){
    Serial.println("Interrupcao");
}

void setup(){
    Serial.begin(115200);

    attachInterrupt(PINO_SENSOR_PRESENCA, sensorDePresenca, RISING);
}

void loop (){
    Serial.println("Hello World!");
    delay(1000);    
}