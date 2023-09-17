/*
Projeto criado por:Felipe Vieira
*/

//Biblioteca
#include <Adafruit_NeoPixel.h>

#define DELAYVAL 60 // Delay do sistema
#define PINO_CONFG 2 //Pino de configuração dos leds
#define NUMEROPIXELS 24 //Quantidade de leds
#define PINO_BOTAO 25  //Pino do botão


Adafruit_NeoPixel pixels(NUMEROPIXELS, PINO_CONFG, NEO_GRB + NEO_KHZ800);

uint8_t ledcounter = 0;
uint8_t tabelaCores[]={0,50,100};

typedef struct Cor{
  uint8_t r;
  uint8_t g;
  uint8_t b;
}Cor;

Cor corFitaLeds;

void setup() {
  
  pinMode(PINO_BOTAO,INPUT_PULLUP); //definir o "PINO_BOTAO" como entrada
  pixels.begin(); // Inicializar o sistema de pixeis
}

void loop() {
  pixels.clear(); //limpar a lista de pixeis


  if(digitalRead(PINO_BOTAO)==LOW){
    corFitaLeds = (Cor){
      random(0,2),
      random(0,2),
      random(0,2)
    };
    delay(250);
  }
  
  for (int i = -1; i < NUMEROPIXELS; i++) { //Para cara pixel

    pixels.setPixelColor(i, pixels.Color(corFitaLeds.r,corFitaLeds.g,corFitaLeds.b));
    
    if (i == ledcounter) {//se for primeiro pixel
      Cor corLedPrincipal = (Cor){
        random(0,2),
        random(0,2),
        random(0,2)
      };
      pixels.setPixelColor(i, pixels.Color(corLedPrincipal.r,corLedPrincipal.g,corLedPrincipal.b));
    }

    
  }
  delay(DELAYVAL); //fazer um delay pro proximo loop
  
  ledcounter < NUMEROPIXELS - 1 ? ledcounter++ : ledcounter = 0;
  
  pixels.show();   // atualizar a lista de pixeis
}