# Conexões Display Nokia a5110 com ESP32

## *Entendendo as conexões*

### •Display Nokia a5110

<img src="https://github.com/IshikawaRasoto/Central-de-Seguranca-Residencial/blob/main/Teste/DisplayNokia/assets/PinosDisplayNokia5110.png" width = 300>

#### →Pino 1: Reset <br />
#### →Pino 2: Chip Enable(CE) <br />
#### →Pino 3: Data/Command(DC) <br />
#### →Pino 4: Serial IN(DIN) <br />
#### →Pino 5: Clock(CLK) <br />
#### →Pino 6: Power(VCC) <br />
#### →Pino 7: Back Light(BL) <br />
#### →Pino 8: Ground(GND) <br />

### •ESP32 DevKit V1

<img src="https://github.com/IshikawaRasoto/Central-de-Seguranca-Residencial/blob/main/Teste/DisplayNokia/assets/PinosESP32.png" width = 600>

### •Conectando o Display Nokia com a ESP32 

<img src="https://github.com/IshikawaRasoto/Central-de-Seguranca-Residencial/blob/main/Teste/DisplayNokia/assets/ConexaoDisplay-ESP32.1.jpg" width = 600>

<img src="https://github.com/IshikawaRasoto/Central-de-Seguranca-Residencial/blob/main/Teste/DisplayNokia/assets/ConexaoDisplay-ESP32.2.jpg" width = 600>

## *Entendendo a biblioteca gráfica*

### A biblioteca Adafruit_GFX.h possibilita com que o display mostre imagens na tela. Isso é possível convertendo uma imagem convencional para o tipo *.bmp*, dessa forma, há a leitura da imagem para o display. Abaixo o passo a passo:

##### Observação: você pode importar imagens de qualquer estilo, mas, lembre-se, como a dimensão do display é pequeno, a imagem se redimensionará para aquela proporção, muitas vezes, perdendo seus detalhes. Além disso, o display funciona no formato monocromático, impossibilitando uma paleta de cores além de preto e branco.

#### 1. Ache a imagem desejada e a baixe
#### 2. Após ter a imagem, você precisa transformar o formato da imagem para .bmp. Você pode fazer isso de diversas formas, como, por exemplo, no software Paint.
#### 3. Agora, tendo a imagem convertida, acesse https://javl.github.io/image2cpp/ e siga os passos a baixo:
##### 3.1 Importe a imagem no site[1].
##### 3.2 Mude a proporção da imagem para o tamanho desejado. *Lembre-se que o Display Nokia a5110 tem a proporção de 84x48!*
##### 3.3 Defina o estilo da imagem[2]. 
###### Recomendação: Selecione white; Inverta a cor da imagem; Ajuste a luminosidade para 225; *Mude a escala para: "stretch to fill canvas"*.
##### 3.4 Clique em gerar código e copie apenas a informação do vetor. Isto é, a imagem em bitmap.

#### 4. Após tem a informação que precisamos, criamos em nosso código o vetor que armazenará nossa imagem. Sua assinatura será: *static const uint8_t arduino_icon[1024] =*.
#### 5. Atribua ao vetor o código que obtivemos do site. Isto é, a imagem em bitmap.
#### 6. Após os passos, sua imagem já está pronta para ser usada no código. Você vai a implementar por meio da função drawBitmap()

## Entendendo o drawBitmap()
### Exemplo de código: display.drawBitmap(0, 0, arduino_icon, 20, 20, 1);
#### Os dois primeiros parâmetros (0,0) se referem, respectivamente, ao X e Y iniciais da imagem. Ou seja, onde a imagem se iniciará.
#### Após isso, o terceiro parâmetro. Ele se refere ao vetor que possui nossa imagem em bitmap. 
#### Os dois próximos parâmetros são a proporção de nossa imagem. Lembra que no site onde pegamos a informação de nosso vetor, informamos a proporção da imagem? Pois bem, você informará essa proporção neste parâmetro, respectivamente, o X e o Y. No caso do exemplo, nossa imagem tem proporção 20x20.
#### O último se refere a estilização da imagem, ou seja, preto(0) ou branco(1).

