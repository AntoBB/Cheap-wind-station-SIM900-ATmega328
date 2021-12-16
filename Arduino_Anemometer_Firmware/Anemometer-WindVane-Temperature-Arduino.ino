//*****************Another Arduino anemometer sketch******************************
#include <OneWire.h>
#include <DallasTemperature.h>

// One Wire Temperature Sensor Pin
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const byte interruptPin = 3; //anemomoter input to digital pin
volatile unsigned long sTime = 0; //stores start time for wind speed calculation
unsigned long dataTimer = 0; //used to track how often to communicate data
volatile float pulseTime = 0; //stores time between one anemomter relay closing and the next
volatile float culPulseTime = 0; //stores cumulative pulsetimes for averaging
volatile bool start = true; //tracks when a new anemometer measurement starts
volatile unsigned int avgWindCount = 0; //stores anemometer relay counts for doing average wind speed
float aSetting = 60.0; //wind speed setting to signal alarm

//-----------------WindVane Parameters
int WindVanePin = A0;
unsigned long WindVaneDuration;
String WindName;

//-------------------------------------------------------------------------

//**************** Sim900 Parameters ----------------------------------

#include <SoftwareSerial.h>
SoftwareSerial GSM(7, 8); // RX, TX PIN

enum _parseState {
  PS_DETECT_MSG_TYPE,

  PS_IGNORING_COMMAND_ECHO,

  PS_HTTPACTION_TYPE,
  PS_HTTPACTION_RESULT,
  PS_HTTPACTION_LENGTH,

  PS_HTTPREAD_LENGTH,
  PS_HTTPREAD_CONTENT
};

byte parseState = PS_DETECT_MSG_TYPE;
char buffer[80];
byte pos = 0;

int contentLength = 0;

void resetBuffer() {
  memset(buffer, 0, sizeof(buffer));
  pos = 0;
}

void sendGSM(const char* msg, int waitMs = 500) {
  GSM.println(msg);
  delay(waitMs);
  while(GSM.available()) {
    parseATText(GSM.read());
  }
}

//-------------------------------------------------------------------------

void setup() {
  
  pinMode(WindVanePin, INPUT);
  sensors.begin();
  
  pinMode(13, OUTPUT); //setup LED pin to signal high wind alarm condition
  pinMode(interruptPin, INPUT_PULLUP); //set interrupt pin to input pullup
  attachInterrupt(interruptPin, anemometerISR, RISING); //setup interrupt on anemometer input pin, interrupt will occur whenever falling edge is detected
  dataTimer = millis(); //reset loop timer
  GSM.begin(9600);     //------------------ Sim900 Setup
}

void loop() {
  
  WindVaneDuration = pulseIn(WindVanePin, HIGH);
  sensors.requestTemperatures();
  
  unsigned long rTime = millis();
  if((rTime - sTime) > 2500) pulseTime = 0; //if the wind speed has dropped below 1MPH than set it to zero
     
  if((rTime - dataTimer) > 1000){ //See if it is time to transmit
   
    detachInterrupt(interruptPin); //shut off wind speed measurement interrupt until done communication
    culPulseTime = 0; //reset cumulative pulse counter
    avgWindCount = 0; //reset average wind count

    float aFreq = 0; //set to zero initially
    if(pulseTime > 0.0) aFreq = getAnemometerFreq(pulseTime); //calculate frequency in Hz of anemometer, only if pulsetime is non-zero
    
    Serial.begin(9600);
    //-------------------------Debug-------------------------------------
    //Serial.begin(9600); //start serial monitor to communicate wind data
    //Serial.println();
    //Serial.println("...................................");
    //Serial.print("Anemometer speed in Hz ");
    //Serial.println(aFreq);
    //Serial.println("Wind Direction: " + WindVaneDuration);
    //Serial.print("Temperature: ");
    //Serial.println(sensors.getTempCByIndex(0)); 
    //--------------------------------------------------
    
    //serial uses interrupts so we want to turn it off before we turn the wind measurement interrupts back on
   
    start = true; //reset start variable in case we missed wind data while communicating current data out
    attachInterrupt(digitalPinToInterrupt(interruptPin), anemometerISR, RISING); //turn interrupt back on
    dataTimer = millis(); //reset loop timer

  //Need to build the string to send via GSM SMS:
  // http://www.mywebsite.com/AnemometerReceiver.php?speed=3.45&direction=110000&temp=21.1
  //  *speed =>       aFreq                       anemometer frequency
  //  *direction =>   WindVaneDuration            WindVane Direction
  //  *temp =>        sensors.getTempCByIndex(0)  temperature Sensor
  //----------------------------------------------------------------------

  //Later convert it into CharArray to build HTTP POST by SIM900
  // req11 is the "mainstring"
  
  char req1[] = "AT+HTTPPARA=\"URL\",\"http://www.insanetech.it/receiver.php?speed=";
  String req11 = String(req1);
  String aFrequency = String(aFreq);
  req11.concat(aFrequency);
  
  char req2[] = "&direction=";
  String req22 = String(req2);
  String WindVaneDur = String(WindVaneDuration);
  req22.concat(WindVaneDur);
  
  req11.concat(req22);

  char req3[] = "&temp=";
  String req33 = String(req3);
  String temper = String(sensors.getTempCByIndex(0));
  req33.concat(temper);

  req11.concat(req33);
  
  char req4[] = "\"\"";
  String req44 = String(req4);
  req11.concat(req44);
  char finalreq[req11.length()];
  req11.toCharArray(finalreq, req11.length());

  //Now that we've build the URL of HTTP POST, we're ready to send it by 3G/4G

  sendGSM("AT+SAPBR=3,1,\"APN\",\"internet.myapn\"");
  delay(1000);
  sendGSM("AT+SAPBR=3,1,\"PHONENUM\",\"+7654321098\"");
  delay(1000);
  
  sendGSM("AT+SAPBR=1,1");
  sendGSM("AT+HTTPINIT");  
  sendGSM("AT+HTTPPARA=\"CID\",1");
  //Serial.println(finalreq);     //DEBUG complete String
  sendGSM(finalreq);
  sendGSM("AT+HTTPACTION=0");
  }    
  //Now wait that the HTTP Request is done.
  delay(30000);
}

