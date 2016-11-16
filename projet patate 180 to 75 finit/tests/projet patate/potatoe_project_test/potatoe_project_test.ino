const int outPin = 9; // output pin
const float maxFreq = 16000000; //max freq supported by Arduino (16MHz)
const float frequency = 100000;

void setup()
{


  Serial.begin(115200); //for sending A0 pin values
  
  pinMode(outPin,OUTPUT);        //Signal generator pin
   
}

void loop()
{  
  int i;
  for (i=1; i<161; i++){
      setFrequency(frequency * i); 
  }
                                     
  Serial.println( analogRead(A0));

}

void setFrequency(float frequency)
{
  if(frequency >=0.24 || frequency <=16000000)
  {
    
    TCCR1A=0b10000010;        // Set up frequency generator, mode 14 (Fast PWM)
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

