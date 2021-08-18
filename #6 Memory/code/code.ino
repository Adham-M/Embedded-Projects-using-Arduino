#include <EEPROM.h>

const unsigned char myArr[] PROGMEM = {
    0,1,2,3,4,5,6,7,8,9,10,11,12,
    13,14,15,16,17,18,19,20,21,22,23,24,
    25,26,27,28,29,30,31,32,33,34,35,36,
    37,38,39,40,41,42,43,44,45,46,47,48,
    49,50,51,52,53,54,55,56,57,58,59,60,
    61,62,63,64,65,66,67,68,69,70,71,72,
    73,74,75,76,77,78,79,80,81,82,83,84,
    85,86,87,88,89,90,91,92,93,94,95,96,
    97,98,99,100,101,102,103,104,105,106,107,108,
    109,110,111,112,113,114,115,116,117,118,119,120,
    121,122,123,124,125,126,127,128,129,130,131,132,
    133,134,135,136,137,138,139,140,141,142,143,144,
    145,146,147,148,149,150,151,152,153,154,155,156,
    157,158,159,160,161,162,163,164,165,166,167,168,
    169,170,171,172,173,174,175,176,177,178,179,180,
    181,182,183,184,185,186,187,188,189,190,191,192,
    193,194,195,196,197,198,199,200,201,202,203,204,
    205,206,207,208,209,210,211,212,213,214,215,216,
    217,218,219,220,221,222,223,224,225,226,227,228,
    229,230,231,232,233,234,235,236,237,238,239,240,
    241,242,243,244,245,246,247,248,249,250,251,252,
    253,254,255,0,1,2,3,4,5,6,7,8,
    9,10,11,12,13,14,15,16,17,18,19,20,
    21,22,23,24,25,26,27,28,29,30,31,32,
    33,34,35,36,37,38,39,40,41,42,43,44,
    45,46,47,48,49,50,51,52,53,54,55,56,
    57,58,59,60,61,62,63,64,65,66,67,68,
    69,70,71,72,73,74,75,76,77,78,79,80,
    81,82,83,84,85,86,87,88,89,90,91,92,
    93,94,95,96,97,98,99,100,101,102,103,104,
    105,106,107,108,109,110,111,112,113,114,115,116,
    117,118,119,120,121,122,123,124,125,126,127,128,
    129,130,131,132,133,134,135,136,137,138,139,140,
    141,142,143,144,145,146,147,148,149,150,151,152,
    153,154,155,156,157,158,159,160,161,162,163,164,
    165,166,167,168,169,170,171,172,173,174,175,176,
    177,178,179,180,181,182,183,184,185,186,187,188,
    189,190,191,192,193,194,195,196,197,198,199,200,
    201,202,203,204,205,206,207,208,209,210,211,212,
    213,214,215,216,217,218,219,220,221,222,223,224,
    225,226,227,228,229,230,231,232,233,234,235,236,
    237,238,239,240,241,242,243,244,245,246,247,248,
    249,250,251,252,253,254,255,0,1,2,3,4,
    5,6,7,8,9,10,11,12,13,14,15,16,
    17,18,19,20,21,22,23,24,25,26,27,28,
    29,30,31,32,33,34,35,36,37,38,39,40,
    41,42,43,44,45,46,47,48,49,50,51,52,
    53,54,55,56,57,58,59,60,61,62,63,64,
    65,66,67,68,69,70,71,72,73,74,75,76,
    77,78,79,80,81,82,83,84,85,86,87,88,
    89,90,91,92,93,94,95,96,97,98,99,100,
    101,102,103,104,105,106,107,108,109,110,111,112,
    113,114,115,116,117,118,119,120,121,122,123,124,
    125,126,127,128,129,130,131,132,133,134,135,136,
    137,138,139,140,141,142,143,144,145,146,147,148,
    149,150,151,152,153,154,155,156,157,158,159,160,
    161,162,163,164,165,166,167,168,169,170,171,172,
    173,174,175,176,177,178,179,180,181,182,183,184,
    185,186,187
  };

void writeEepromInt(int address, int value){
  EEPROM.update(address, value);
  EEPROM.update(address + 1, value >> 8);
}

int readEepromInt(int address){
  int val;

  val = (EEPROM.read(address + 1) << 8);
  val |= EEPROM.read(address);

  return val;
}

bool checkIfExist(char fileName[], int& address){ // also get the file's address
  address = 0;
  int next = 1;
  
  while(1){
    next = readEepromInt(next);

    //Serial.print("Next to check file: ");
    //Serial.println(next);

    if(EEPROM[next+13] == 0)
      return false;

    bool flag = true;

    for(int i=0; i<12; ++i){
      if(fileName[i] != EEPROM[next+i]){
        flag = false;
        break;
      }
    }
    if(EEPROM[next+13] == 0)
      flag = false;

    if(EEPROM[next] == 0)
      return false;
  
    next += 13;

    if(!flag)
      continue;

    address = next - 13;
    return true;
  }
  
  return false;
}


unsigned char* readFile(char fileName[], int& arrSize){
  int address = 0;
  if(checkIfExist(fileName, address)){
    arrSize = readEepromInt(address + 15);
    unsigned char* arr = new unsigned char[arrSize];
    //Serial.println(arrSize);
    address += 17; //first byte of file's data
    for(int i=0; i<arrSize; ++i){
      arr[i] = EEPROM.read(address + i);
    }
    return arr;
  }
  else 
    return NULL;
}


