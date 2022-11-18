#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "defineESPCentral.hpp"
#include "Display.hpp"

/*Definicao de Pinos*/
Adafruit_PCD8544 display = Adafruit_PCD8544(18,23,4,15,2);

void serialDisplayConfig()
{
    display.begin();
    display.setContrast(50);
}

void atualizaStatus(const bool modoAP, const char statusWiFi, const bool alarme)
{
  display.clearDisplay();

  /*Modo AP*/
  if(modoAP) //Caso TRUE
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

  /*Mostar nome do projeto*/
  display.setCursor(30,30);
  display.setTextSize(0.5);
  display.println ("Central\nDe\nSeguranca");
  display.display();

  /*WiFi*/
  switch(statusWiFi)
  {
    /*Wifi OK*/
    case NET_OK:
      display.setCursor(0,0); 
      display.setTextSize(1); 
      display.println("NET OK");   
      display.display(); 

    break;

    /*Wifi NOK*/
    case SEM_NET:
      display.setCursor(0,0); 
      display.setTextSize(1); 
      display.println("NET NOK");   
      display.display(); 
        
    break;

    /*Sem Wifi*/
    case SEM_WIFI:
      display.setCursor(0,0); 
      display.setTextSize(0.5); 
      display.println("Wi-Fi NOK");   
      display.display(); 

    break;
  }

  /*Alarme ativado*/
  if(alarme)
  {
    display.setCursor(15,0);
    display.setTextSize(0.5);
    display.println("Alarme ativado");
    display.display(); 
  }

  /*Alarme desativado*/
  else if(!alarme)
  {
    display.setCursor(15,0);
    display.setTextSize(0.5);
    display.println("Alarme desativado");
    display.display(); 
  }
}





  /*Explicacao dos Comandos

  display.clearDisplay(); //Limpa a tela
  display.setCursor(0,0); //Seta onde comeca o texto
  display.setTextSize(1); //Tamanho do texto
  display.println("Wi-fi [OK]\nAlarm [OK]"); //Exemplo de print
  display.display(); //Transfere a informacao para o display
  display.drawBitmap(0, 0, arduino_icon, 84, 48, 1); //Printa o bitmap na tela -> xi, yi, bitmap, xf, yf, preto0/branco1
  display.setTextColor(BLACK); //Define a cor do texto

  //Mais exemplos:

  display.drawBitmap(0, 0, arduino_icon, 84, 48, 1);
  display.drawBitmap(0, 0, wifi_icon, 20, 20, 1);
  display.drawBitmap(30, 0, alarm_icon, 20, 20, 1);
  display.setCursor(0,25);
  display.setTextSize(0.5);
  display.drawBitmap(0, 0, book_icon, 20, 20, 1);
  display.setTextColor(BLACK);
  display.setCursor(0,25);
  display.setTextSize(0.5);
  display.println("Oficinas\nde\nIntegracao");
  display.display();
  display.drawBitmap(0, 0, calendar_icon, 20, 20, 1);
  display.setTextColor(BLACK);
  display.setCursor(0,25);
  display.println("19\nSetembro\n2022");
  display.display();

*/
