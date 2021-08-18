//Pins configuration
//Inputs
#define Right_echoPin 0 
#define Forward_echoPin 1
#define FireSensor 2
#define PowerSwitch 3
#define Left_echoPin 4
//Outputs
#define Fan_1 5
#define Fan_2 6
#define trigPin 7
#define BlinkLed 8
#define Left_Motor_1 9
#define Left_Motor_2 10
#define Right_Motor_1 11
#define Right_Motor_2 12
#define Reverse_Motion_indicator 13


//Constants
#define stepsPerRevolution 32 // Motors' steps per revolution 
#define interval 1000   //The time we need to wait to change the led state (in milli sec.)
#define SafeDistance 100 //The max distance that should be between the sensors and an obstacle in (cm)

//Define variables:
  //For leds
  bool ledState = false; // state variable for the led
   
  //For motors
  int RightSteps = 0, LeftSteps = 0;
  bool MovingInReverse = false, MotorMotion = false;

  //For fire fighting fan
  bool Start2CheckFire = false, OnFire = false, FanMotion = false;
   
  //Timing delay 
  unsigned long previousMillis = 0, MotorTime = 0, FanTime = 0, WaveTime, IntialFireTime;
  
void setup() {


  for(int i = 0; i < 14; i++)
    i < 5? pinMode(i, INPUT) : pinMode(i, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(FireSensor), ActivateFan, RISING);
}

void loop() {

  if(digitalRead(PowerSwitch)){
    //Blink The Led
    unsigned long currentMillis = millis(); 
    if ((unsigned long)(currentMillis - previousMillis) >= interval) {
      ledState = !ledState; // "toggles" the state
      digitalWrite(BlinkLed, ledState); // sets the LED based on ledState
      previousMillis = currentMillis; // save the "current" time
    }

    if(!OnFire){
      //Check fan activation
      if(Start2CheckFire && digitalRead(FireSensor)
             && (unsigned long)(currentMillis - IntialFireTime) >= 500)
          OnFire = true;
      
      //Check where can the robot go
      if(!MovingInReverse){
        if(!CanGo(Forward_echoPin)){
          if(LeftSteps <= 0 && RightSteps <= 0 && CanGo(Right_echoPin))
            RightSteps = stepsPerRevolution;
          else if(RightSteps <= 0 && LeftSteps <= 0 && CanGo(Left_echoPin))
            LeftSteps = stepsPerRevolution;
          else if(RightSteps <= 0 && LeftSteps <= 0)
            MovingInReverse = true;
        }
      }
      else {
        if(CanGo(Right_echoPin)){
          RightSteps = stepsPerRevolution;
          MovingInReverse = false;
        }
        else if(CanGo(Left_echoPin)){
          LeftSteps = stepsPerRevolution;
          MovingInReverse = false;
        }
      }
      
      //MoveMotors
      if ((unsigned long)(currentMillis - MotorTime) >= 32) {
        MotorMotion = !MotorMotion;
        MotorTime = currentMillis; // save the "current" time
      
        if(!MovingInReverse){
          digitalWrite(Reverse_Motion_indicator, LOW);
          if(LeftSteps <= 0){
            digitalWrite(Right_Motor_1, MotorMotion);
            digitalWrite(Right_Motor_2, !MotorMotion);
            RightSteps--;
          }
          if(RightSteps <= 0){
            digitalWrite(Left_Motor_1, MotorMotion);
            digitalWrite(Left_Motor_2, !MotorMotion);
            LeftSteps--;
          }
        }
        else{
          digitalWrite(Reverse_Motion_indicator, HIGH);
          digitalWrite(Right_Motor_1, !MotorMotion);
          digitalWrite(Right_Motor_2, MotorMotion);
          digitalWrite(Left_Motor_1, !MotorMotion);
          digitalWrite(Left_Motor_2, MotorMotion);
        }
      }
    }
    else if(OnFire){
      if ((unsigned long)(currentMillis - FanTime) >= 25) {
        FanMotion = !FanMotion; // "toggles" the state
        FanTime = currentMillis; // save the "current" time

        digitalWrite(Fan_1, !FanMotion);
        digitalWrite(Fan_2, FanMotion);
      }
    }
  }
  else if(ledState)
    digitalWrite(BlinkLed, LOW); 
}

bool CanGo(int echoPin){ 
  digitalWrite(trigPin, LOW); 

  digitalRead(echoPin); // Delay 4.9 micro sec
 
  digitalWrite(trigPin, HIGH);
  
  digitalRead(echoPin); // Delay 9.8 micro sec
  digitalRead(echoPin); 
  
  digitalWrite(trigPin, LOW);

  while(1)
    if(digitalRead(echoPin)){
      WaveTime = micros();
      break;
    }   
  while(1)
    if(!digitalRead(echoPin))
      return ((micros() - WaveTime)/2) * 0.0344 > SafeDistance;
}

void ActivateFan(){
  if(!OnFire){
    IntialFireTime = millis();
    Start2CheckFire = true;
  }
  else{
    OnFire = !OnFire;
    Start2CheckFire = false;
  }
}
