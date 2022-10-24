/*
    EEPROMCam.hpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include "EEPROMCam.hpp"
#include "defineESPCAM.hpp"
#include <EEPROM.h>

void inicializaEEPROM(){
    EEPROM.begin(400);
}

void limparMemoria(){
    for(unsigned int i = 0; i < 250; i++){
        EEPROM.writeByte(i, 0);
    }
    EEPROM.commit();
}

void carregarDados(String* ssid, String* senha, String* token, String* chatID){
    (*ssid)     = EEPROM.readString(EEPROM_SSID);
    (*senha)    = EEPROM.readString(EEPROM_SENHA);
    (*token)    = EEPROM.readString(EEPROM_TOKEN);
    (*chatID)   = EEPROM.readString(EEPROM_CHATID);
}

bool testeCredenciais(){
    String ssid_teste   = EEPROM.readString(EEPROM_SSID);
    String senha_teste  = EEPROM.readString(EEPROM_SENHA);
    String token_teste  = EEPROM.readString(EEPROM_TOKEN);
    String chatID_teste = EEPROM.readString(EEPROM_CHATID);

    if(ssid_teste.length() > 0 && senha_teste.length() > 0 && token_teste.length() > 0 && chatID_teste.length() > 0)
        return true;
    return false;
}

bool escreverNaMemoria(String ssid, String senha, String token, String chatID){
    Serial.println("escreverNaMemoria");
    
    char buffer1 [TAMANHO_STRING];
    char buffer2 [TAMANHO_STRING];
    char buffer3 [TAMANHO_STRING_TOKEN];
    char buffer4 [TAMANHO_STRING];

    ssid.toCharArray(buffer1, TAMANHO_STRING);
    senha.toCharArray(buffer2, TAMANHO_STRING);
    token.toCharArray(buffer3, TAMANHO_STRING_TOKEN);
    chatID.toCharArray(buffer4, TAMANHO_STRING);

    EEPROM.writeString(EEPROM_SSID, buffer1);
    EEPROM.writeString(EEPROM_SENHA, buffer2);
    EEPROM.writeString(EEPROM_TOKEN, buffer3);
    EEPROM.writeString(EEPROM_CHATID, buffer4);

    EEPROM.commit();

    String ssid_ver     = EEPROM.readString(EEPROM_SSID);
    String senha_ver    = EEPROM.readString(EEPROM_SENHA);
    String token_ver    = EEPROM.readString(EEPROM_TOKEN);
    String chatID_ver   = EEPROM.readString(EEPROM_CHATID);

    if(ssid == ssid_ver && senha == senha_ver && token == token_ver && chatID == chatID_ver)
        return true;
    return false;
}
