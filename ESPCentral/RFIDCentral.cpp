/*+
    RFIDCentral.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include "defineESPCentral.hpp"
#include "RFIDCentral.hpp"
#include "WiFiCentral.hpp"
#include "RTCCentral.hpp"
#include <MFRC522.h>

bool resetEscala = false;
 bool flagCadastro;
 String nomeCadastro = "";
 int tipoEntrada;
//using namespace ext;

byte sdaPins[] = {SDA_ENTRADA, SDA_SAIDA};

MFRC522 mfrc522[2];

SPIClass SPI2 (HSPI);

File dados;

String IDtag = ""; 

Usuarios *vetorUsuarios[MAX_USUARIOS];

int numVetorUsuarios = 0;

bool jaCadastradoFlag = false;
bool tipoCadastro = false;

void configuraRFID_SD(){

    Serial.println("Configura RFID");

    pinMode(SD_CS_PIN, OUTPUT);
    pinMode(CS, OUTPUT); //SD
    pinMode(SDA_ENTRADA, OUTPUT); //RFID
    pinMode(SDA_SAIDA, OUTPUT);
    pinMode(RESET, OUTPUT);
    pinMode (14, INPUT); //BOTAO TEMPORARIO
    flagCadastro = false;

    SPI2.begin(SD_CLK_PIN , SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);

    SD.begin(SD_CS_PIN, SPI2);

    
    for (uint8_t reader = 0; reader < 2; reader++) {
        mfrc522[reader].PCD_Init(sdaPins[reader], RESET);
        Serial.print(F("Reader "));
        Serial.print(reader);
        Serial.print(F(": "));
        mfrc522[reader].PCD_DumpVersionToSerial();
    }
}

void verificaRFID(){

    if(flagCadastro && numVetorUsuarios < MAX_USUARIOS){
      Serial.println("Aproxime a TAG para cadastrar");
      cadastraTAG();
      flagCadastro = false;
    }else{

      if (mfrc522[0].PICC_IsNewCardPresent() && mfrc522[0].PICC_ReadCardSerial()) { //ENTRADA
            Serial.println("RFID 0:");
            salvaTAG(0);
            Serial.println(IDtag);
            tipoEntrada = 0;
            verificaTAG("simples");
            IDtag = "";
            mfrc522[0].PICC_HaltA();
      } 
    
      if (mfrc522[1].PICC_IsNewCardPresent() && mfrc522[1].PICC_ReadCardSerial()) { //SAIDA
            Serial.println("RFID 1:");
            salvaTAG(1);
            Serial.println(IDtag);
            tipoEntrada = 1;
            verificaTAG("simples");
            IDtag = "";
            mfrc522[1].PICC_HaltA();
      } 
    }
    delay(100);
}

void verificaTXT(){

    Serial.println("Verifica TXT");
    
    if (!SD.begin(SD_CS_PIN)){
        while (!SD.begin(SD_CS_PIN)){
            Serial.println ("Erro ao inicializar cartão SD"); //ALTERAR
            delay(1000);
        } 
    }

    else{
        if(!SD.exists("/Dados.txt")){
            Serial.println("Dados.txt está sendo criado");
            dados = SD.open ("/Dados.txt", FILE_WRITE);
            dados.close();
        }
    }
    configuraVetorUsuarios();

}




void verificaTAG(String tipo){

    String IDtagTemp = "";
    dados = SD.open("/Dados.txt");


    if (tipo == "simples"){
        if (dados){
            while (dados.available()){
                char letra = dados.read();

                if(isPrintable(letra)){
                    IDtagTemp.concat (letra);
                }

                else if(letra == '\n'){
                    if(IDtag ==  IDtagTemp){ //revisar
                        dados.close();
                        String total = "/" + IDtag + ".txt";
                        Serial.println("total = " + total);
                        char totalChar[15];

                        total.toCharArray(totalChar, 15);
                        
                        dados = SD.open (totalChar);
                        IDtagTemp = "";
                        while (dados.available()){
                            Serial.println("Entrou no loop");
                            char letra1 = dados.read();
                        
                            if(isPrintable(letra1)){
                                if(letra1 == '/'){
                                  Serial.println("Nome lido = "+ IDtagTemp);
                                  Serial.print("TipoEntrada = ");
                                  Serial.println(tipoEntrada);
                                  dados.close();
                                  int i = 0;

                                  for (i = 0; i < numVetorUsuarios; i++){
                                    if(vetorUsuarios[i]->ID == IDtag){
                                      break;
                                    }
                                  }
                                  if(tipoEntrada == 1){
                                    vetorUsuarios[i]->emCasa = false;
                                  }

                                  else if (tipoEntrada == 0){
                                    vetorUsuarios[i]->emCasa = true;
                                  }
                                  acessoLiberado();
                                  acessoLiberadoWiFi(IDtagTemp, tipoEntrada);
                                  
                                }
                                else {
                                  IDtagTemp.concat (letra1);
                                }
                 
                            }
                        }
                        return;
                    }
                    else{
                        IDtagTemp = "";
                    }
                }
            }
            acessoNegado();
            dados.close();
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
                        return;
                    }
                    else{
                        IDtagTemp = "";
                    }
                }
            }
            dados.close();
            //cadastraTAG_SD();ERRADO
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
        Serial.println("Esperando...");
        //COMPLETAR COM FUNCOES
    }
    salvaTAG(1);
    verificaTAG("complexa");
    if (jaCadastradoFlag){
        jaCadastradoFlag = false;
        IDtag = "";
    }
    else {
        cadastraTAG_SD();
    }
    

}

void cadastraTAG_SD(){
    dados = SD.open("/Dados.txt", FILE_APPEND);
    if (dados){
      Serial.print("Cadastrando TAG: ");
      Serial.println(IDtag);
      dados.println(IDtag);
      dados.close(); 
    }
    String IDtagTotal = "/" + IDtag + ".txt";

    char IDtagChar[15];
    IDtagTotal.toCharArray(IDtagChar, 15);
    dados = SD.open(IDtagChar, FILE_WRITE);
    dados.close();
    String mensagem = "Insira o nome do Usuário\n";
    mensagemParaTelegram(mensagem);
    tipoCadastro = true;
    while (!cadastroTelegram(tipoCadastro)){
    }
    tipoCadastro =false;
    dados = SD.open(IDtagChar, FILE_APPEND);
    dados.print(nomeCadastro);
    dados.println(IDtag);
    dados.close();
    for (int i = 0; i < MAX_USUARIOS; i++){
      vetorUsuarios[i] = NULL;
    }
    numVetorUsuarios = 0;
    configuraVetorUsuarios();
    mensagem = "TAG cadastrada com sucesso!\n";
    mensagemParaTelegram(mensagem);
    IDtag = "";
}


void jaCadastrado(){
    jaCadastradoFlag = true;
    Serial.println("TAG ja cadastrada");
    String mensagem = "TAG já cadastrada!";
    mensagemParaTelegram(mensagem);
}

void acessoLiberado(){
    Serial.println("Cartão encontrado");
    digitalWrite (BUZZER, LOW);
    delay(10);
    digitalWrite (BUZZER, HIGH);
    delay(100);
    digitalWrite (BUZZER, LOW);
    delay(100);
    digitalWrite (BUZZER, HIGH);
    delay(100);
    digitalWrite (BUZZER, LOW);
    Serial1.print("abrir");
    
}
void acessoNegado(){
    digitalWrite (BUZZER, HIGH);
    delay(1000);
    digitalWrite (BUZZER, LOW);
    Serial.println("Cartão não encontrado");
    String mensagem = "Alguém utilizou uma TAG não cadastrada na residência!";
    mensagemParaTelegram(mensagem);
    Serial1.print("foto");
    
}

void setCadastro(const bool x){
    flagCadastro = x;
}

void salvaNomeCadastro (String nome){
    nomeCadastro = nome + "/";
}

void listaTelegram(){

    String lista = "Lista de usuários no formato (Nome/ID):\n";
    String IDtagTemp = "";
    dados = SD.open("/Dados.txt");

    if (dados){
        while(dados.available()){
            char letra = dados.read();

            if (isPrintable(letra)){
                IDtagTemp.concat(letra);
            }

            else if(letra == '\n'){
                String IDtagTotal = "/" + IDtagTemp + ".txt";
                char IDtagChar[15];
                IDtagTotal.toCharArray(IDtagChar, 15);
                File dadosTemp = SD.open (IDtagChar);
                IDtagTemp = "";
                while (dadosTemp.available()){
                  char letraTemp = dadosTemp.read();

                  if (isPrintable(letraTemp)){
                    IDtagTemp.concat(letraTemp);
                  }

                  else if (letraTemp == '\n'){
                    lista += "-";
                    lista += IDtagTemp;
                    lista += "\n";
                    dadosTemp.close();
                  }
                }
                IDtagTemp = "";
            }
        }
        dados.close();
        mensagemParaTelegram (lista);
        Serial.print(lista);
    }

}

void deletaUsuario(String texto){
    Serial.println("Deleta Usuario: " + texto);
    File dadosTemp;
    dadosTemp = SD.open("/dadosTemp.txt", FILE_WRITE);
    dadosTemp.close();
    bool flagID = true;
    String IDtagTemp = "";
    String IDtagTotal = "/" + texto + ".txt";
    Serial.println("IDtagTotal: " + IDtagTotal);
    char IDtagChar[15];
    IDtagTotal.toCharArray(IDtagChar, 15);
    SD.remove(IDtagChar);
    dados = SD.open("/Dados.txt");
    if (dados){
      while (dados.available()){
        char letra = dados.read();

        if (isPrintable(letra)){
          IDtagTemp.concat(letra);
        }

        else if(letra == '\n'){
          if (IDtagTemp == texto){
            mensagemParaTelegram ("Deletando cadastro...");
            IDtagTemp = "";
            flagID = false;
          }
          else {
            dadosTemp = SD.open("/dadosTemp.txt", FILE_APPEND);
            if (dadosTemp){
              Serial.println("Printando em dadosTemp a tag: " + IDtagTemp);
              dadosTemp.println(IDtagTemp);
              dadosTemp.close();
            }
            IDtagTemp = "";
          }
        }
      }
      dados.close();
    }
    SD.remove ("/Dados.txt");
    
    delay(15);
    dados = SD.open ("/Dados.txt", FILE_WRITE);
    dados.close();
    IDtagTemp = "";
    dadosTemp = SD.open("/dadosTemp.txt");
    if(dadosTemp){
      while(dadosTemp.available()){
        char letra = dadosTemp.read();

        if (isPrintable(letra)){
          IDtagTemp.concat(letra);
        }

        else if(letra == '\n'){
          dados = SD.open("/Dados.txt", FILE_APPEND);
          if (dados){
            Serial.println("Printando em Dados a tag: " + IDtagTemp);
            dados.println(IDtagTemp);
            dados.close();
          }
          IDtagTemp = "";
        }
      }
      dadosTemp.close();
    }
    SD.remove("/dadosTemp.txt");
    if (flagID){
      mensagemParaTelegram ("ID não encontrada, digite o comando novamente e insira uma ID válida.");
    }
    else {
      for(int i =0; i < MAX_USUARIOS; i++){
        vetorUsuarios[i] = NULL;
      }
      numVetorUsuarios = 0;
      configuraVetorUsuarios();
      mensagemParaTelegram ("Usuário deletado com sucesso!");
    }
}


void configuraVetorUsuarios(){
    Serial.println("Configura Vetor Usuários");
    String texto = "";
    dados = SD.open("/Dados.txt");

    if (dados){
      Serial.println("DADOS");
      while (dados.available()){
        char letra = dados.read();

        if (isPrintable(letra)){
          texto.concat(letra);
        }
        
        else if (letra == '\n'){
          Serial.println("ID Arquivo: " + texto);
          bool flagEscala = false;
          vetorUsuarios[numVetorUsuarios] = new Usuarios();
          String textoTotal = "/" + texto + ".txt";
          char textoChar[15];
          int i = 0;
          textoTotal.toCharArray(textoChar, 15);
          File dadosTemp = SD.open(textoChar);
          texto = "";
          if(dadosTemp){
            while(dadosTemp.available()){
              char letraTemp = dadosTemp.read();
              if(isPrintable(letraTemp)){
                if(letraTemp == '/'){
                  vetorUsuarios[numVetorUsuarios]->Nome = texto;
                  Serial.println("Nome:" + texto);
                  Serial.println("NOME VETOR: " + vetorUsuarios[numVetorUsuarios]->Nome);
                  texto = "";
                }

                else if(letraTemp == ':'){
                  flagEscala = true;
                  vetorUsuarios[numVetorUsuarios]->Sair = texto;
                  Serial.println("SAIR: " + vetorUsuarios[numVetorUsuarios]->Sair);
                  texto = "";
                }

                else {
                  texto.concat(letraTemp);
                }
              }

              else if (letraTemp == '\n'){
                if(i == 0){
                  vetorUsuarios[numVetorUsuarios]->ID = texto;
                  Serial.println("ID: " + vetorUsuarios[numVetorUsuarios]->ID);
                  texto = "";
                  i++;
                }

                else if(i == 1){
                  vetorUsuarios[numVetorUsuarios]->Entrar = texto;
                  Serial.println("ENTRAR: " + vetorUsuarios[numVetorUsuarios]->Entrar);
                  texto = "";
                }
              }
            }
            dadosTemp.close();
          }
          if (flagEscala){
            vetorUsuarios[numVetorUsuarios]->flagEntrar = true; // possui um leve erro
            vetorUsuarios[numVetorUsuarios]->flagSair = true;
            vetorUsuarios[numVetorUsuarios]->escala = true;
          }
          numVetorUsuarios ++;
          texto = "";
        }
      }
    dados.close();
    }
}

Usuarios::Usuarios(String ID):
    Nome(""),
    ID(ID),
    Sair(""),
    escala(false),
    flagSair(false),
    Entrar(""),
    flagEntrar(false),
    emCasa(true)
{}

Usuarios::~Usuarios(){}

void novaEscala (String texto){
    String id = texto.substring(0, texto.indexOf('/'));
    String horario = texto.substring(texto.indexOf('/') + 1);

    Serial.println("ID acessado: " + id);
    Serial.println("Horario salvo: " + horario);

    String idTotal = "/" + id + ".txt"; 
    char idChar[15];

    Serial.println("Arquivo a ser aberto: " + idTotal);

    idTotal.toCharArray (idChar, 15);

    dados = SD.open (idChar, FILE_APPEND);

    if (dados){
      dados.println(horario);
      dados.close();
      String horaEntrada  = horario.substring(0, horario.indexOf(':'));
      String horaSaida    = horario.substring(horario.indexOf(':')+1);
      for (int i = 0; i < numVetorUsuarios; i++){
        if (vetorUsuarios[i]->ID == id){
          vetorUsuarios[i]->Entrar      = horaEntrada;
          vetorUsuarios[i]->Sair        = horaSaida;
          vetorUsuarios[i]->flagEntrar  = true;
          vetorUsuarios[i]->flagSair    = true;
          vetorUsuarios[i]->escala      = true;
        }
      }
      mensagemParaTelegram("Horário salvo com sucesso!");
    }

    else {
      mensagemParaTelegram("Erro ao salvar o horário!");
    }

}

void imprimeUsuario(String texto){
    String idTotal = "/" + texto + ".txt";

    char idChar[15];
    idTotal.toCharArray(idChar, 15);
    dados = SD.open(idChar);
    texto = "";

    if (dados){
      while(dados.available()){
        char letra = dados.read();
        texto.concat(letra);
      }
    }
    mensagemParaTelegram("Dados do usuário:\n");
    mensagemParaTelegram(texto);

}

void verificaEscalaUsuarios (){

    Serial.println("Verifica Escala Usuários");
    String horario = getHorario();
    String hora = horario.substring(0, horario.indexOf(':'));

    if(hora.toInt() == 0 && resetEscala){
        Serial.println("Reseta Escala");
        for(int i = 0; i < numVetorUsuarios; i++){
          if(vetorUsuarios[i]->escala){
            vetorUsuarios[i]->flagEntrar = true;
            vetorUsuarios[i]->flagSair   = true;
          }
        }
        resetEscala = false;
    }else if (hora != 0){
        resetEscala = true;
    }

    for (int i = 0; i<numVetorUsuarios; i++){
      Serial.println("Hora: " + String(hora.toInt()) + "Horario de Saida: " + vetorUsuarios[i]->Sair);
      if ((vetorUsuarios[i]->flagSair == true) && (vetorUsuarios[i]->Sair.toInt() <= hora.toInt())){
        vetorUsuarios[i]->flagSair = false;
        Serial.println("Verificou Saida");
        if(vetorUsuarios[i]->emCasa){
          String texto = vetorUsuarios[i]->Nome;
          texto += " não saiu da residência no horário previsto";
          mensagemParaTelegram(texto);
        }
      }

      if ((vetorUsuarios[i]->flagEntrar == true) && (vetorUsuarios[i]->Entrar.toInt() <= hora.toInt())){
        vetorUsuarios[i]->flagEntrar = false;
        if(!(vetorUsuarios[i]->emCasa)){
          String texto = vetorUsuarios[i]->Nome;
          texto += " não chegou na residência no horário previsto";
          mensagemParaTelegram(texto);
        }
      }
    }
}
