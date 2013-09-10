//Copyright: Taiyo Kato
const int NONE = B111; //7
const int RED = B011; //3
const int GREEN = B101; //5
const int BLUE = B110; //6
const int YELLOW = B001; //1
const int CYAN = B100; //4
const int MAGENTA = B010; //2
const int WHITE = B000; //0
const int COLORS[] = {
  RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE};
const String COLORNAMES[] = {   
  "RED", "GREEN", "BLUE", "YELLOW", "CYAN", "MAGENTA", "WHITE"};

const int ON = HIGH;
const int OFF = LOW;

//result led
const int green_led = 12;
const int red_led = 8;
//result led end

const int red_four = 9;
const int red_four_i = 2;//
int red_four_state = 0;
const int green_four = 11;//10
const int green_four_i = 7;//
int green_four_state = 0;
const int blue_four = 10;//11
const int blue_four_i = 4;
int blue_four_state = 0;

//next button
int next_i_state = 0;
const int next_i = 5;

int game_round = 1;
boolean playing = false; //pause flag
int next_i_count = 0;
int disparr[30]; //meh. just set limit to 30
int pressedarr[30]; //same here
boolean waitinput = false;

boolean debugmode = 0;
boolean debugprint = 1;
int cur_press = NONE; //for next input safety

void setup(){
  //output pins
  pinMode(green_led,OUTPUT);
  pinMode(red_led,OUTPUT);
  pinMode(red_four,OUTPUT);
  pinMode(green_four,OUTPUT);
  pinMode(blue_four,OUTPUT);

  //input pins
  pinMode(red_four_i,INPUT);
  pinMode(green_four_i,INPUT);
  pinMode(blue_four_i,INPUT);
  pinMode(next_i, INPUT);

  next_i_count = 0;
  OutputColor(NONE);

  //loopdisp();
  Serial.begin(9600);
  randomSeed(analogRead(0)); //initialize randomizer

  digitalWrite(red_led,LOW);
  digitalWrite(green_led, LOW);

  while (debugmode){
    OutputColor(GetPressed());
  } 
}

byte GetPressed(){
  int p = B000; //all off
  p ^= ((digitalRead(red_four_i) ? B100 : B000));
  p ^= ((digitalRead(green_four_i) ? B010 : B000));
  p ^= ((digitalRead(blue_four_i) ? B001 : B000));
  return p;
}

int prev_press = NONE;

void loop(){
  if (game_round >= 30){
    return; //force stop to avoid memory OOB 
  }
  if (playing){
    if (next_i_count > (game_round+1)){
      for (int a = 0; a < (game_round+1); a++){
        if (disparr[a] != pressedarr[a]){
          digitalWrite(red_led, LOW);
          delay(1000); //player surprise
          digitalWrite(red_led, HIGH);
          Serial.println("Game over! \nYour high score is:");
          Serial.println(game_round);
          game_round = 1; //reset game round
          next_i_count = 0; //reset next count
          playing = false;
          delay(500);
          digitalWrite(red_led,LOW);
          digitalWrite(green_led, LOW);

          return;
        }
      }
      digitalWrite(green_led, LOW);
      game_round ++;
      next_i_count = 0;
      delay(2000);
      GameStart();
    }

    boolean nextinput = 1;
    int pressed = NONE;
    while (nextinput==1){
      pressed = GetPressed();
      OutputColor(pressed);	                
      nextinput = digitalRead(next_i);
      if (pressed == NONE){		
        nextinput = 1;
      }	
    }
    if (debugprint){
    Serial.println("Current next_i_count: ");
    Serial.println(next_i_count);
    }
    while(digitalRead(next_i)==0){
      //do nothing until next_i is released.   
    }
    pressedarr[next_i_count] = pressed;
    next_i_count++;
    pressed=NONE;
    OutputColor(NONE);
    delay(200);

  }
  else{
    int down = 0;
    while (digitalRead(next_i)==0)
    {
      down++;
      Serial.println(down);
      
      if (down >= 500){
        playing = true;
        GameStart();
        return;
      }
      if (digitalRead(next_i)==1){
        down = 0;
        return;
      }
    }

  }
}

void GameStart(){
  if (game_round>=27) return; //stop because disparr will be OOB if round is over 27
  Serial.println("Starting round: ");
  Serial.println(game_round);
  digitalWrite(red_led, HIGH);
  digitalWrite(green_led, HIGH);
  for (int i =0 ; i<(game_round+2); i++){
    int combi = random(8);//random(8);
    OutputColor(COLORS[combi]);
    disparr[i] = COLORS[combi];
    if (debugprint) Serial.println(COLORNAMES[combi]);
    int d = 375;//(500 * (game_round * 3));
    delay(d);
    OutputColor(NONE);
    delay(d);

  } 
  digitalWrite(green_led,HIGH);

}

void OutputColor(int combo){
  digitalWrite(red_four,(combo&B100));
  digitalWrite(green_four,(combo&B010));
  digitalWrite(blue_four,(combo&B001));  
}