void writeFile(char fileName[], const unsigned char *arr, const int arrSize){
  int address = 0;
  int next = 3;
  int data = 1;

  EEPROM.update(0, EEPROM[0] + 1);
  
  while(data != 0){
    data = readEepromInt(next + 13);
    if(data != 0)
      next = readEepromInt(next + 13);
  }

  for(int i=0; i<12; ++i){
    EEPROM[next+i] = fileName[i];
  }
  EEPROM[next+12] = 1; // exist flag
  writeEepromInt(next+13, next + 18 + arrSize);
  EEPROM[next + 18 + arrSize] = 0;
  writeEepromInt(next + 18 + 13 + arrSize, 0);
  Serial.print("Next file: ");
  Serial.println((next + 18 + arrSize));
  writeEepromInt(next+15, arrSize);

  next += 17; //first address to write file data
  for(int i=0; i<arrSize; ++i){
    EEPROM.update(next+i, arr[i]);
  }
}

void deleteFile(char fileName[]){

  int before = 1;
  int next = 3;
  EEPROM.update(0, EEPROM[0] - 1);
  
  while(1){
    next = readEepromInt(before);

    if(EEPROM[next] == 0)
      return;

    bool flag = true;

    for(int i=0; i<12; ++i)
      if(fileName[i] != EEPROM[next+i]){
        flag = false;
        break;
      }

    next += 13;

    if(!flag){
      before = next;
      continue;
    }

    if (EEPROM[0] == 0) {
      writeEepromInt(1, 3); // position of the first file
      writeEepromInt(3 + 13, 0); // first file
      return;
    }

    EEPROM[next - 13] = 0;
    EEPROM[next - 1] = 0; //setting flag to 0

    if( EEPROM[readEepromInt(next)] != 0)
      writeEepromInt(before, readEepromInt(next));
  }
}

int getUsedMemory(){
  int count = 0;
  int next = 1;
  
  while(1){
    next = readEepromInt(next);

    if (!(EEPROM[next + 13] == 0 || EEPROM[next] == 0))
      count += readEepromInt(next + 15);
    else
      return count;
  
    next += 13;
  }
}





void setup() {
  Serial.begin(9600);
  
  EEPROM.update(0, 0); // number of files
  writeEepromInt(1,3); // position of the first file
  writeEepromInt(3+13,0); // first file

  
  ////////////////////////////////////////////////////////////////


  int t;
  char fileName1[] = "test1___.txt";
  unsigned char myArr1[700];
  for(int i=0; i<700; ++i)
    myArr1[i] = myArr[i];

  
  writeFile(fileName1, myArr1, 700);
  
  if(checkIfExist(fileName1 ,t))
    Serial.println(F("File test1___.txt exists"));
  else
    Serial.println(F("File test1___.txt does not exists [ERROR]"));

  int arrSize;
  unsigned char* arr = readFile(fileName1, arrSize);

  bool correct = true;
  for(int i=0; i<arrSize; ++i){
    if(arr[i] != myArr1[i]){
      correct = false;
      Serial.print(arr[i]);
      Serial.print(" != ");
      Serial.println( myArr[i]);
      break;
    }
  }
  if(correct)
    Serial.println(F("Read array data Correct"));
  else
    Serial.println(F("Read array data wrong [ERROR]"));

  Serial.print(F("Memory used: "));
  Serial.print(getUsedMemory());
  Serial.println(F(" data Bytes"));

  deleteFile(fileName1);
  
  if(checkIfExist(fileName1 ,t))
    Serial.println(F("File test1___.txt exists [ERROR]"));
  else
    Serial.println(F("File test1___.txt does not exists"));
    

  //Serial.println(readEepromInt(1)); // next file (first)

  Serial.print(F("Memory used: "));
  Serial.print(getUsedMemory());
  Serial.println(F(" data Bytes"));
  
  char fileName2[] = "test2___.txt";
  char fileName3[] = "test3___.txt";
  char fileName4[] = "test4___.txt";

  unsigned char myArr2[100];
  for(int i=0; i<100; ++i)
    myArr2[i] = myArr[i];
    
  unsigned char myArr3[100];
  for(int i=0; i<100; ++i)
    myArr3[i] = myArr[i+100];


  writeFile(fileName2, myArr2, 100);
  writeFile(fileName3, myArr3, 100);
  
  if(checkIfExist(fileName2 ,t)){
    Serial.print(F("File 2 location : "));
    Serial.println(t);
  }
  if(checkIfExist(fileName3 ,t)){
    Serial.print(F("File 3 location : "));
    Serial.println(t);
  }

  unsigned char myArr4[500];
  for(int i=0; i<500; ++i)
    myArr4[i] = myArr[i+200];


  writeFile(fileName4, myArr4, 500);

  if(checkIfExist(fileName2 ,t)){
    Serial.print(F("File 2 location : "));
    Serial.println(t);
  }
  if(checkIfExist(fileName3 ,t)){
    Serial.print(F("File 3 location : "));
    Serial.println(t);
  }
  if(checkIfExist(fileName4 ,t)){
    Serial.print(F("File 4 location : "));
    Serial.println(t);
  }

  Serial.print(F("Memory used: "));
  Serial.print(getUsedMemory());
  Serial.println(F(" data Bytes"));

  Serial.println(F("Deleting all files"));

  deleteFile(fileName2);
  deleteFile(fileName3);
  deleteFile(fileName4);  

  Serial.print(F("Memory used: "));
  Serial.print(getUsedMemory());
  Serial.println(F(" data Bytes"));
}

void loop() {
  while(1){}
}
