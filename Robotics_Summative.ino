//Mehadi Chowdhury
//Ateeb Jawaid
//January 24 2018

#include <Servo.h>
#include <Ultrasonic.h>
#include <AFMotor.h>
#define sensor A10 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)

int speakerPin =40;
int buttonPin1= 30;
int state1= LOW;

long lastDebounceTime= 0;
long DebounceDelay= 50;

Servo myservo, claw;
AF_DCMotor rightmotor(1), leftmotor(2);
//const int ultrasonicPin=9;

int length = 5; // the number of notes
int tempo = 300;
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };

int l= 0;

//pin 52

Ultrasonic ultrasonic(22, 24); //Init an Ultrasonic object
int distance;
int val;
int dur= 0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin (9600);
  myservo.attach(10);//working
  claw.attach(9);

  leftmotor.run(RELEASE); //make the left motor go forward
  rightmotor.run(RELEASE); //make the right motor go forward
  rightmotor.setSpeed(255);
  leftmotor.setSpeed(245);

  delay(500); //initial state of the car

  //setup buttonPin1, and speakerPin
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(speakerPin, OUTPUT);

  //setup servos
  myservo.write(90);
  claw.write(0);
}

void loop()
{
  int reading= digitalRead(buttonPin1);

if( (millis() - lastDebounceTime) > DebounceDelay )
  {
    Serial.println("Pressed");
    if( reading == HIGH & l == 0)
    {
      Serial.println("First");
     for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }
     }

    rturn();

    backward();
    delay(2000);

    stops();
    stops();
    stops();
    stops();

    
  l++;
  lastDebounceTime= millis();
  reading= LOW;
  }
  else if (reading == HIGH && l == 1 )
    {

    do
    {
          Serial.println("Second");
    
    //forward
    forward();
    
    dur +=100;
    
    //calculates distance
      val = analogRead(sensor);// value from sensor 
      distance = (6762/(val-9))-4;//13*pow(volts, -1); // worked out from datasheet graph
      delay(100); // slow down serial port 
      Serial.println(distance);   // print the distance
    
    //determines distance is less than 40
       if( distance < 6 )//20
       {
    
        stops();

    //plays beat
        for (int i = 0; i < length; i++) {
        if (notes[i] == ' ') {
          delay(beats[i] * tempo); // rest
        } else {
          playNote(notes[i], beats[i] * tempo);
        }
          }
      
         escape();//escape
        
      }
    }
    while( distance >= 6 );//loops while distance is less than 6
      lastDebounceTime= millis();
      l--;
      reading= LOW;
    }
}
}

void playTone(int tone, int duration) {//play tone
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

  void playNote(char note, int duration) {//play notes
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

  

void escape()//escape
{
  //stops
  stops();

  
  //open claw
  claw.write(0);
  delay(1500);

  //forward
  forward();
  delay(100);

  stops();//stops car

  //close claw
  claw.write(90);
  delay(1000);

  //move backwards
  backward();
  delay(1000);

  //stops
  stops();
  delay(500);

  //180 degree turn
  rturn();
  delay(1200);

  //stops
  stops();
  delay(500);

  
//calculate distance
  val = analogRead(sensor);// value from sensor 
  distance = (6762/(val-9))-4;//13*pow(volts, -1); // worked out from datasheet graph
  delay(500); // slow down serial port 
  Serial.println(distance);   // print the distance


  //forward
  forward();
  delay(dur);

  //stops
  stops();
  delay(500);

  //backward
  backward();
  delay(500);

  //stops
  stops();
  delay(500);

//forward
forward();
delay(dur);

//stops
stops();

  //open claw
  claw.write(0);
  delay(1500);

  //close claw
  claw.write(90);
  delay(1000);

//backwards
  backward();
  delay(500);

//play beat
  for (int i = 0; i < length; i++) {
    if (notes[i] == ' ') {
      delay(beats[i] * tempo); // rest
    } else {
      playNote(notes[i], beats[i] * tempo);
    }
      }

  stops();//stops

  //180 degree turn
  rturn();
  rturn();

  //stops
  stops();
  delay(1000);
}

void forward()//forward
{
  leftmotor.run(FORWARD); //make the left motor go forward
  rightmotor.run(FORWARD); //make the right motor go forward
  leftmotor.setSpeed(248); //set the speed of the left motor
  rightmotor.setSpeed(211); //set the speed of the right motor
}

void backward()//backward
{
  leftmotor.run(BACKWARD); //make the left motor go forward
  rightmotor.run(BACKWARD); //make the right motor go forward
  leftmotor.setSpeed(250); //set the speed of the left motor
  rightmotor.setSpeed(215); //set the speed of the right motor
}

void rturn()//right turn
{
  //wheel is stationary
  rightmotor.run(RELEASE);
  //set wheels forward
  leftmotor.run(FORWARD);
  //set speed
  leftmotor.setSpeed(230);
  delay(1800);
}

void lturn()//left turn
{
 //wheel is stationary
  leftmotor.run(RELEASE);
  //set wheels forward
  rightmotor.run(FORWARD);
  //set speed
  rightmotor.setSpeed(200);
  delay(1100);
}

void stops()//stops
{
  leftmotor.run(RELEASE); //make the left motor go forward
  rightmotor.run(RELEASE); //make the right motor go forward
  delay(500); //initial state of the car
}
