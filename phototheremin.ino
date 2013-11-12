/*
 * Title: AnalogRead Phototheremin
 *
 * Author: Michael Vartan
 * E-mail: admin@mvartan.com
 *
 */

const int DARK = 80; //value of the analog in at its darkest
const int LIGHT = 300; //what is the value of the analog in at ambient light? (check the serial monitor)
const int NUM_AVG=1000; //number of times to read the pin to average out
const int BUTTON = 7;
const int PIEZO = 8;
const int PHOTOSENSOR = 0;
const int A=0, AS=1, B=2, C=3, CS=4, D=5, DS=6, E=7, F=8, FS=9, G=10, GS=11;

//Below: Only leave one uncommented, which scale you want it to play.
//const int SCALE[] = {A,AS,B,C,CS,D,DS,E,F,FS,G,GS}; //all notes
//const int SCALE[] = {A,C,D,E,G}; //pentatonic
//const int SCALE[] = {C,D,DS,E,G,A};  //blues major
const int SCALE[] = {C,DS,F,FS,G,AS}; //blues minor
//const int SCALE[] = {E,G,A,B,D}; //pentatonic a minor

void setup() {
  pinMode(BUTTON, INPUT); //set up our push button
  Serial.begin(9600); //uncomment if you want to use the serial port to debug DARK/LIGHT constants
}  

void loop() {
  //Begin averaging out reads to get an accurate result
  long total=0;
  for(int i=0;i<NUM_AVG;i++)
    total += analogRead(PHOTOSENSOR);
  int avg=(total/NUM_AVG);
  //end averaging out reads.
  
  Serial.println(avg); //uncomment if you want to use the serial port to debug DARK/LIGHT constants
  
  //begin mapping the number to a note on the chosen scale
  int val = map(avg, DARK, LIGHT, -12, 13);  //converts the value. -12 is an octave below the middle, 12 is an octave above.
  int note = val%12; //converts notes from A440 Pitch Standard to just note.
  int closest = 0;
  for(int i=1;i<sizeof(SCALE);i++) { //finds the closest note in the scale to the mapped note.
    if(SCALE[i]==note) {            // (if this loop were not here, it would just play all notes instead of scaled notes)
      closest = i;
      break;
    }
    if(abs(note-SCALE[i])<abs(note-SCALE[closest]))
      closest = i;
  }
  note=closest;
  //end mapping
  
  if(digitalRead(BUTTON)==0){
    tone(BUZZER, pow(2,((double)val)/12)*440); //plays the note according to the position of the piezo element.
  } else {
    noTone(BUZZER); //turn off the piezo element 
  }
}  
