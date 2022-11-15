/*
ESPCAM.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
Projeto - Central de Segurança Residencial
Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

volatile bool statusComunicacao;
volatile char statusWiFi;

#include "defineESPCAM.hpp"
#include "ESPCam.hpp"
#include "WiFiCam.hpp"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"

bool sendPhoto = false;

#define FLASH_LED_PIN 4
bool flashState = LOW;

//Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;


void configInitCamera()
{
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    config.frame_size = FRAMESIZE_CIF;
    config.jpeg_quality = 20;
    config.fb_count = 1;

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
    }

    // Drop down frame size for higher initial frame rate
    sensor_t * s = esp_camera_sensor_get();
    s->set_framesize(s, FRAMESIZE_CIF);  // UXGA|SXGA|XGA|SVGA|VGA|CIF|QVGA|HQVGA|QQVGA
}


void configuracao()
{   
    serialConfig();
    informacoesWiFi();

    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
    pinMode(FLASH_LED_PIN, OUTPUT);
    digitalWrite(FLASH_LED_PIN, flashState);
    pinMode(TRANCA, OUTPUT);

    configInitCamera(); 
    conectaWiFi();
}

void executar()
{
    operacoes();
    verificaConexao();
    testeESPComunicacao();
}

void operacoes()
{
    if (sendPhoto) 
    {
        Serial.println("Preparing photo");
        sendPhotoTelegram(); 
        sendPhoto = false; 
    }   
}

void serialConfig()
{
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, RX_CENTRAL, TX_CENTRAL);
    Serial.println("Serial iniciada");
}

void inicializarVariaveis()
{
    statusComunicacao = false;
    statusWiFi = SEM_WIFI;
}

void verificaConexao()
{
    testeConexao(&statusWiFi);
}

void testeESPComunicacao()
{  
    while(!Serial1.available());

    String mensagem = Serial1.readString();
    Serial.println(mensagem);

    if(mensagem == "TesteComunicacao")
    {
        Serial1.print(mensagem);
    }

    else if(mensagem == "abrir")
    {
        digitalWrite (TRANCA, HIGH);
        delay(5000);
        digitalWrite (TRANCA, LOW);
        Serial1.print("resetRFID");
    }
    
    else if(mensagem == "RESTART")
    {
        ESP.restart();
    }

    else if(mensagem == "foto"){
        sendPhoto = true;
    }
}
