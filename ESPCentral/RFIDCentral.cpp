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

File dados;

String IDtag = ""; 

void configuraRFID_SD(){

    pinMode(CS, OUTPUT); //SD
    pinMode(SDA_ENTRADA, OUTPUT); //RFID
    pinMode(SDA_SAIDA, OUTPUT);
    pinMode(RESET, OUTPUT);

    for (uint8_t reader = 0; reader < 2; reader++) {
    mfrc522[reader].PCD_Init(sdaPins[reader], RESET); 
    }

    ativaRFID();
}

void verificaRFID(){

    mfrc522[0].PICC_HaltA();
    mfrc522[1].PICC_HaltA();

    if (mfrc522[0].PICC_IsNewCardPresent() && mfrc522[0].PICC_ReadCardSerial()) { //ENTRADA
          salvaTAG(0);
          verificaTAG("simples");
          IDtag = "";
    } 

    if (mfrc522[1].PICC_IsNewCardPresent() && mfrc522[1].PICC_ReadCardSerial()) { //SAIDA
          salvaTAG(1);
          verificaTAG("simples");
          IDtag = "";
    } 
    delay(200);
}

void verificaTXT(){

    ativaSD();

    if (!SD.begin(CS)){
        while (!SD.begin(CS)){
            Serial.println ("Erro ao inicializar cartão SD"); //ALTERAR
            delay(1000);
        } 
    }

    else{
        if(!dados){
            dados = SD.open ("/IDtag.txt", FILE_WRITE);
            dados.close();
        }
    }

    ativaRFID();
}




void verificaTAG(String tipo){

    ativaSD();

    String IDtagTemp = "";
    dados = SD.open("/IDtag.txt");


    if (tipo == "simples"){
        if (dados){
            while (dados.available()){
                char letra = dados.read();

                if(isPrintable(letra)){
                    IDtagTemp.concat (letra);
                }

                else if(letra == '\n'){
                    if(IDtag ==  IDtagTemp){ //revisar
                        acessoLiberado();
                        dados.close();
                        ativaRFID();
                        return;
                    }
                    else{
                        IDtagTemp = "";
                    }
                }
            }
            acessoNegado();
            dados.close();
            ativaRFID();
        }
    }
    
    else if (tipo == "complexa"){
        if (dados){
            while (dados.available()){
                char letra = dados.read();

                if(isPrintable(letra)){
                    IDtagTemp.concat (letra);
                }

                else if(letra == '\n'){
                    if(IDtag ==  IDtagTemp){ 
                        jaCadastrado();
                        dados.close();
                        ativaRFID();
                        return;
                    }
                    else{
                        IDtagTemp = "";
                    }
                }
            }
            cadastraTAG_SD();
        }
    }
    
}

void salvaTAG (const int x){
    for (byte i = 0; i < mfrc522[x].uid.size; i++) {        
        IDtag.concat(String(mfrc522[x].uid.uidByte[i], HEX));
    } 
}

void cadastraTAG(){

    while (!mfrc522[1].PICC_IsNewCardPresent() || !mfrc522[1].PICC_ReadCardSerial()){ //APENAS PODE CADASTRAR UMA TAG DENTRO DE CASA
        delay(200);
        //COMPLETAR COM FUNCOES
    }
    salvaTAG(1);
    ativaSD();
    verificaTAG("complexa");

}

void cadastraTAG_SD(){

}

void jaCadastrado(){
    
}

void ativaRFID(){

    digitalWrite(CS, HIGH);
    digitalWrite(SDA_ENTRADA, LOW);
    digitalWrite(SDA_SAIDA, LOW);
    delay(100);
}

void ativaSD(){

    digitalWrite(SDA_ENTRADA, HIGH);//COMPLETAR DESATIVACOES (DESATIVAR DISPLAY, ETC)
    digitalWrite(SDA_SAIDA, HIGH);
    digitalWrite(CS, LOW);
    delay(100);
}

void acessoLiberado(){}
void acessoNegado(){}
