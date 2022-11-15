#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "defineESPCentral.hpp"
#include "ESPCentral.hpp"
#include "Display.hpp"

/*Definicao de Pinos*/
Adafruit_PCD8544 display = Adafruit_PCD8544(18,23,4,15,2);

int checa_wifi = 0;
int checa_modoap = 0;

void atualizaStatus(const bool modoAP, const char statusWiFi, const bool alarme)
{}
/*
{
  /*Modo AP
  if(modoAP)
  {
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2); 
    display.println("MODO AP");  
    display.setTextSize(0.5);
    display.println("ssid: Central_De_Seguranca");
    display.println("senha: CentralSEG!2022");
    display.println("link: 192.168.4.1");
    display.display();   
    return;
  }

  display.println("Central de Seguranca Residencial");

  switch(statusWiFi)
  {
    /*Wifi OK
    case NET_OK:
      if(checa_wifi !=0 && checa_wifi !=1)
      {
        display.clearDisplay(); 
        display.setCursor(0,0); 
        display.setTextSize(1); 
        display.println("Wi-fi [OK]");   
        display.display(); 
      }

      else if(!checa_wifi)
      {
        display.setCursor(0,0); 
        display.setTextSize(1); 
        display.println("Wi-fi [OK]");   
        display.display(); 
        checa_wifi = 1;
      }

      else if(checa_wifi == 1) break;
        
    break;

    /*Wifi NOK
    case SEM_NET:
      if(checa_wifi !=0 && checa_wifi !=2)
      {
        display.clearDisplay(); 
        display.setCursor(0,0); 
        display.setTextSize(1); 
        display.println("Wi-fi [OK]");   
        display.display(); 
      }

      else if(!checa_wifi)
      {
        display.setCursor(0,0); 
        display.setTextSize(1); 
        display.println("Wi-fi [OK]");   
        display.display(); 
        checa_wifi = 2;
      }

      else if(checa_wifi == 2) break;
        
    break;

    /*Sem Wifi
    case SEM_WIFI:
      if(checa_wifi !=0 && checa_wifi !=3)
      {
        display.clearDisplay(); 
        display.setCursor(0,0); 
        display.setTextSize(1); 
        display.println("Wi-fi [OK]");   
        display.display(); 
      }

      else if(!checa_wifi)
      {
        display.setCursor(0,0); 
        display.setTextSize(1); 
        display.println("Wi-fi [OK]");   
        display.display(); 
        checa_wifi = 3;
      }

      else if(checa_wifi == 3) break;
        
    break;
  }

  /*Alarme ativo
  if(alarme)
  {

  }

  else if(!alarme)
  {

  }

}

/*
void setup()   
{
  Serial.begin(115200);
  display.begin();
  display.setContrast(50);
}*/



  /*

  /*Comandos Uteis

    display.clearDisplay(); //Limpa a tela
    display.setCursor(0,0); //Seta onde comeca o texto
    display.setTextSize(1); //Tamanho do texto
    display.println("Wi-fi [OK]\nAlarm [OK]");   
    display.display(); //Transferir informacao
    display.clearDisplay();
  }
  */


  /*display.clearDisplay();
  display.drawBitmap(0, 0, arduino_icon, 84, 48, 1);
  display.display();
  delay(2000);
  display.clearDisplay();

  display.drawBitmap(0, 0, wifi_icon, 20, 20, 1);
  display.drawBitmap(30, 0, alarm_icon, 20, 20, 1);
  display.setTextColor(BLACK);
  display.setCursor(0,25);
  display.setTextSize(0.5);
  display.println("Wi-fi [OK]\nAlarm [OK]");  
  display.display();
  delay(2000);
  display.clearDisplay();
 
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.setTextSize(2);
  display.println("Teste");
  display.display();

  delay(1000);
  display.clearDisplay();
  
  display.drawBitmap(0, 0, book_icon, 20, 20, 1);
  display.setTextColor(BLACK);
  display.setCursor(0,25);
  display.setTextSize(0.5);
  display.println("Oficinas\nde\nIntegracao");
  display.display();

  delay(1000);
  display.clearDisplay();

  display.drawBitmap(0, 0, calendar_icon, 20, 20, 1);
  display.setTextColor(BLACK);
  display.setCursor(0,25);
  display.setTextSize(1);
  display.println("19\nSetembro\n2022");
  display.display();

  delay(1000);*/
