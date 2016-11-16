//****************************************************************************************
// Illutron take on Disney style capacitive touch sensor using only passives and Arduino
// Dzl 2012
//****************************************************************************************


//                              10n
// PIN 9 --[10k]-+-----10mH---+--||-- OBJECT
//               |            |
//              3.3k          |
//               |            V 1N4148 diode
//              GND           |
//                            |
//Analog 0 ---+------+--------+
//            |      |
//          100pf   1MOmhm
//            |      |
//           GND    GND



#define SET(x,y) (x |=(1<<y))                //-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))               // |
#define CHK(x,y) (x & (1<<y))                   // |
#define TOG(x,y) (x^=(1<<y))                    //-+



#define N 160  //How many frequencies


const int ledrouge  = 1;
const int ledorange = 3;


float results[N];            //-Filtered result buffer
float freq[N];            //-Filtered result buffer
int sizeOfArray = N;
const int analogPin = 0;
int val = 0;

  //musique
  int speakerPin = 4;

int length = 15; // the number of notes

//twinkle twinkle little star
char notes[] = "ccggaag ffeeddc ggffeed ggffeed ccggaag ffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

void playTone(int tone, int duration) {
 for (long i = 0; i < duration * 1000L; i += tone * 2) {
   digitalWrite(speakerPin, HIGH);
   delayMicroseconds(tone);
   digitalWrite(speakerPin, LOW);
   delayMicroseconds(tone);
 }
}

void playNote(char note, int duration) {
 char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
 int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
 
 // play the tone corresponding to the note name
 for (int i = 0; i < 8; i++) {
   if (names[i] == note) {
     playTone(tones[i], duration);
   }
 }
}

 

void setup()
{
 TCCR1A=0b10000010;        //-Set up frequency generator
 TCCR1B=0b00011001;        //-+
 ICR1=110;
 OCR1A=55;

 pinMode(9,OUTPUT);        //-Signal generator pin
 pinMode(8,OUTPUT);        //-Sync (test) pin
 pinMode(4,OUTPUT);
 pinMode(5,OUTPUT);

 Serial.begin(115200);

 for(int i=0;i<N;i++)      //-Preset results
   results[i]=0;         //-+


   //musique
    pinMode(speakerPin, OUTPUT);
}

void loop()
{
 
  val = analogRead(analogPin);
  Serial.println(val);

 if(val <25)
  {
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
   //musique
  for (int i = 0; i < length; i++) {
   if (notes[i] == ' ') {
     delay(beats[i] * tempo); // rest
   } else {
     playNote(notes[i], beats[i] * tempo);
   }
   
   // pause between notes
   delay(tempo / 2); 
 }
 
  }
  else
  {
    digitalWrite(5, HIGH);
    digitalWrite(4, LOW);
  }

 unsigned int d;

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
  
   freq[d] = d;
  
  
 }
PlottArray(1,freq,results); 

 TOG(PORTB,0);            //-Toggle pin 8 after each sweep (good for scope)

 
}
