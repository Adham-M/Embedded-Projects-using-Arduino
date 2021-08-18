#define KeyInterupt 2

#define RedCream 5
#define YellowCream 6
#define Heater 7
#define ProductionLineMotor 8
#define Mixer 9
#define FloorValve 10
#define SugerValve 11
#define VanillaValve 12
#define EggValve 13

#define CakeStand1 A0
#define CakeStand2 A1
#define CakeStand3 A2
#define CakeStand4 A3
#define DecoratorArm A4
#define KeypadInput A5

#define StepsPerRev 32
#define CakeStandStepsPerRev 28

#define Keypad4 1022
#define Keypad3 786
#define Keypad2 681
#define Keypad1 568

//#define Speed600rpm 6250‬
#define Speed300rpm 12500
#define Speed60rpm 62

bool CakeStandMotorCases[4] = {1,0,0,0};

bool MixerState = true;
bool ProdLineState = true;

int decorationMode = 1;

void setup() {
  for(int i=5;i<A5;i++)
    pinMode(i,OUTPUT);

  pinMode(KeyInterupt,INPUT);
  pinMode(KeypadInput,INPUT);

  attachInterrupt(digitalPinToInterrupt(KeyInterupt), GetKeyPressed, RISING);
}

void loop() {
  //Opening eggs valve for 500 Milliseconds.
  controlValve(EggValve,true);
  delay(500);
  controlValve(EggValve,false);

  //Making 5 mixer rotations.
  moveStepper(Mixer,MixerState,6250,5);

  //Opening vanilla valve for 100 Milliseconds while making 4 mixer rotations.
  controlValve(VanillaValve,true);
  moveStepper(Mixer,MixerState,6250,1);
  controlValve(VanillaValve,false);
  moveStepper(Mixer,MixerState,6250,3);

  //Opening suger valve for 100 Milliseconds while making 10 mixer rotations.
  controlValve(SugerValve,true);
  moveStepper(Mixer,MixerState,6250,2);
  controlValve(SugerValve,false);
  moveStepper(Mixer,MixerState,6250,8);

  //Opening floor valve for 100 Milliseconds while making 4 mixer rotations, three times.
  for(int i=0;i<3;i++){
    controlValve(FloorValve,true);
    moveStepper(Mixer,MixerState,Speed300rpm,0.5);
    controlValve(FloorValve,false);
    moveStepper(Mixer,MixerState,Speed300rpm,3.5);
  }

  //Move the production line for 1 full rotation.
  moveStepper(ProductionLineMotor,ProdLineState,Speed60rpm,1);

  //Opening the heater for 1 second.
  digitalWrite(Heater,HIGH);
  delay(1000);
  digitalWrite(Heater,LOW);

  //Move the production line for 1 full rotation.
  moveStepper(ProductionLineMotor,ProdLineState,Speed60rpm,1);

  //Cooling down the cake for 1 second.
  delay(1000);

  //Decorating the cake.
  DecorateCake();
}

void controlValve(int valvePin, bool openValve){
  digitalWrite(valvePin, HIGH);
  delayMicroseconds(openValve? 600 : 1487);
  digitalWrite(valvePin, LOW);
  delayMicroseconds(openValve? 19400 : 18523);
}

void moveStepper(int motor, bool &flag, int mSpeed, float rotation){
  for(int i=0;i<StepsPerRev/2*rotation;i++){
    digitalWrite(motor, flag);
    flag = !flag;
    if(mSpeed != Speed60rpm)
      delayMicroseconds(mSpeed);
    else
      delay(mSpeed);
  }
}

void GetKeyPressed(){
  int input_kp = analogRead(KeypadInput);

  if (input_kp == Keypad1) decorationMode = 1;
  else if (input_kp == Keypad2) decorationMode = 2;
  else if (input_kp == Keypad3) decorationMode = 3;
  else if (input_kp == Keypad4) decorationMode = 4;
}

void DecorateCake(){
  switch (decorationMode) {
    case 1:
      controlDeroratorArm(1);
      delay(40);
      digitalWrite(RedCream,HIGH);
      rotateCakeStand(CakeStandStepsPerRev);
      digitalWrite(RedCream,LOW);
      controlDeroratorArm(2);
      delay(100);
      digitalWrite(YellowCream,HIGH);
      rotateCakeStand(CakeStandStepsPerRev);
      digitalWrite(YellowCream,LOW);

      break;
      
    case 2:
      for(int i=0;i<4;i++){
        controlDeroratorArm(1);
        delay(150);
        digitalWrite(RedCream,HIGH);
        rotateCakeStand(4);
        digitalWrite(RedCream,LOW);
        controlDeroratorArm(2);
        delay(150);
        digitalWrite(YellowCream,HIGH);
        rotateCakeStand(3);
        digitalWrite(YellowCream,LOW);
      }
      
      break;
    case 3:
      controlDeroratorArm(1);
      delay(150);
      for(int i=0;i<14;i++){
        digitalWrite(RedCream,HIGH);
        delay(100);
        digitalWrite(RedCream,LOW);
        rotateCakeStand(2);
        delay(50);
      }
      controlDeroratorArm(3);
      delay(150);
      digitalWrite(YellowCream,HIGH);
      delay(100);
      digitalWrite(YellowCream,LOW);
      
      break;
      
    case 4:
      controlDeroratorArm(1);
      delay(150);
      for(int i=0;i<7;i++){
        digitalWrite(RedCream,HIGH);
        delay(100);
        digitalWrite(RedCream,LOW);
        rotateCakeStand(2);
        delay(50);
        digitalWrite(YellowCream,HIGH);
        delay(100);
        digitalWrite(YellowCream,LOW);
        rotateCakeStand(2);
      }
      controlDeroratorArm(3);
      delay(150);
      digitalWrite(RedCream,HIGH);
      delay(600);
      digitalWrite(RedCream,LOW);
      
      break;    
  }
    
  controlDeroratorArm(4);
}

void controlDeroratorArm(int Position){
  digitalWrite(DecoratorArm, HIGH);
  delayMicroseconds(Position==1? 1400 : Position==2? 1250 : Position==3? 1100 : 1487);
  digitalWrite(DecoratorArm, LOW);
  delayMicroseconds(Position==1? 18600 : Position==2? 18750 : Position==3? 18900 : 18523);
}

void rotateCakeStand(int steps){

  for(int j=0; j < steps; j++){
 
    for(int i=0; i < 4; i++)
      digitalWrite(CakeStand1 + i, CakeStandMotorCases[i]);
  
    bool temp = CakeStandMotorCases[3];
    for(int i=3;i>0;i--)
      CakeStandMotorCases[i] = CakeStandMotorCases[i-1];  
    CakeStandMotorCases[0] = temp;

    delay(24);
  }
}