//using time between anemometer pulses calculate frequency of anemometer
float getAnemometerFreq(float pTime) { 
  return (1/pTime); 
  }

//This is the interrupt service routine (ISR) for the anemometer input pin
//it is called whenever a falling edge is detected
void anemometerISR() {
  unsigned long cTime = millis(); //get current time
  if(!start) { //This is not the first pulse and we are not at 0 MPH so calculate time between pulses
    pulseTime = (float)(cTime - sTime)/1000;
    culPulseTime += pulseTime; //add up pulse time measurements for averaging
    avgWindCount++; //anemomter went around so record for calculating average wind speed
  }
  
  sTime = cTime; //store current time for next pulse time calculation
  start = false; //we have our starting point for a wind speed measurement
}

void parseATText(byte b) {

  buffer[pos++] = b;

  if ( pos >= sizeof(buffer) )
    resetBuffer(); // just to be safe

  switch (parseState) {
  case PS_DETECT_MSG_TYPE: 
    {
      if ( b == '\n' )
        resetBuffer();
      else {        
        if ( pos == 3 && strcmp(buffer, "AT+") == 0 ) {
          parseState = PS_IGNORING_COMMAND_ECHO;
        }
        else if ( b == ':' ) {
          if ( strcmp(buffer, "+HTTPACTION:") == 0 ) {
            Serial.println("Received HTTPACTION");
            parseState = PS_HTTPACTION_TYPE;
          }
          else if ( strcmp(buffer, "+HTTPREAD:") == 0 ) {
            Serial.println("Received HTTPREAD");            
            parseState = PS_HTTPREAD_LENGTH;
          }
          resetBuffer();
        }
      }
    }
    break;

  case PS_IGNORING_COMMAND_ECHO:
    {
      if ( b == '\n' ) {
        Serial.print("Ignoring echo: ");
        Serial.println(buffer);
        parseState = PS_DETECT_MSG_TYPE;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPACTION_TYPE:
    {
      if ( b == ',' ) {
        Serial.print("HTTPACTION type is ");
        Serial.println(buffer);
        parseState = PS_HTTPACTION_RESULT;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPACTION_RESULT:
    {
      if ( b == ',' ) {
        Serial.print("HTTPACTION result is ");
        Serial.println(buffer);
        parseState = PS_HTTPACTION_LENGTH;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPACTION_LENGTH:
    {
      if ( b == '\n' ) {
        Serial.print("HTTPACTION length is ");
        Serial.println(buffer);
        // now request content
        GSM.print("AT+HTTPREAD=0,");
        GSM.println(buffer);
        parseState = PS_DETECT_MSG_TYPE;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPREAD_LENGTH:
    {
      if ( b == '\n' ) {
        contentLength = atoi(buffer);
        Serial.print("HTTPREAD length is ");
        Serial.println(contentLength);
        Serial.print("HTTPREAD content: ");
        parseState = PS_HTTPREAD_CONTENT;
        resetBuffer();
      }
    }
    break;

  case PS_HTTPREAD_CONTENT:
    {
      // for this demo I'm just showing the content bytes in the serial monitor
      Serial.write(b);
      contentLength--;
      
      if ( contentLength <= 0 ) {
        // all content bytes have now been read
        parseState = PS_DETECT_MSG_TYPE;
        resetBuffer();
      }
    }
    break;
  }
}

