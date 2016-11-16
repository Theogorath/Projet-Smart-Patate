const int LEDV1=5;
const int LEDV2=2;
const int LEDJ=3;
const int LEDR=4;
const float maxFreq = 16000000; //max freq supported by Arduino (16MHz)

#define SET(x,y) (x |=(1<<y))        //-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))           // |
#define CHK(x,y) (x & (1<<y))               // |
#define TOG(x,y) (x^=(1<<y))                //-+



#define N 160  //How many frequencies

float results[N];            //-Filtered result buffer
float freq[N];            //-Filtered result buffer
int sizeOfArray = N;

                    
void setup()
{
                                                       
  pinMode(LEDV1, OUTPUT);
  pinMode(LEDV2, OUTPUT);
  pinMode(LEDJ, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(A0, INPUT);
  
  TCCR1A=0b10000010;        //-Set up frequency generator
  TCCR1B=0b00011001;        //-+
  ICR1=110;
  OCR1A=55;

  pinMode(9,OUTPUT);        //-Signal generator pin
  pinMode(8,OUTPUT);        //-Sync (test) pin

  Serial.begin(115200);
}
void setFrequency(float frequency)
{
 
  if(frequency >=0.24 || frequency <=8000000)
  {
    
    TCCR1A=0b10000010;        // Set up frequency generator, mode 14 (Fast PWM)
    //WGM11 = 1, COM1A1 = 1
    //Fast PWM: TOP=ICR1, update of OCR1x=BOTTOM, TOV1 Flag Set on TOP
    
  
    unsigned int v=0;
    int indscale=0;
    float prescale[] = {1.0,8.0,64.0,256.0,1024.0};
    float period=1/frequency;
    
    while(v==0)
    {
      float curfreq=maxFreq/prescale[indscale];
      float tickperiod=1/curfreq;
      float nbtick=period/tickperiod;
     
      if(nbtick>65535)
      {
        indscale=indscale+1;
      }
      else
      {
        v=nbtick;
      }

    }
    
    int c=prescale[indscale];
    
     switch (c) {
      
         case 1: TCCR1B=0b00011001; break;
         case 8: TCCR1B=0b00011010; break;
         case 64: TCCR1B=0b00011011; break;
         case 256: TCCR1B=0b00011100; break;
         case 1024: TCCR1B=0b00011101; break;
         default: TCCR1B=0b00011000;
     }
    
  
    ICR1=v; //pulse duration = ICR1 value x time per counter tick
      
    OCR1A=v/2; //Output Compare Register //low state

  }
  
  
}
void loop()
{


  unsigned int d;
  
  int moyenne;
  
  moyenne = 0;

  float val = analogRead(0);    

    for(int i=1;i<161;i++){
      setFrequency(350000);
      moyenne = moyenne + analogRead(A0);
    }

    moyenne = moyenne / 160;
    Serial.println (moyenne);
    
    
    
  if ( moyenne > 118)
  {
    digitalWrite(LEDJ,HIGH);
    digitalWrite(LEDV1,LOW);
    digitalWrite(LEDV2,LOW);
       delay(100);
    digitalWrite(LEDR,LOW);

  }
  else
  {
    if ( moyenne > 86)
    {
    digitalWrite(LEDV1,HIGH);
    digitalWrite(LEDV2,LOW);
    digitalWrite(LEDJ,LOW);
       delay(100);
    digitalWrite(LEDR,LOW);
 
         
    }
    else    
    {  
      if ( moyenne >= 83)
    {
      digitalWrite(LEDV2,HIGH);
      digitalWrite(LEDV1,LOW);
      digitalWrite(LEDJ,LOW);
         delay(100);
      digitalWrite(LEDR,LOW);

    }
      else 
      {
        if ( moyenne > 81)
        {digitalWrite(LEDR,HIGH);
         digitalWrite(LEDV1,LOW);
         digitalWrite(LEDV2,LOW);
            delay(100);
         digitalWrite(LEDJ,LOW);

        }
        }
    }
  }
           
}
   
