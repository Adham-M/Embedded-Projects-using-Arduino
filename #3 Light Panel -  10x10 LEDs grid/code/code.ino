/**
 * In Emoji mode
 * 0 = 🙂
 * 1 = 🙁
 * 2 = 🙃
 * 3 = 😀
 * 4 = 😐
 * 5 = 😮
 * 6 = 😒
 * 7 = 😦
 * 8 = 😣
 * 9 = 😶
 */

#define EmojiModeIndecator 0
#define AudioOut 3
#define Reset_PIN 5
#define OutputNum_0 6
#define OutputNum_1 7
#define OutputNum_2 8
#define OutputNum_3 9
#define OutputNum_4 10
#define OutputNum_5 11
#define OutputNum_6 12
#define OutputEnable 13
#define KeypadInput A0

// TONES  ===========================================================
#define Passfreq 4000    // 240 Hz
#define Key0freq 3830    // 261 Hz
#define Key1freq 3400    // 294 Hz
#define Key2freq 3038    // 329 Hz
#define Key3freq 2864    // 349 Hz
#define Key4freq 2550    // 392 Hz
#define Key5freq 2272    // 440 Hz
#define Key6freq 2028    // 493 Hz
#define Key7freq 1912    // 523 Hz
#define Key8freq 1890    // 529 Hz
#define Key9freq 1850    // 540 Hz

int melody[] = {  Key0freq, Key1freq, Key2freq, Key3freq, Key4freq,
                  Key5freq, Key6freq, Key7freq, Key8freq, Key9freq};
//===================================================================

// Shapes data  ===========================================================================================
int Digits[10][24] = {/*0*/{5,6,17,28,38,48,58,68,78,87,96,95,84,73,63,53,43,33,23,14},
                      /*1*/{23,14,5,15,25,35,45,55,65,75,85,93,94,95,96,97},
                      /*2*/{23,14,5,6,17,28,37,47,56,66,65,75,84,93,94,95,96,97,98},
                      /*3*/{13,4,5,6,17,27,37,46,45,57,67,77,87,96,95,94,83},
                      /*4*/{97,87,77,67,57,47,37,27,17,7,16,25,34,43,53,63,64,65,66,68},
                      /*5*/{7,6,5,4,3,13,23,33,43,44,45,56,67,77,86,95,94,93},
                      /*6*/{18,7,6,5,4,13,23,33,43,53,63,73,83,94,95,96,97,88,78,68,57,56,55,54},
                      /*7*/{3,13,4,14,5,15,6,16,7,17,8,18,27,36,46,55,65,74,84,94},
                      /*8*/{5,6,17,28,37,46,45,54,63,73,84,95,96,87,78,68,57,34,23,14},
                      /*9*/{83,94,95,96,97,88,78,68,58,48,38,28,18,7,6,5,4,13,23,33,44,45,46,47}};

int DigitsSize [10] = {20,16,19,17,20,18,24,20,20,24};

int faceCircle [28] = {31,41,51,61,72,82,83,94,95,96,97,88,89,79,70,60,50,40,29,19,18,7,6,5,4,13,12,22};
#define faceCircleSize 28

int Emojis[10][10] = {/*🙂*/{34,37,54,65,66,57},
                      /*🙁*/{34,37,64,55,56,67},
                      /*🙃*/{64,67,44,35,36,47},
                      /*😀*/{34,37,54,64,75,76,67,57,56,55},
                      /*😐*/{34,44,37,47,64,65,66,67},
                      /*😮*/{24,27,45,46,57,67,76,75,64,54},
                      /*😒*/{34,35,45,37,38,48,74,65,66,77},
                      /*😦*/{34,37,55,56,67,77,76,75,74,64},
                      /*😣*/{33,44,53,38,47,58,74,65,66,77},
                      /*😶*/{34,44,37,47}};
                      
int EmojisSize [10] = {6,6,6,10,8,10,10,10,10,4};
//=========================================================================================================


//for key control
int keyPressed;
bool stillPressing = false;
int keyArray[4] = {-1,-1,-1,-1};

//for Password
int numberOfKeysPressed = 0;
int keyPass[4] = {1,2,3,4};
bool changePass = false;
int changePassCount = 0;

//for drawing
int drawingNow = -1;
bool finishedDrawing = true;
bool drawingInterrupted = false;

//Opterating mode indecator
bool EmojiMode = false;

//for Audio out
bool ActivateKeySound = false;

void setup() {
  pinMode(KeypadInput,INPUT);
  pinMode(EmojiModeIndecator,OUTPUT);
  pinMode(AudioOut,OUTPUT);

  for(int i=5;i<14;i++)
    pinMode(i,OUTPUT);

  selfTesting(); 
}

void loop() {
  getKeyPressed();
  
  if(!finishedDrawing){
    drawingInterrupted = false;
    if(!EmojiMode)
      drawNumber();
    else
      drawEmoji();
  }
}


void drawNumber(){
  for (int i = 0; i<DigitsSize[drawingNow]; i++){
    if(drawingInterrupted)
      break;
    illuminateALed(Digits[drawingNow][i]);
    myDelay(100);
    if(i == (DigitsSize[drawingNow] -1))
      finishedDrawing = true;
    getKeyPressed();
  }
}

