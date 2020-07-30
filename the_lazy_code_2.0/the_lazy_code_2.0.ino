#include <IRremote.h>
#include <EEPROM.h>

/////////////////////////////////////

const int channel1 = 3;
const int channel2 = 4;
const int channel3 = 5;
const int channel4 = 6;

/*
(1)--here in the relay module high resembles off state & low resembles on  state
*/

bool switch1 =  false;
bool switch2 = false;
bool switch3 = false;
bool switch4 = false;


const int ir_data = 8;
IRrecv irrecv(ir_data);
decode_results results;

int addr[4];
///////////////////////////////////

void preStart(){
   digitalWrite(channel1,HIGH);
   digitalWrite(channel2,EEPROM.read(addr[1]));
   digitalWrite(channel3,EEPROM.read(addr[2]));
   digitalWrite(channel4,EEPROM.read(addr[3]));
  }

void setup(){
  Serial.begin(9600);
   pinMode(channel1,OUTPUT);
   pinMode(channel2,OUTPUT);
   pinMode(channel3,OUTPUT);
   pinMode(channel4,OUTPUT);
   irrecv.enableIRIn();
   preStart();
}
                                              

void loop(){
  if(irrecv.decode(&results)){
          //Serial.println(results.value, HEX);
      switch(results.value){
         case 0x207BEF0F:                                       //channel 1
               if(EEPROM.read(addr[0]))
               {
                  digitalWrite(channel1,HIGH);
                  Serial.println("channel1 is off state");
                  EEPROM.write(addr[0],0 );
                  Serial.println(EEPROM.read(addr[0]));
                  
                  
                  switch1 = false;
                }
                else
                {
                  digitalWrite(channel1,LOW);
                  Serial.println("channel1 is on state");
                  EEPROM.write(addr[0],1);
                  Serial.println(EEPROM.read(addr[0]));
                  
                  switch1 = true;

                 } 
         break;

         case 0xE8455D8E:                                             //channel 2
               if(switch2)
               {
                  digitalWrite(channel2,HIGH);
                  Serial.println("channel2 is off state");
                  EEPROM.write(addr[1],switch2);
                  Serial.println(EEPROM.read(addr[1]));
                  
                  switch2 = false;
                }
                else
                {
                  digitalWrite(channel2,LOW);
                  Serial.println("channel2 is on state");
                  EEPROM.write(addr[1],switch2);
                  Serial.println(EEPROM.read(addr[1]));
                  
                  switch2 = true;
                 }
         break;

         case 0xCBB7E949:                                       //channel 3
              if(switch3)
              {
                digitalWrite(channel3,HIGH);
                Serial.println("channel3 is off state");
                EEPROM.write(addr[2],switch3);
                 Serial.println(EEPROM.read(addr[2]));

                switch3 = false;
                
               }
                 else
                 {
                  digitalWrite(channel3,LOW);
                  Serial.println("channel3 is on state");
                  EEPROM.write(addr[2],switch3);
                  Serial.println(EEPROM.read(addr[2]));
                  
                  switch3 = true;
                 }
         break;

         case 0x2C1F3172:                                 //channel 4
                if(switch4){
                  digitalWrite(channel4,HIGH);;
                  Serial.println("channel4 is off state");
                  EEPROM.write(addr[3],switch4);
                  Serial.println(EEPROM.read(addr[3]));

                  switch4 =  false;
                  
                }
                else 
                {
                   digitalWrite(channel4,LOW);
                   Serial.println("channel4 is on state");
                   EEPROM.write(addr[3],switch4); 
                  Serial.println(EEPROM.read(addr[3]));
                  
                   switch4 = true;
                   
                 }
         break;

         default:; 
        }
        delay(500);
        irrecv.resume();
        }
}
