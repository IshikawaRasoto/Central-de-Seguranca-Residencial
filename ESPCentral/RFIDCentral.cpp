/*
    RFIDCentral.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include "defineESPCentral.hpp"
#include "RFIDCentral.hpp"
#include <MFRC522.h>

using namespace ext;

byte sdaPins[] = {SDA_ENTRADA, SDA_SAIDA};

MFRC522 mfrc522[2];

File usuarios;

String IDtag = ""; 

void verificaTXT(){

    digitalWrite(SDA_ENTRADA, HIGH);
    digitalWrite(SDA_SAIDA, HIGH);
    digitalWrite(CS, LOW);

    if (!SD.begin(CS)){
        while (!SD.begin(CS)){
            Serial.println ("Erro ao inicializar cartão SD"); //ALTERAR
            delay(1000);
        } 
    }

    else{
        if(!usuarios){
            usuarios = SD.open ("/usuarios.txt", FILE_WRITE);
            usuarios.close();
        }
    }

    digitalWrite(CS, HIGH);
    digitalWrite(SDA_ENTRADA, LOW);
    digitalWrite(SDA_SAIDA, LOW);
}

void configuraRFID_SD(){

    pinMode(CS, OUTPUT); //SD
    pinMode(SDA_ENTRADA, OUTPUT); //RFID
    pinMode(SDA_SAIDA, OUTPUT);
    pinMode(RESET, OUTPUT);

    for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RESET); 
    }
}

void verificaRFID(){

    mfrc522.PICC_HaltA();

    if (mfrc522[0].PICC_IsNewCardPresent() && mfrc522[0].PICC_ReadCardSerial()) {
          salvaTAG(0);
          verificaTAG();
    } 

    if (mfrc522[1].PICC_IsNewCardPresent() && mfrc522[1].PICC_ReadCardSerial()) {
          salvaTAG(1);
          verificaTAG();
    } 
    delay(200)
}

void salvaTAG (const int x){
    for (byte i = 0; i < mfrc522[x].uid.size; i++) {        
        IDtag.concat(String(mfrc522.uid.uidByte[i], HEX));
    } 
}

void verificaTAG(){

    digitalWrite(SDA_ENTRADA, HIGH);//COMPLETAR DESATIVACOES
    digitalWrite(SDA_SAIDA, HIGH);
    digitalWrite(CS, LOW);

    String IDtagTemp = "";
    usuarios = SD.open("/usuarios.txt");

    if (usuarios){
        while (usuarios.available()){
            char letra = usuarios.read();

            if(isPrintable(letra)){
                IDtagTemp.concat (letra);
            }

            else if(letra == '/'){
                
            }

        }
    }
}