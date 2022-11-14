/*
    ESPCAM.cpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

volatile bool statusComunicacao;
volatile char statusWiFi;

#include "defineESPCAM.hpp"
#include "ESPCam.hpp"
//#include "EEPROMCam.hpp"
#include "WiFiCam.hpp"

bool sendPhoto = false;

WiFiClientSecure clientTCP;
UniversalTelegramBot bot(BOTtoken, clientTCP);

#define FLASH_LED_PIN 4
bool flashState = LOW;

//Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;


configInitCamera()
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

/*Definicao de comandos para ESP-CAM*/
handleNewMessages(int numNewMessages)
{
    Serial.print("Handle New Messages: ");
    Serial.println(numNewMessages);

    for (int i = 0; i < numNewMessages; i++) 
    {
        String chat_id = String(bot.messages[i].chat_id);
        if (chat_id != CHAT_ID)
        {
            bot.sendMessage(chat_id, "Unauthorized user", "");
            continue;
        }
        
        // Print the received message
        String text = bot.messages[i].text;
        Serial.println(text);
        
        String from_name = bot.messages[i].from_name;
        if (text == "/start") 
        {
            String welcome = "Welcome , " + from_name + "\n";
            welcome += "Use the following commands to interact with the ESP32-CAM \n";
            welcome += "/photo : takes a new photo\n";
            welcome += "/flash : toggles flash LED \n";
            bot.sendMessage(CHAT_ID, welcome, "");
        }

        if (text == "/flash") 
        {
            flashState = !flashState;
            digitalWrite(FLASH_LED_PIN, flashState);
            Serial.println("Change flash LED state");
        }

        if (text == "/photo") 
        {
            sendPhoto = true;
            Serial.println("New photo request");
        }
    }
}

sendPhotoTelegram()
{
    const char* myDomain = "api.telegram.org";
    String getAll = "";
    String getBody = "";

    camera_fb_t * fb = NULL;
    fb = esp_camera_fb_get();
    esp_camera_fb_return(fb);
    fb = esp_camera_fb_get();
   
  
    if(!fb) 
    {
        Serial.println("Camera capture failed");
        delay(1000);
        ESP.restart();
        return "Camera capture failed";
    }  
  
    Serial.println("Connect to " + String(myDomain));

    if (clientTCP.connect(myDomain, 443)) 
    {
        Serial.println("Connection successful");
    
        String head = "--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"chat_id\"; \r\n\r\n" + CHAT_ID + "\r\n--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"photo\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
        String tail = "\r\n--RandomNerdTutorials--\r\n";

        uint16_t imageLen = fb->len;
        uint16_t extraLen = head.length() + tail.length();
        uint16_t totalLen = imageLen + extraLen;
  
        clientTCP.println("POST /bot"+BOTtoken+"/sendPhoto HTTP/1.1");
        clientTCP.println("Host: " + String(myDomain));
        clientTCP.println("Content-Length: " + String(totalLen));
        clientTCP.println("Content-Type: multipart/form-data; boundary=RandomNerdTutorials");
        clientTCP.println();
        clientTCP.print(head);
    
        uint8_t *fbBuf = fb->buf;
        size_t fbLen = fb->len;
        for (size_t n=0;n<fbLen;n=n+1024) 
        {
            if (n+1024<fbLen) 
            {
                clientTCP.write(fbBuf, 1024);
                fbBuf += 1024;
            }
            
            else if (fbLen%1024>0) 
            {
                size_t remainder = fbLen%1024;
                clientTCP.write(fbBuf, remainder);
            }
        }  
    
        clientTCP.print(tail);
        esp_camera_fb_return(fb);
    
        int waitTime = 10000;   // timeout 10 seconds
        long startTimer = millis();
        boolean state = false;
        
        while ((startTimer + waitTime) > millis())
        {
            Serial.print(".");
            delay(100);      
            while (clientTCP.available()) 
            {
                char c = clientTCP.read();
                if (state==true) getBody += String(c);        
                if (c == '\n') 
                {
                    if (getAll.length()==0) state=true; 
                    getAll = "";
                }

                else if (c != '\r')
                getAll += String(c);
                startTimer = millis();
            }
            if (getBody.length()>0) break;
        }

        clientTCP.stop();
        Serial.println(getBody);
    }

    else 
    {
        getBody="Connected to api.telegram.org failed.";
        Serial.println("Connected to api.telegram.org failed.");
    }

    return getBody;
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
    //verificaDadosWiFi();
    operacoes();
    //verificaConexao();
    //verificaModoAP();
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

    if (millis() > lastTimeBotRan + botRequestDelay)  
    {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        while (numNewMessages) 
        {
            Serial.println("got response");
            handleNewMessages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }
        lastTimeBotRan = millis();
    }
}

void serialConfig()
{
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, RX_CENTRAL, TX_CENTRAL);
    Serial.println("Serial iniciada");
}

void inicializarVariaveis(){
    statusComunicacao = false;
}

void verificaConexao(){
    //testeConexao(&statusWiFi);
}

void verificaModoAP(){
    //while(enviaFormulario());
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
    
    else if("RESTART")
    {
        ESP.restart();
    }
}
