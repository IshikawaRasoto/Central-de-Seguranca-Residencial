/*
    ArduinoDisplay.ino - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(8,9,10,11,12);

static const unsigned char PROGMEM wifi_ok[] = 
{
  0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x0f, 0xfe, 0x00, 0x1f, 0xff, 0x80, 0x3e, 0x0f, 0xc0, 0x78, 
  0x03, 0xe0, 0x70, 0x01, 0xe0, 0x61, 0xf8, 0xc0, 0x07, 0xfc, 0x00, 0x0f, 0xff, 0x00, 0x1f, 0x1f, 
  0x00, 0x0c, 0x06, 0x00, 0x00, 0x40, 0x00, 0x00, 0xe0, 0x00, 0x01, 0xf0, 0x00, 0x01, 0xf0, 0x00, 
  0x01, 0xf0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM wifi_nok[] = 
{
  0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x70, 0xf0, 0x00, 0x7b, 0xfe, 0x00, 0x3f, 0xff, 0xc0, 0x7f, 
  0xff, 0xe0, 0xff, 0x7f, 0xf0, 0xff, 0x83, 0xf0, 0x73, 0xc0, 0xe0, 0x27, 0xfe, 0x40, 0x0f, 0xff, 
  0x00, 0x0f, 0xfb, 0x00, 0x07, 0xbe, 0x00, 0x02, 0x1e, 0x00, 0x00, 0x6f, 0x00, 0x00, 0xf7, 0x80, 
  0x01, 0xfb, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM no_wifi[] = 
{
  0x03, 0xfc, 0x00, 0x0f, 0xff, 0x00, 0x1b, 0x6d, 0x80, 0x36, 0x66, 0xc0, 0x7c, 0x63, 0xe0, 0x4e, 
  0x67, 0x20, 0xcf, 0xff, 0x30, 0x8c, 0x63, 0x10, 0x8c, 0x63, 0x10, 0xff, 0xff, 0xf0, 0xff, 0xff, 
  0xf0, 0x8c, 0x63, 0x10, 0x8c, 0x63, 0x10, 0xcf, 0xff, 0x30, 0x4e, 0x67, 0x20, 0x7c, 0x63, 0xe0, 
  0x36, 0x66, 0xc0, 0x1b, 0x6d, 0x80, 0x0f, 0x6f, 0x00, 0x03, 0xfe, 0x00
};

static const unsigned char PROGMEM com_alarme[] =
{
  0x10, 0x00, 0x80, 0x3a, 0x05, 0xc0, 0x7e, 0x07, 0xe0, 0x39, 0xf9, 0xc0, 0x3b, 0xfe, 0xc0, 0x36, 
  0x06, 0xc0, 0x0c, 0x03, 0x00, 0x0c, 0x03, 0x00, 0x0c, 0x03, 0x00, 0x0c, 0x03, 0x00, 0x0c, 0x03, 
  0x00, 0x08, 0x01, 0x80, 0x18, 0x01, 0x80, 0x30, 0x00, 0xc0, 0x38, 0x01, 0xc0, 0x1f, 0xff, 0x80, 
  0x0f, 0xff, 0x00, 0x01, 0x98, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x60, 0x00
};

static const unsigned char PROGMEM sem_alarme[] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x60, 0x00, 0xf0, 0x70, 0x00, 0x7b, 0xfc, 0x00, 0x3f, 
  0xfc, 0x00, 0x1f, 0xfe, 0x00, 0x0f, 0xfe, 0x00, 0x03, 0xfe, 0x00, 0x01, 0xfe, 0x00, 0x0c, 0xff, 
  0x00, 0x0f, 0x3f, 0x00, 0x0f, 0x9f, 0x80, 0x1f, 0xcf, 0x80, 0x1f, 0xf3, 0xc0, 0x00, 0x01, 0xe0, 
  0x00, 0xf0, 0xf0, 0x00, 0xf0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM dispara_alarme[] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 
  0x00, 0xff, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x01, 0xc3, 0x80, 0x00, 0x00, 0x03, 
  0xc3, 0xc0, 0x00, 0x00, 0x03, 0x81, 0xc0, 0x00, 0x00, 0x07, 0x81, 0xe0, 0x00, 0x00, 0x0f, 0x00, 
  0xf0, 0x00, 0x00, 0x0f, 0x00, 0xf0, 0x00, 0x00, 0x1e, 0x00, 0x78, 0x00, 0x00, 0x1e, 0x00, 0x38, 
  0x00, 0x00, 0x3c, 0x00, 0x3c, 0x00, 0x00, 0x38, 0x18, 0x1c, 0x00, 0x00, 0x78, 0x18, 0x1e, 0x00, 
  0x00, 0x70, 0x18, 0x0e, 0x00, 0x00, 0xf0, 0x18, 0x07, 0x00, 0x01, 0xe0, 0x18, 0x07, 0x80, 0x01, 
  0xc0, 0x18, 0x03, 0x80, 0x03, 0xc0, 0x18, 0x03, 0xc0, 0x03, 0x80, 0x18, 0x01, 0xc0, 0x07, 0x80, 
  0x18, 0x01, 0xe0, 0x0f, 0x00, 0x18, 0x00, 0xf0, 0x0f, 0x00, 0x18, 0x00, 0xf0, 0x1e, 0x00, 0x18, 
  0x00, 0x78, 0x1c, 0x00, 0x00, 0x00, 0x38, 0x3c, 0x00, 0x18, 0x00, 0x3c, 0x38, 0x00, 0x3c, 0x00, 
  0x1c, 0x78, 0x00, 0x3c, 0x00, 0x1e, 0xf0, 0x00, 0x1c, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x07, 
  0xe0, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x07, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


void mostrarModoAP()
{
  /*Refazer proporcionalidade na tela e rever informações*/
  display.setCursor(0,0);
  display.setTextSize(2); 
  display.println("MODO AP");
  display.setTextSize(0.5);
  display.println("ssid: Central_De_Seguranca");
  display.println("senha: 12345678");
  display.println("link: 192.168.4.1");
  display.display();   
}

