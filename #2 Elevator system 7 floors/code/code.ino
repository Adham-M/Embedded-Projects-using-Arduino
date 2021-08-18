/**
 * The elevator checks weight only
 * if the elevator will start to move
 * after the stoppage. So it won't stop
 * on its way.
 */

 //Pins configuration  
  #define PushButtonPressed 2
  #define KeypadPressed 3
  #define GroundFloor_switch 4
  #define FirstFloor_switch 5
  #define SecondFloor_switch 6
  #define ThirdFloor_switch 7
  #define FourthFloor_switch 8
  #define FifthFloor_switch 9
  #define SixthFloor_switch 10
  #define SeventhFloor_switch 11

  #define SevenSegment_1 0
  #define SevenSegment_2 12
  #define SevenSegment_3 13
  
  #define Motor_4 A0
  #define Motor_3 A1
  #define Motor_2 A2
  #define Motor_1 A3
  
  #define WightIndecatorLed A4
  #define WightSensor A5


 //Constants
  #define stepsPerRevolution 32 // Motor's steps per revolution 
  #define WightLimit 70
  #define MotorSpeed 60 // Motor's rpm
  #define KeyDelay 200
  #define ElevatorStopageDelay 1000
  

 //Motor Control
  bool MotorCases[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
  int NextMotorCase = 3;
  int const MotorDelay = round(MotorSpeed / 60 * 1000 / stepsPerRevolution + 0.5);
  int MotorSteps = 0;
  int curruntFloor = 0;
  int TargetFloor = 0;
  bool MotorMoving = false;
  bool OverWeight = false;

 //Timing delay 
  unsigned long currentMillis;
  unsigned long MotorTime = 0;
  unsigned long ElevatorStoppageTime = 0;
  unsigned long KeyTime [8];
  
 //Keys control
  bool KeyCheck [8] = {false,false,false,false,false,false,false,false};
  bool OrderedFloor [8] = {false,false,false,false,false,false,false,false};
  int KeysQueue[8];
  int FloorsOrdered = 0;

  int ShownFloor = 0;
  bool ShowingDirection = false;


void setup() {
  pinMode(SevenSegment_1,OUTPUT);
 
  pinMode(WightSensor,INPUT);
  //pinMode(WightIndecatorLed,OUTPUT);

  for(int i=2; i<12; i++)
    pinMode(i,INPUT);

  for(int i=12; i<19; i++)
    pinMode(i,OUTPUT);

  attachInterrupt(digitalPinToInterrupt(PushButtonPressed), SeePushButtonPressed, CHANGE);
  attachInterrupt(digitalPinToInterrupt(KeypadPressed), SeeKeypadPressed, CHANGE);
}

//--------------------------Main Loop--------------------------//
void loop() {
  currentMillis = millis();

  if(ShownFloor != curruntFloor)
    ShowCurrentFloor();
    
  for(int i=0; i<8; i++)
    if(KeyCheck[i])
      CheckKeys();

  if(FloorsOrdered > 0){
    if((!MotorMoving || MotorSteps == 0) && !OverWeight){

      OrderedFloor[KeysQueue[0]] = false;
      TargetFloor = KeysQueue[0];
      MotorSteps = (TargetFloor - curruntFloor) * stepsPerRevolution;
      MotorMoving = true;
      
    }
    else if(MotorSteps%32 == 0 && OrderedFloor[curruntFloor]){
      ElevatorStoppageTime = currentMillis;

      for(int i=0;i<FloorsOrdered-1;i++)
        if(KeysQueue[i] == curruntFloor){
          for(int j=i;j<FloorsOrdered-1;j++)
            KeysQueue[j] = KeysQueue[j+1];
         break;
        }
      
      FloorsOrdered--;
      OrderedFloor[curruntFloor] = false;
      MotorMoving = false;
    }
  }

  if(!MotorMoving || MotorSteps == 0)
    OverWeight = !CheckWeight();

  if(MotorSteps!=0 && ElevatorCanMove() && !OverWeight){
    MotorMoving = true;
    MoveMotor(MotorSteps);
  }  
}
//-------------------------------------------------------------//

void ShowCurrentFloor(){
  int tempnum = curruntFloor;
  ShownFloor = curruntFloor;

  digitalWrite(SevenSegment_1,tempnum % 2);
  tempnum /= 2;
  digitalWrite(SevenSegment_2,tempnum % 2);
  tempnum /= 2;
  digitalWrite(SevenSegment_3,tempnum % 2);
   
}

void SeePushButtonPressed(){
  for(int i=0; i < 8; i++)
    if(digitalRead(GroundFloor_switch + i) && !KeyCheck[i] && !OrderedFloor[i]){
      KeyTime[i] = currentMillis;
      KeyCheck[i] = true;
    }
}

void SeeKeypadPressed(){
  for(int i=0; i < 8; i++)
    if(digitalRead(GroundFloor_switch + i) && !OrderedFloor[i]){
      AddKey(i);
    }
}

void CheckKeys(){
  for(int i=0; i < 8; i++)
    if(digitalRead(GroundFloor_switch + i)){
      if (KeyCheck[i] && (unsigned long)(currentMillis - KeyTime[i]) >= KeyDelay){
        AddKey(i);
      }
    }
    else if(KeyCheck[i]){
      KeyCheck[i] = false;
    }
}

void AddKey(int f){
  KeyCheck[f] = false;
  if(!(f==curruntFloor && MotorSteps%32 == 0) && TargetFloor != f){
    OrderedFloor[f] = true;
    KeysQueue[FloorsOrdered] = f;
    FloorsOrdered++;
  }
}

bool CheckWeight(){
  //return true it the weight is safe
  
  int sensorValue = analogRead(WightSensor);
  // Convert the analog reading (which goes from 0 - 1023) to a weight (0 - 100 Kg):
  float wight = sensorValue * (100.0 / 1023.0);
  
  if (wight <= WightLimit){
    digitalWrite(WightIndecatorLed,LOW);
    return true;  
  }
  digitalWrite(WightIndecatorLed,HIGH);
  return false;
}

bool ElevatorCanMove(){
  return ((unsigned long)(currentMillis - ElevatorStoppageTime) >= ElevatorStopageDelay);
}

void MoveMotor(int &steps){
  if(steps != 0){
    if ((unsigned long)(currentMillis - MotorTime) >= MotorDelay) {
      MotorTime = currentMillis; // save the "current" time
  
      if(steps > 0){
        NextMotorCase = NextMotorCase!=3? NextMotorCase+1 : 0; 
        steps--;
        if(steps%32 == 0)
          curruntFloor++;
      }
      else if(steps < 0){
        NextMotorCase = NextMotorCase!=0? NextMotorCase-1 : 3; 
        steps++;
        if(steps%32 == 0)
          curruntFloor--;
      }
      
      for(int i=0; i < 4; i++)
        digitalWrite(Motor_4 + i, MotorCases[NextMotorCase][i]);

      if(steps == 0){
       MotorMoving = false;
       ElevatorStoppageTime = currentMillis;
       for(int i=0;i<FloorsOrdered-1;i++)
         KeysQueue[i] = KeysQueue[i+1];
       FloorsOrdered--;
      }
    }
  }
}
