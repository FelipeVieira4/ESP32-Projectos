#include <LiquidCrystal_I2C.h> // Biblioteca utilizada para fazer a comunicação com o display 20x4 
#include <ArduinoJson.h>
#include <EspMQTTClient.h>
#include <PubSubClient.h>

#define pinoBuzzer 10

#define col 16 // Serve para definir o numero de colunas do display utilizado
#define lin  2 // Serve para definir o numero de linhas do display utilizado
#define ende  0x27 // Serve para definir o endereço do display.

#define botaoJoystick 4
#define JoyStickVy 34

LiquidCrystal_I2C lcd_1(ende,col,lin); // Chamada da funcação LiquidCrystal para ser usada com o I2C



uint8_t dificuldadeJogo=0,modoJogo=0;
uint8_t chances=0,chute=0,correto=0;

int placarJogo=0; 

#define publishTopic "esp32-publish"

EspMQTTClient mqttSystem(
  "Wokwi-GUEST",
  "",
  "mqtt.tago.io",
  "Default",
  "5134b7a5-9055-424e-bac5-298047e7a1f2",
  "Esp32"
);

void mensagemRecebida(const String &topic){}

void onConnectionEstablished(){
  mqttSystem.subscribe(publishTopic, mensagemRecebida);
}

void PrintTelaJogo(){
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("CHUTE:");
    lcd_1.print(chute);
    lcd_1.setCursor(6, 1);
    lcd_1.print("CHANCES:");
    lcd_1.print(chances);
}

void salvarPartida(){

  uint8_t posicaoLetra=0;
  char nomeJogador[]="AAA";
  char letra=65;

  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print("NOME:");
  
  while(posicaoLetra<3){

    lcd_1.setCursor(5, 0);
    lcd_1.print(nomeJogador);

    letra+=+(analogRead(JoyStickVy) <= 1000)-(analogRead(JoyStickVy) >= 3000);
    
    nomeJogador[posicaoLetra]=letra;
    if(digitalRead(botaoJoystick)==LOW){
      posicaoLetra++;
    }

    delay(200);
  }

  char mensagemJson[100];
  DynamicJsonDocument pacoteDados(1024);

  pacoteDados[0]["variable"]="placar";
  pacoteDados[0]["value"]=placarJogo;
  pacoteDados[1]["variable"]="nome";
  pacoteDados[1]["value"]=String(nomeJogador);
  
  //pacoteDados[0]["nome"]=String(nomeJogador);
  serializeJson(pacoteDados, mensagemJson);
  mqttSystem.publish(publishTopic, mensagemJson);
  mqttSystem.loop();
}

void TelaInicial(){

  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print("ESCOLHA NIVEL:");
  while(true){
    mqttSystem.loop();
    delay(500);
    lcd_1.setCursor(5, 1);

    //Printar a dificuldade
    switch(dificuldadeJogo){
      case 0:
          lcd_1.print("FACIL  ");
      break;
      case 1:
          lcd_1.print("MEDIO  ");
      break;
      case 2:
          lcd_1.print("DIFICIL");
      break;
    }

    if(analogRead(JoyStickVy) <= 1000 && dificuldadeJogo < 3){
      dificuldadeJogo++;   //Aumentar dificuldade
    }else if(analogRead(JoyStickVy) >= 3000 && dificuldadeJogo > 0){
      dificuldadeJogo--;  //Diminuir dificuldade
    }

    if(digitalRead(botaoJoystick) == LOW){
      
      if(dificuldadeJogo < 2)dificuldadeJogo++;
      
      correto = random(((dificuldadeJogo)*10)+1);
      chances = 10/(dificuldadeJogo+1);
      modoJogo=1;//Ir pro jogo
      break;
    }
    delay(250);
  }
}

void TelaJogo(){
  while(true){
    mqttSystem.loop();
    delay(500);
    uint8_t oldChute = chute;
     
    if(analogRead(JoyStickVy) <= 1000 && chute+1 < (dificuldadeJogo*10)+1){
      chute++;
      //MudarNumero();
    }else if(analogRead(JoyStickVy) >= 3000){
      chute--;
      //MudarNumero();
    }

    if(oldChute!=chute){
      lcd_1.setCursor(6, 0);
      lcd_1.print("   ");
      lcd_1.setCursor(6, 0);
      lcd_1.print(chute);
    }
    
    if(digitalRead(botaoJoystick)==LOW){
      //PressButton();
      
      if(chute==correto){
        lcd_1.clear();
        lcd_1.setCursor(4, 0);
        lcd_1.print("PARABENS");

        delay(3000);
        modoJogo=0;
        placarJogo++;
        break;  //Voltar ao menu
      }        
      else if(chances--<1){
        lcd_1.clear();
        lcd_1.setCursor(2, 0);
        lcd_1.print("GAME OVER");
        lcd_1.setCursor(3, 1);
        lcd_1.print("RESULTADO:"); 
        lcd_1.print(correto);
        
        delay(2000);

        salvarPartida();
        placarJogo=0;
        dificuldadeJogo=0;
        modoJogo=0;
        break;  //Voltar ao menu
      }else if(chute > correto){

        lcd_1.clear();
        lcd_1.setCursor(5, 0);
        lcd_1.print("MENOR");   //Printar maior na tela    
      }else{
        lcd_1.clear();
        lcd_1.setCursor(5, 0);
        lcd_1.print("MAIOR");    //Printar menor na tela     
      }
      delay(2000); 
      PrintTelaJogo();
    }
    
  }
}

void setup()
{
  Serial.begin(9600);
 
  lcd_1.init();
  lcd_1.clear();
  lcd_1.backlight();

  pinMode(botaoJoystick,INPUT_PULLUP);
  
  pinMode(pinoBuzzer,OUTPUT);
  
  dificuldadeJogo=0;
  modoJogo=0;
  chute=0;
  correto=0;
  chances=0;
}


void loop()
{
  mqttSystem.loop();
  if(modoJogo==0){
    TelaInicial();
  }else{
    PrintTelaJogo();
    TelaJogo();
  }
}