/*
   Author : Sudharshan Acharya
   Tittle : The_lazy_code
    Date  : 2020-08-03

  "remembers the last state of all Switchs"
  "button 5,shutdown all the channels in 5 mins, indicated by blinking SMD"
    
*/

// NOT AT COMPLETED

#include <IRremote.h>
#include <EEPROM.h>
#define pass (void)0

const int channel1 = 6;
const int channel2 = 5;
const int channel3 = 4;
const int channel4 = 3;

const int ir_data = 8;
IRrecv irrecv(ir_data);
decode_results result;

unsigned long time;
bool waitKey = true;


void preStart(){
   for(int i = 0;i<4; i++)
    {
        if(!EEPROM.read(i))
        {
            int pinNo = i+6;
            digitalWrite(pinNo, LOW);
            delay(300);  
        }  
    }
  }

void setup() {
  Serial.begin(9600);
   pinMode(channel1,OUTPUT);
   pinMode(channel2,OUTPUT);
   pinMode(channel3,OUTPUT);
   pinMode(channel4,OUTPUT);
   irrecv.enableIRIn();
   preStart();
}

void shutDownChannels()
{
    for(int i = 0;i<4; i++)
    {
        if(!EEPROM.read(i))
        {
            int pinNo = i+6;
            digitalWrite(pinNo, HIGH);
            delay(300);  
        }  
    }  
}

void writeShutdown()
{
  EEPROM.write(0,1);
  EEPROM.write(1,1);
  EEPROM.write(2,1);
  EEPROM.write(3,1);
  
}

int timer()
{
    time = millis();
    while(true){
      if(irrecv.isIdle())
      {
          Serial.println("passes through if statement");
          irrecv.resume();
          delay(50);
          Serial.println("pass");
          
          if(!result.value)
          {
            result.value = 1;  
          }
          
          switch(result.value)
          {
            case 0x207BEF0F: return 0;                          // no1
            case 0xE8455D8E: return 0;                          // no2
            case 0xCBB7E949: return 0;                          // no3
            case 0x2C1F3172: return 0;                          // no4
            case 0xF124B0B6: return 0;                          // power button
            case 0x905ED4F5: return 0;
            default :;
          }
          if(millis()-time == 7000)    //300000ms = 5mins
          {
          Serial.println("returned True");
          return 1;
        }
      }
    }
}



void loop() {
  if(irrecv.decode(&result)){
    Serial.println(result.value, HEX);
    switch(result.value)
    {
      case 0x207BEF0F:            // No:1 in sony tv remote
      {
      if(EEPROM.read(0))          // if previously relay is OFF state it enters to 'if' condition. 
      {
      
          //  'LOW' represents 'ON' state of relay, can be varied according to the need.
          digitalWrite(channel1,LOW);

          // EEPROM.write(0,0) --> set the byte value '0' in "zero th" address in the memory
          EEPROM.write(0,0);
          
          Serial.println("channel1 ON");
          Serial.println(EEPROM.read(0));  
      }
      else                        // if previously relay is ON state then it enters to else condition
      {
           digitalWrite(channel1, HIGH);
           EEPROM.write(0,1);
           Serial.println("channel is off");
           Serial.println(EEPROM.read(0));
      }
      }
      break;

      case 0xE8455D8E:                             // No:2 in sony tv remote
      {
      if(EEPROM.read(1))
      {
          digitalWrite(channel2,LOW);
          EEPROM.write(1,0);
          Serial.println("channel ON");
          Serial.println(EEPROM.read(1));
      }
      else
      {
           digitalWrite(channel2, HIGH);
           EEPROM.write(1,1);
           Serial.println("channel is off");
           Serial.println(EEPROM.read(1));
      }
      }
      break;

      case 0xCBB7E949:                                //No:3 in sony tv remote
      {
       if(EEPROM.read(2))
      {
          digitalWrite(channel3,LOW);
          EEPROM.write(2,0);
          Serial.println("channel ON");  
          Serial.println(EEPROM.read(2));
      }
      else
      {
           digitalWrite(channel3, HIGH);
           EEPROM.write(2,1);
           Serial.println("channel is off");
           Serial.println(EEPROM.read(2));
      }
      }
      break;

      case 0x2C1F3172:                              //No:4 in sony tv remote
      {
        if(EEPROM.read(3))
      {
          digitalWrite(channel4,LOW);
          EEPROM.write(3,0);
          Serial.println("channel ON"); 
          Serial.println(EEPROM.read(3)); 
      }
      else
      {
           digitalWrite(channel4, HIGH);
           EEPROM.write(3,1);
           Serial.println("channel is off");
           Serial.println(EEPROM.read(3));
      }
      }
      break;

      case 0x905ED4F5:                                   //  5 mins shutdown all   no: 5 in sony remote
      {   
          if(waitKey)
          {    
               waitKey = false;
               if(timer())
               {
                  shutDownChannels();
               }
          }
          else
          {
              waitKey = true;
              preStart();
          }
          
           
      }
      break;

      case 0xF124B0B6:                                   // power button in sony remote, shutdown 
      {
            shutDownChannels();
            writeShutdown();  
      }
      break;

      
      default:;
      }
      delay(200);                             // 200ms delay to recieve next signal
      irrecv.resume();
    
    }

}
