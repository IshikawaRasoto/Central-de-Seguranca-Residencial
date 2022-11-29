/*
    ESPCentral.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include "defineESPCentral.hpp"
#include "ESPCentral.hpp"
#include "EEPROMCentral.hpp"
#include "WiFiCentral.hpp"
#include "DHTCentral.hpp"
#include "RFIDCentral.hpp"

//Variáveis de Wi-Fi
String ssid;
String senha;
String chatID;
String token;


//Variáveis do Sistema
volatile bool modoAP;
bool alarme;
bool flagDisparaAlarme;
//volatile char statusWiFi;
bool statusComunicacao;

unsigned int timerEscala;

//Variáveis DHT11
float temperatura;
float umidade;

// Configuração

void configuracao(){
    serialConfig();
    configPinos();
    inicializaEEPROM();
    carregarDados(&ssid, &senha, &token, &chatID);
    enviaDadosWiFi();
    inicializarVariaveis();
    configModulos();


    conectaWiFi();
    testeConexao();
    conectaTelegram();
}

void serialConfig(){
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, RX_CAM, TX_CAM);
}


void inicializarVariaveis(){
    modoAP              = false;
    statusComunicacao   = true;
    alarme              = false;
    flagDisparaAlarme       = false;
    setStatusWiFi(SEM_WIFI);
    timerEscala = 0;
}

void configPinos(){
    pinMode(BOTAO_AP, INPUT);
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);
}

void configModulos(){
    SPI.begin();
    configuraRFID_SD();
    verificaTXT();
    inicializaDHT();
}

void enviaDadosWiFi(){
    delay(10000);
    Serial.println("SSID enviado: " + ssid);
    Serial1.print(ssid);
    delay(2000);
    Serial.println("Senha Enviada: " + senha);
    Serial1.print(senha);
    delay(2000);
    Serial.println("Token enviado: " + token);
    Serial1.print(token);
    delay(5000);
    Serial.println("ChatID Enviado: " + chatID);
    Serial1.print(chatID);
    delay(2000);
}

// Execução
void executar(){
    verificacoes();
    operacoes();
    
}

void operacoes(){
    atualizarDHT(&umidade, &temperatura);
    verificaRFID();
    mensagemTelegram();
}

void verificacoes(){
    verificaModoAP();
    verificaConexao();
    verificaComunicacao();
    verificaAlarme();
    verificaEscala();
}

void verificaConexao(){
    Serial.println("Verifica Conexão");
    testeConexao();
}

void verificaModoAP(){
    Serial.println("Verifica modo AP");
    if(modoAP)
        limparMemoria();
    if(testeCredenciais())
        return;
    modoAP = true;
    while(enviaFormulario());
}

void verificaComunicacao(){
    Serial1.print("Teste");
    //Serial.println("TesteComunicacao");
    delay(1250);
    int i;
    for(i = 0; i < 7 && !Serial1.available(); i++){
        delay(2500);
        Serial.print(i);
    }
    if(i == 7){
        Serial.println("SERIAL NOK");
        statusComunicacao = false;
        flagDisparaAlarme = true;
        return;
    }

    flagDisparaAlarme = false;


    String mensagem = Serial1.readString();
    Serial.println(mensagem);
    if(mensagem == "NETOK"){
        statusComunicacao = true;
        Serial.println("Comunicação Serial OK");
        return;
    }else if(mensagem == "NETNOK"){
        statusComunicacao = true;
        Serial.println("Comunicação Serial OK e ESPCAM NET NOK");
        mensagemParaTelegram("ESPCAM sem Internet!");
        return;
    }else if(mensagem == "WIFINOK"){
         statusComunicacao = true;
         Serial.println("Comunicação Serial OK e ESPCAM SEM WIFI");
         mensagemParaTelegram("ESPCAM sem Wi-Fi!");
         return;
    }
/*
    while(Serial.available()){
        String mensagemSerial = Serial.readString();
        Serial.println("Mensagem Serial Recebida: " + mensagemSerial);
        if(mensagemSerial == "MODOAP"){
            setModoAP(true);
        }
    }
*/
}

void verificaAlarme(){
    if(!statusComunicacao){
        flagDisparaAlarme = true;
        disparaAlarmeComunicacao();
    }     
}

void disparaAlarmeComunicacao(){
    if (alarme && flagDisparaAlarme){
        digitalWrite(BUZZER, HIGH);
        mensagemParaTelegram("ALARME DISPARADO (ESPCAM DESCONECTADA)!");
    }
    else if (!alarme && flagDisparaAlarme){
        digitalWrite(BUZZER, LOW);
        mensagemParaTelegram("ALARME DESLIGADO (ESPCAM DESCONECTADA)!");
    }
    
    
}

void verificaEscala(){
    timerEscala++;
    if(timerEscala == 5){
        Serial.println("Verificando Escala...");
        timerEscala = 0;
        verificaEscalaUsuarios(); 
    }
}


void movimentoDetectado(){
    if(alarme)
        flagDisparaAlarme = true;
}



void setModoAP(volatile bool ap){modoAP = ap;}
void configAlarme(const bool alm){alarme = alm; if(!alm) flagDisparaAlarme = alm;}