void drawEmoji(){
  for (int i = 0; i<faceCircleSize; i++){
    if(drawingInterrupted)
      break;
    illuminateALed(faceCircle[i]);
    myDelay(100);
    getKeyPressed();
  }
  for (int i = 0; i<EmojisSize[drawingNow]; i++){
    if(drawingInterrupted)
      break;
    illuminateALed(Emojis[drawingNow][i]);
    myDelay(100);
    if(i == EmojisSize[drawingNow] -1)
      finishedDrawing = true;
    getKeyPressed();
  }
}


void illuminateALed(int num){
  digitalWrite(OutputEnable,LOW);
  for(int j = 6; j<13; j++){
    digitalWrite(j, num%2 == 1);
    num/=2;
  }
  digitalWrite(OutputEnable,HIGH);
}

void clearAllLeds(){
  for(int j = 6; j<13; j++)
    digitalWrite(j, LOW);
  digitalWrite(Reset_PIN, HIGH);
  digitalWrite(OutputEnable,LOW);
  digitalWrite(Reset_PIN, LOW);
}


void selfTesting(){
  clearAllLeds();
  
  myDelay(200);

  //Testing columns
  for (int i = 1; i<11; i++){
    for(int k = 0; k<10; k++){
      illuminateALed(i + 10*k); 
      if(k == 9){
        myDelay(200);
        clearAllLeds();
      }
    }
  }

  //Testing rows
  for (int i = 1; i<101; i++){
    illuminateALed(i);
    if(i%10 == 0){
      myDelay(200);
      clearAllLeds();
    }
  }
}


void myDelay(int Millis2Wait){
   unsigned long currentMillis = millis();
   while((millis() - currentMillis) < Millis2Wait){getKeyPressed();}
}


void getKeyPressed(){
  int input_kp = analogRead(KeypadInput);

  if(stillPressing && input_kp)
    return;
  else
    stillPressing = false;
  
  if (input_kp){
    if (input_kp == 1022) keyPressed = 9;
    else if (input_kp == 1000) keyPressed = 8;
    else if (input_kp == 974) keyPressed = 7;
    else if (input_kp == 954) keyPressed = 6;
    else if (input_kp == 929) keyPressed = 5;
    else if (input_kp == 911) keyPressed = 4;
    else if (input_kp == 889) keyPressed = 3;
    else if (input_kp == 872) keyPressed = 2;
    else if (input_kp == 852) keyPressed = 1;
    else if (input_kp == 837) keyPressed = 0;

    stillPressing = true;
    numberOfKeysPressed++;

    //Adding keyPressed to the keyArray
    for(int i=0;i<3;i++)
      keyArray[i] = keyArray[i+1];
    keyArray[3] = keyPressed;

    if(drawingNow != keyPressed){
      finishedDrawing = false;
      drawingInterrupted = true;
      drawingNow = keyPressed;
      clearAllLeds();
    }
    
    //Changing password
    if(changePass && changePassCount <= 3){
      keyPass[changePassCount] = keyPressed;
      changePassCount++;
      return;
    }
    else if(changePassCount > 3){
      changePassCount = 0;
      numberOfKeysPressed = 0;
      changePass = false;
    }

    
    //change password
    if(keyArray[0] == 1 && keyArray[1] == 2 &&
       keyArray[2] == 1 && keyArray[3] == 2){
         changePass = true;
         ActivateKeySound = false;
    }

    //If the password is palindrome
    else if(numberOfKeysPressed > 3 && 
       keyArray[0] == keyPass[3] && keyArray[1] == keyPass[2] &&
       keyArray[2] == keyPass[1] && keyArray[3] == keyPass[0] &&
       keyArray[0] == keyPass[0] && keyArray[1] == keyPass[1] &&
       keyArray[2] == keyPass[2] && keyArray[3] == keyPass[3]){
         EmojiMode = !EmojiMode;
         digitalWrite(EmojiModeIndecator,EmojiMode);
         numberOfKeysPressed = 0;
         ActivateKeySound = false;
    }
    //change to emogi mode
    else if(numberOfKeysPressed > 3 && 
       keyArray[0] == keyPass[0] && keyArray[1] == keyPass[1] &&
       keyArray[2] == keyPass[2] && keyArray[3] == keyPass[3]){
         EmojiMode = true;
         digitalWrite(EmojiModeIndecator,EmojiMode);
         numberOfKeysPressed = 0;
         ActivateKeySound = false;
    } 
    //change to numeric mode
    else if(numberOfKeysPressed > 3 && 
       keyArray[0] == keyPass[3] && keyArray[1] == keyPass[2] &&
       keyArray[2] == keyPass[1] && keyArray[3] == keyPass[0]){
         EmojiMode = false;
         digitalWrite(EmojiModeIndecator,EmojiMode);
         numberOfKeysPressed = 0;
         ActivateKeySound = false;
    }
    else{
      ActivateKeySound = true;
    }

    ActivateAudioOut();
  }
}


void ActivateAudioOut(){
  int tone_ = ActivateKeySound? (EmojiMode? melody[keyPressed] : melody[keyPressed]+2000) : Passfreq;
  long duration = ActivateKeySound? 100000 : 1000000; // Set up timing  
  long elapsed_time = 0;
  
  while (elapsed_time < duration) {
      digitalWrite(AudioOut,HIGH);
      delayMicroseconds(tone_ / 2);

      digitalWrite(AudioOut, LOW);
      delayMicroseconds(tone_ / 2);

      // Keep track of how long we pulsed
      elapsed_time += (tone_);
    }
}
