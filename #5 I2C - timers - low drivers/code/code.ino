#include <Arduino.h>

#define HRZ 1300 //Clock frequency
#define ADDRESS 48 // 24 * 2 


#define STOP_CLOCK TCCR1B &= ~(1<<CS12) & ~(1<<CS11) & ~(1<<CS10)
#define PRESCALER_1 TCCR1B &= ~(1<<CS12) & ~(1<<CS11); TCCR1B |= 1<<CS10; OCR1A = floor(16000000/HRZ/2 - 1)
#define PRESCALER_8 TCCR1B &= ~(1<<CS12) & ~(1<<CS10); TCCR1B |= 1<<CS11; OCR1A = floor(2000000/HRZ/2 - 1)
#define PRESCALER_64 TCCR1B &= ~(1<<CS12); TCCR1B |= 1<<CS11 | 1<<CS10; OCR1A = floor(250000/HRZ/2 - 1)
#define PRESCALER_256 TCCR1B &= ~(1<<CS11) & ~(1<<CS10); TCCR1B |= 1<<CS12; OCR1A = floor(62500/HRZ/2 - 1)
#define PRESCALER_1024 TCCR1B &= ~(1<<CS11); TCCR1B |= 1<<CS10 | 1<<CS12; OCR1A = floor(15625/HRZ/2 - 1)


#define MASTER_CLK PORTD7 //pin 7
#define MASTER_DATA PORTD4 //pin 4
#define SLAVE_CLK PORTB0 //pin 8 
#define SLAVE_DATA PORTB1 //pin 9

volatile int counter = 0;
volatile int data = 0; // data=0 -> sending address

// Slave
int Address[] = {0, 0, 1, 1, 0, 0, 0, 0};
volatile bool CommStarted = false;
volatile bool Data[8];
volatile int Counter = 0;
volatile bool MyAddress = false;
volatile bool dontPrintAddressFlag = false;



void Master();
void Slave();
int ByteToInt(bool Data[]);


void setup() {

  Serial.begin(9600);

  pinMode(13,OUTPUT);
  digitalWrite(13,0);
  
  //Master
  DDRD |= 1<<MASTER_CLK; //set it to CLK
  DDRD |= 1<<MASTER_DATA; //set it to data

  //Slave
  DDRD &= ~(1<<SLAVE_DATA);
  PORTD |= (1<<SLAVE_DATA);
  DDRD &= ~(1<<SLAVE_CLK);
  PORTD |= (1<<SLAVE_CLK);

  //set the mode to CTC
  TCCR1A &= ~(1<<WGM11) & ~(1<<WGM10);
  TCCR1B &= ~(1<<WGM13);
  TCCR1B |= 1<<WGM12;
  
  PRESCALER_8;
  //OCR1A = 0;
  

  //set interupt controller
  TIMSK1 |= 1<<OCIE0A;

  //set clk interupt
  PCMSK2 |= (1<<SLAVE_CLK); 
  PCICR |= (1<<SLAVE_CLK);
  
  PORTD &= ~(1<<MASTER_CLK); //CLK initial value
  PORTD &= ~(1<<MASTER_DATA); //DATA initial value
}


ISR(TIMER1_COMPA_vect){
 
  Master();
  
  PIND |= 1<<MASTER_CLK;  // toggle clk
}

ISR(PCINT2_vect){
  Slave();
}


void Master(){
  if(counter < 8 && (PORTD>>MASTER_CLK) & 1){

    if(counter == 0){ // data pin -> output
      DDRD |= 1<<MASTER_DATA;
      
      if(data == 201){  //Stop Counter
        STOP_CLOCK;
        return;
      }
    }
    
    if(data < 1){
      switch(ADDRESS>>(7-counter) & 1){
        case true:
          PORTD |= 1<<MASTER_DATA;
          //Serial.println(1);
        break;
        case false:
          PORTD &= ~(1<<MASTER_DATA);
          //Serial.println(0);
        break;
      }
    }
    else{
      switch(data>>(7-counter) & 1){
        case true:
          PORTD |= 1<<MASTER_DATA;
          //Serial.println(1);
        break;
        case false:
          PORTD &= ~(1<<MASTER_DATA);
          //Serial.println(0);
        break;
      }
    }
    ++counter;
  }
  else if(counter == 8 && (PORTD>>MASTER_CLK) & 1){
    DDRD &= ~(1<<MASTER_DATA); // input 
    PORTD |= 1<<MASTER_DATA; // pull up
    counter = -1;
    ++data;
  }
  else if(counter == -1 && !((PORTD>>MASTER_CLK) & 1)){
    if( (PIND>>MASTER_DATA) & 1 != 0 )//ACK
      while(1){ digitalWrite(13,1);}  //No Ack recived 
    counter = 0;
  }
}


void Slave(){
  
  bool ClockValue = 1 & (PIND>>SLAVE_CLK);
  bool DataValue = 1 & (PIND>>SLAVE_DATA);
  
  if(!CommStarted && ClockValue == 0 && DataValue == 0){
    CommStarted = true;
    return;
  }

  if(CommStarted){
    if(ClockValue == 1 && Counter < 8){   //Positive Edge
      Data[Counter] = DataValue;
      Counter++;
    }
    else if(ClockValue == 0 && Counter >= 8){  //Negative Edge
      if(!MyAddress){
        int i;
        for(i=0; i<8; i++)
          if(Data[i] != Address[i])
            break;

        if(i == 8){
          MyAddress = true;
          Serial.println("My address :)");
  
          //Ack
          DDRD |= (1<<SLAVE_DATA);
          PORTD &= ~(1<<SLAVE_DATA);
          Counter++;
          return;
        }       
      }

      if(MyAddress){
        if(Counter == 8){
          ByteToInt(Data);

          //Ack
          DDRD |= (1<<SLAVE_DATA);
          PORTD &= ~(1<<SLAVE_DATA);
          Counter++;
        }
        else if(Counter == 9){  //Data 
          DDRD &= ~(1<<SLAVE_DATA);
          PORTD |= (1<<SLAVE_DATA);

          Counter = 0;
        }
      }
    }
  }
}


int ByteToInt(volatile bool Data[]){
  
  int Result = 0;
  for(int i=0; i<8; i++)
    Result |= (Data[7-i] << i) ;

  Serial.print("Byte: ");
  Serial.print(Result);
  Serial.print(", micros: ");
  Serial.println(micros());

  return Result;
}


void loop() {}
