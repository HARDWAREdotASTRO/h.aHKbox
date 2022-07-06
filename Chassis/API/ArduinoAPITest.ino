// Include the Wire library for I2C
#include <Wire.h>




// LED on pin 13
char msg;
int yesNo;
byte me = 0x08;
int randomBytes;
int randomVal;
bool alreadySent;
bool send;
int funcVal;
int temp;

void setup() {
  // Join I2C bus as follower with address 8
  Wire.begin(0x08);
  Serial.begin(9600);

  // Call receiveEvent when data received

  //Wire.onReceive(receiveEvent);
  Wire.onRequest(sendData); 
  Wire.onReceive(receiveData);
  randomSeed(analogRead(0));

}

template <typename T_ty> struct TypeInfo { static const char * name; };
template <typename T_ty> const char * TypeInfo<T_ty>::name = "unknown";

// Handy macro to make querying stuff easier.
#define TYPE_NAME(var) TypeInfo< typeof(var) >::name

// Handy macro to make defining stuff easier.
#define MAKE_TYPE_INFO(type)  template <> const char * TypeInfo<type>::name = #type;

// Type-specific implementations.
MAKE_TYPE_INFO( int )
MAKE_TYPE_INFO( float )
MAKE_TYPE_INFO( short )
MAKE_TYPE_INFO( byte )
MAKE_TYPE_INFO( char )

void receiveData(int byteCount) {


  while(Wire.available()){
    for(int i = 0; i < byteCount; i++){
    byte c = Wire.read();
      Serial.print("I am reading: ");
      Serial.print(c, BIN);
      Serial.println(TYPE_NAME(c));
      Serial.println();
      if(c == me){
        //sendData(c);
        //Wire.write(0x18);
        send = true;
        alreadySent = false;
        funcVal = 0;
        
        
      }
      if(c == 0x58){

        send = false;
        alreadySent = true;
        funcVal = 1;
        
      }
      if(c == 0xa8){

        funcVal = 2;
      }
      //Serial.println("yes no value:" +  yesNo);
      //sendData(c);
    }
  }


}

// Function that executes whenever data is received from master

  void sendData(byte msg){
    yesNo = random(0,2);
    randomBytes = random(3, 16);

    //randomVal = random(0,200);
    int data[temp];
    
 
    //Serial.println(yesNo);
  //  Wire.write(0x18);

   if(msg == byte(0x08)){
    //Serial.print("1st if statement");
   // while( yn = 0){
  // yesNo = 0; //random(0,2);
   Wire.write(byte(0x18));
  // Serial.println("msg = 0x08 function is working :)");
  // test = true;
   }
   if(funcVal == 0){
    Wire.write(0x18);
   // temp = randomBytes;
    Wire.write(0x80);
    send = false;
    delay(100);
    
   }
   if (funcVal == 1 && yesNo == 0){
   // Serial.println(" poll ");
    Wire.write(0x18);
    temp = randomBytes;
    Wire.write(temp);
    alreadySent = false;
    Serial.println("random bytes being sent: " + temp);
    delay(100);
   }
   if (funcVal == 1 && yesNo == 1){
   // Serial.println(" poll ");
    Wire.write(0x18);
    Wire.write(0x80);
    alreadySent = false;
    delay(10);
   }
   if(funcVal == 2){
    data[0] = {0x38};
    Wire.write(data[0]);

    //data [6] = {0x38, 45, 203, 6, 65, 222};
    for(int i = 1; i<= temp; i++){
        //randomVal = (1,100);
        //data[0] = {0x38};
        data[i] = {random(0,200)};
        Wire.write(data[i]);
     
    }
    delay(10);
   }
   /*
    if(yesNo == 0){
      Serial.print("the only yesNo function");
      //YES!! Send data!
      //send two bytes of information as temp place holder test value

        //randomBytes = random(3,16);
      // logistical response
      //  Wire.write(0x18);
        // amount of data to needing to send
      //  Wire.write(6);
       // alreadySent = true;
      //  delay(50);
    }
    
       if(yesNo == 1 ){
// No! there is not data to be sent
// wait for Leader to send ack and move on
        Wire.write(0x18);
        Wire.write(0x80);
        delay(50);
   
      }
      */

      /*
      byte t = 0xa8;
   

        if(msg == t){
          Serial.println(":D");
          int temp = 6;//randomBytes;
          int data [temp] = {0x38, 4, 26, 45, 22, 5};
/*
          for(int k = 0; k < temp; k++){
            data[k] = {random(0,100)};
         */   
           // for(int j = 0; j< temp; j++) {
        
            //  int data[j] = {random(0,100)};
              //Wire.write(0x18);
              //Wire.write(randomBytes);

            //}
            //Wire.write(data[k]);
          
           // for(int i = 0; i < temp; i++){
           //    Wire.write(data[i]);
               //alreadySent = false;
               //delay(100);
               
            //}
           // alreadySent = false;
       // }
   //}
          
        }
          



          
        //delay(100);
        /*
        if(msg = 0xa8){
          
          int data[4] = {0x38, 0x02, 0xf4, 0x05f};
          for(int i =0; i < 4; i++){
            Wire.write(data[i]);
          
        }
        */
        //delay(100);
      
   
  //}
  // while()

    
   //}

  
//responds to initall poll function
/*
    if(msg = 0x08){ //was 0x01
     // byte yesNo =  1; //random(0,1);

      if (yesNo == 1){
        int data [2] = {24, 2};
        for (int i = 0; i < 2; i++){
          Wire.write(data[i]);
        }

       // Serial.println((int)msg);
       // Wire.write(2);
      }
      else if (yesNo == 0){
        Wire.write(24);
        Wire.write(128);
      }
     
    }
 
    


//responds with 17 or 00010001
  if((0xf0 && msg)== 0xa0){
    //Wire.write(data[i]);
       int data [2] = {86, 42};
        for (int i = 0; i < 2; i++){
          Wire.write(data[i]);
        }

  }
    if(msg = 0x61){ //0b0110 0001

      byte bootresponse = 0x11;
     // Serial.println("boot response");
      Wire.write(bootresponse);




    }
//      cc = Wire.read();
//      Serial.println(cc);

    if (msg = 0b00000001) {


   //   byte yesNo =  random(0,1);
/*
      if(yesNo = 0){
        byte yes = 0b00010001;
        byte information = 0b00100000;
        
        Wire.write(information);
      //  r = Wire.read();
        if (msg = 0b10000001 ) {
          byte me = 0b00010000;
          Serial.println(msg);
       //   byte randomvalue = random(1,12);
        //  Serial.println(randomvalue);
         // Wire.write((me<< 8) | randomvalue);
         // ack = Wire.read();
          if (msg = 72) {
            Serial.println("value received");
            
          }
          
        }
        
      }
//      if(yesNo = 1) {
// this value here writes the 9 value
   //     Wire.write(10000000);
        
   //   }
  //    else {
        //Wire.write("7");
   //   }
      
      */

  //  }


   // }
  //}
// Generic catch-all implementation.


void loop() {
  //randomVal = random(2,200);





  //Serial.println(yesNo);
 // delay(1000);

   // for (int i = 0; i < randomBytes; i++){
    //int randomVal = random(0,254);

    //}

 // if(yesNo == 1){
 //   randomBytes = random(3, 20);
 //Serial.println(randomBytes);
//delay(100);
    
  //}

}