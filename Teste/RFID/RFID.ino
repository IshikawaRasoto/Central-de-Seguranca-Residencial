#include <MFRC522.h>
#include <SPI.h>
#include <mySD.h>

using namespace ext;

#define SS_RFID   21
#define RST_PIN   22


#define SS_SD     5

File dados;

MFRC522 mfrc522(SS_RFID, RST_PIN); 

void setup() {
  // Inicia a serial
  pinMode (12, OUTPUT);
  pinMode(SS_RFID, OUTPUT);
  pinMode(SS_SD, OUTPUT);
  pinMode(RST_PIN, OUTPUT);
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  
  // Inicia MFRC522
  mfrc522.PCD_Init(); 
  // Mensagens iniciais no serial monitor
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  digitalWrite (12, LOW);
  digitalWrite (SS_RFID, HIGH);
  digitalWrite (SS_SD, LOW);

  if (!SD.begin(SS_SD)){
    while (!SD.begin(SS_SD)){
      Serial.println ("Erro ao inicializar cartão SD"); 
      delay(1000);
    } 
  }
  digitalWrite (SS_SD, HIGH);
  digitalWrite (SS_RFID, LOW);
  Serial.println("Esperando cartão...");
  
}

void loop() {
  String IDtag = "";    
  mfrc522.PICC_HaltA();
  if(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){    
  for (byte i = 0; i < mfrc522.uid.size; i++) {        
    IDtag.concat(String(mfrc522.uid.uidByte[i], HEX));
  }   
  Serial.print("O ID da tag é: ");
  Serial.println(IDtag);
  digitalWrite (12, HIGH);
  delay(2000);
  digitalWrite (12, LOW);
  
  digitalWrite (SS_RFID, HIGH);
  digitalWrite (SS_SD, LOW);
  delay(200);
 
  Serial.println("Salvando dados...");
  dados = SD.open ("/dados.txt", FILE_WRITE);
  if (!dados){
    Serial.println("Falha ao abrir dados");
    return;
  }
    dados.println(IDtag);
    dados.close();
    Serial.println("Cadastro salvo!");
    Serial.println("");
    Serial.println("Esperando cartão...");
    delay(200);
  
  while (!SD.exists("/dados.txt")){
    Serial.println("nao existe dados.txt");
    delay(500);
  }

  
  digitalWrite (SS_SD, HIGH);
  digitalWrite (SS_RFID, LOW);
  delay(200);
  }
}
