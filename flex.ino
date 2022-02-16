/******************************************************************************
Flex_Sensor_Example.ino
Example sketch for SparkFun's flex sensors
  (https://www.sparkfun.com/products/10264)
Jim Lindblom @ SparkFun Electronics
April 28, 2016

Create a voltage divider circuit combining a flex sensor with a 47k resistor.
- The resistor should connect from A0 to GND.
- The flex sensor should connect from A0 to 3.3V
As the resistance of the flex sensor increases (meaning it's being bent), the
voltage at A0 should decrease.

Development environment specifics:
Arduino 1.6.7
******************************************************************************/
const int FLEX_PIN = A0; // Pin connected to voltage divider output
const int FLEX_PIN1 = A1;
const int FLEX_PIN2 = A2;

const int anglrRLmin = 30;
const int anglrRLmax = 45;

const int anglrBmin = 0;
const int anglrBmax = 5;

// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
const float VCC = 5.03; // Measured voltage of Ardunio 5V line
const float R_DIV = 10000.0; // Measured resistance of 3.3k resistor

// Upload the code, then try to adjust these values to more
// accurately calculate bend degree.
const float STRAIGHT_RESISTANCE = 26500.0; // resistance when straight
const float BEND_RESISTANCE = 50000.0; // resistance at 90 deg

const float STRAIGHT_RESISTANCE_back = 11100.0; // resistance when straight
const float BEND_RESISTANCE_back = 20300.0;

const int buzzer = 9; //buzzer to arduino pin 9

#define RED_PIN   12
#define BLUE_PIN  8
#define GREEN_PIN 7
int t_time = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(FLEX_PIN, INPUT);
  pinMode(FLEX_PIN1, INPUT);
  pinMode(FLEX_PIN2, INPUT);
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output

  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  
}

void loop() 
{
  noTone(buzzer);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
  
  // Read the ADC, and calculate voltage and resistance from it
  // FLEX_PIN
  int flexADC = analogRead(FLEX_PIN);
  float flexV = flexADC * VCC / 1023.0;
  float flexR = R_DIV * (VCC / flexV - 1.0);
  
                   
 // FLEX_PIN1
  int flexADC1 = analogRead(FLEX_PIN1);
  float flexV1 = flexADC1 * VCC / 1023.0;
  float flexR1 = R_DIV * (VCC / flexV1 - 1.0);
  
  //FLEX_PIN2
  int flexADC2 = analogRead(FLEX_PIN2);
  float flexV2 = flexADC2 * VCC / 1023.0;
  float flexR2 = R_DIV * (VCC / flexV2 - 1.0);
  
   // Use the calculated resistance to estimate the sensor's
  // bend angle:                 
  float angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                   0, 90.0);
  float angle1 = map(flexR1, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                   0, 90.0);
  float angle_back = map(flexR2, STRAIGHT_RESISTANCE_back, BEND_RESISTANCE_back,
                    0, 90.0);
                   

  //print to monitor
  Serial.println("Inclination angles"); 
  Serial.println("Right\tLeft\tBack\tUnit");
  //Serial.println(String(flexR)+"\t"+String(flexR1)+"\t"+ String(flexR2) + "ohms"); 
  Serial.println(String(angle)+"\t"+String(angle1)+"\t"+ String(angle_back) + "degrees");
  Serial.println();

// Warning & Alarm according to inappropriate NECK POSITION

  if(angle <=anglrRLmin and angle1 <=anglrRLmin and angle_back <= anglrBmin){
     t_time = 0;
  }
  
  if( angle <=anglrRLmin and angle1 <=anglrRLmin or angle_back <= anglrBmin){
     digitalWrite(GREEN_PIN, HIGH);
     delay(1000);
     
  }
  
  if (angle > anglrRLmin or angle1 > anglrRLmin or angle_back > anglrBmin){
    
    if (t_time > 6)
      tone(buzzer, 500);
    
    
    if(angle > anglrRLmin and angle <=anglrRLmax or angle1 > anglrRLmin and angle1 <=anglrRLmax or angle_back > anglrBmin and angle_back <=anglrBmax){
       digitalWrite(BLUE_PIN, HIGH);
       digitalWrite(GREEN_PIN, LOW);
    }
    
    if(angle > anglrRLmax or angle1 > anglrRLmax or angle_back > anglrBmax){
       digitalWrite(RED_PIN, HIGH);
       digitalWrite(BLUE_PIN, LOW);
       digitalWrite(GREEN_PIN, LOW); 
    }
    t_time += 1;
  }
  delay(1000);
  Serial.println(t_time);
  Serial.println();
  

}
