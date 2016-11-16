const int LEDV1=5;
const int LEDV2=2;
const int LEDJ=3;
const int LEDR=4;


#define SET(x,y) (x |=(1<<y))				//-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))       		// |
#define CHK(x,y) (x & (1<<y))           		// |
#define TOG(x,y) (x^=(1<<y))            		//-+



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
  
  TCCR1A=0b10000010;        //-Set up frequency generator
  TCCR1B=0b00011001;        //-+
  ICR1=110;
  OCR1A=55;

  pinMode(9,OUTPUT);        //-Signal generator pin
  pinMode(8,OUTPUT);        //-Sync (test) pin

  Serial.begin(115200);


}

void loop()
{


  unsigned int d;
  
  float moyenne;
  
  moyenne = 0;

  int val = analogRead(0);    

    for(int i=1;i<161;i++){
      setFrequency(i * 100000);
      moyenne = moyenne + analogRead(A0);
    }
    
    moyenne = moyenne / 160;
    Serial.println (moyenne);
    
  if ( val > 230)
  {
    digitalWrite(LEDJ,HIGH);
  }
  else
  {
    if ( val > 190)
    {
    digitalWrite(LEDV1,HIGH);
    }
    else    
    {  
      if ( val > 150)
    {
      digitalWrite(LEDV1,HIGH);
    }
      else 
      {
        if ( val > 70)
        {digitalWrite(LEDR,HIGH);
        }
        }
    }
  }
                                

  


  int counter = 0;
  for(unsigned int d=0;d<N;d++)
  {
    int v=analogRead(0);    //-Read response signal
    CLR(TCCR1B,0);          //-Stop generator
    TCNT1=0;                //-Reload new frequency
    ICR1=d;                 // |
    OCR1A=d/2;              //-+
    SET(TCCR1B,0);          //-Restart generator

    results[d]=results[d]*0.5+(float)(v)*0.5; //Filter results
    Serial.println(v);
    
 //   plot(v,0);              //-Display
 //   plot(results[d],1);
  // delayMicroseconds(1);
  }


    //PlottArray(1,freq,results); 
 

  TOG(PORTB,0);            //-Toggle pin 8 after each sweep (good for scope)
}
   
