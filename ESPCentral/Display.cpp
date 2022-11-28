#include "Display.hpp"

/*Definicao de Pinos*/
Adafruit_PCD8544 display = Adafruit_PCD8544(18,23,4,15,2);

void serialDisplayConfig()
{
  display.begin();
  display.setContrast(50);
}

void atualizaStatus(const char statusWiFi, const bool alarme, const bool alarme_disparado)
{
  if(alarme_disparado) 
  {
    mostrarDisparo();
  }

  else
  {
    display.clearDisplay();
    mostrarNomeProjeto();
    mostrarWifi(statusWiFi);
    mostrarAlarme(alarme);
  }
}

void mostrarModoAP()
{
  /*Refazer proporcionalidade na tela e rever informações*/
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2); 
  display.println("MODO AP");
  display.setTextSize(0.5);
  display.println("ssid: Central_De_Seguranca");
  display.println("senha: CentralSEG!2022");
  display.println("link: 192.168.4.1");
  display.display();   
}

void mostrarDisparo()
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2); 
  display.println("ALARME\nDISPARADO");
  display.display();
}

void mostrarNomeProjeto()
{
  display.setCursor(0,35);
  display.setTextSize(0.2);
  display.println ("Central de Seg");
  display.display();
}

void mostrarAlarme(const bool alarme)
{
  /*Alarme ativado*/
  if(alarme) /*Caso for diferente de 0*/
  {
    display.setCursor(40,0);
    display.drawBitmap(50, 0, com_alarme, 20, 20, 1);
    display.display(); 
  }

  /*Alarme desativado*/
  else /*Caso for igual a 0*/
  {
    display.setCursor(40,0);
    display.drawBitmap(50, 0, sem_alarme, 20, 20, 1);
    display.display(); 
  }
}

void mostrarWifi(const char statusWiFi)
{
  /*WiFi*/
  switch(statusWiFi)
  {
    /*Wifi OK*/
    case NET_OK:
      display.setCursor(0,0); 
      display.drawBitmap(10, 0, wifi_ok, 20, 20, 1);   
      display.display(); 

    break;

    /*Wifi NOK*/
    case SEM_NET:
      display.setCursor(0,0); 
      display.drawBitmap(10, 0, wifi_nok, 20, 20, 1);  
      display.display(); 
        
    break;

    /*Sem Wifi*/
    case SEM_WIFI:
      display.setCursor(0,0); 
      display.drawBitmap(10, 0, no_wifi, 20, 20, 1);   
      display.display(); 

    break;
  }
}

  /*Explicacao dos Comandos

  display.clearDisplay(); //Limpa a tela
  display.setCursor(0,0); //Seta onde comeca o texto
  display.setTextSize(1); //Tamanho do texto
  display.println("Wi-fi [OK]\nAlarm [OK]"); //Exemplo de print
  display.display(); //Transfere a informacao para o display
  display.drawBitmap(0, 0, arduino_icon, 84, 48, 1); //Printa o bitmap na tela -> xi, yi, bitmap, tamanho da imagem(x,y), preto0/branco1
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