void mostrarDisparo()
{
  display.drawBitmap(25,0, dispara_alarme, 40, 40, 1);
  display.setCursor(20,40);
  display.setTextSize(0.2); 
  display.println("!Alarme!"); 
  display.display(); 
}

void mostrarNomeProjeto()
{
  display.setCursor(0,35);
  display.setTextSize(0.2);
  display.println ("Central de Seg");
  display.display();
}

void mostrarAlarmeAtivado()
{
  /*Alarme ativado*/
    display.setCursor(40,0);
    display.drawBitmap(50, 0, com_alarme, 20, 20, 1);
    display.display(); 
}

void mostrarAlarmeDesativado()
{
  /*Alarme desativado*/
    display.setCursor(40,0);
    display.drawBitmap(50, 0, sem_alarme, 20, 20, 1);
    display.display(); 
}

  /*WiFi*/

void mostrarNetOK()
{
    /*Wifi OK*/
      display.setCursor(0,0); 
      display.drawBitmap(10, 0, wifi_ok, 20, 20, 1);   
      display.display(); 
}

void mostrarNetNOK()
{
    /*Wifi NOK*/
      display.setCursor(0,0); 
      display.drawBitmap(10, 0, wifi_nok, 20, 20, 1);  
      display.display(); 
}

void mostrarSemWifi()
{
    /*Sem Wifi*/
      display.setCursor(0,0); 
      display.drawBitmap(10, 0, no_wifi, 20, 20, 1);   
      display.display(); 
}


void setup()
{
    display.begin();
    display.setContrast(50);
    pinMode(5, OUTPUT); // Luz do Display
    digitalWrite(5, HIGH);   
    Serial.begin(9600);
}

void loop()
{
    /*1. MODO AP - priotario*/
    /*2. DisparoAlarme - prioritario*/
    /*3. NomeProjeto + StatusWifi + MostrarAlarme - flexivel*/
    /*ModoAP:DisparoAlarme:Alarme:Wifi*/
    while(Serial.available()){
      String mensagem = Serial.readString();

    int modoAP = mensagem.substring(0,1).toInt();
    
    int disparoAlarme = mensagem.substring(2,3).toInt();

    int alarme = mensagem.substring(4,5).toInt();

    int wifi = mensagem.substring(6).toInt();

    display.clearDisplay();

    if(modoAP){
        mostrarModoAP();
        break;
    }

    else if(disparoAlarme){
        mostrarDisparo();
        break;
    }

    mostrarNomeProjeto();

    if(alarme){
        mostrarAlarmeAtivado();
    }else{
        mostrarAlarmeDesativado();
    }

    if(wifi == 0){
        mostrarSemWifi();
    }else if(wifi == 1){
        mostrarNetNOK();
    }else{
        mostrarNetOK();
    }
    }

    
}
