#include <Adafruit_MLX90614.h>
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#define echoPin 2 // pin echo
#define trigPin 3 // pin trig

int irsensor = 6;
int valueirsensor = 0;

int state = 0;
int state2 = 0;

Servo myservo;
Servo myservo2;

int temp = 0;
double tempDisplay = 0;

long duration;
int distance;

void setup() {
  Serial.begin(9600);
  myservo.attach(9);
  myservo2.attach(10);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irsensor, INPUT);

  lcd.begin(); //kalu error tukar lcd.init();
  lcd.backlight();

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };

  Serial.print("Emissivity = "); 
  Serial.println(mlx.readEmissivity());

  myservo.write(0);
  myservo2.write(0);

}

void loop() {
  Serial.print ("Temp = ");
  Serial.println(mlx.readObjectTempC());
  valueirsensor = digitalRead(irsensor);
  ultrasonic();

  temp = mlx.readObjectTempC();
  tempDisplay = mlx.readObjectTempC();

  Serial.println(tempDisplay);

  Lcd();
  delay(500);

  if (valueirsensor == LOW){
    state = 1;
    state2 = 1;
    }
  else{
    state = 0;
    state2 = 0;
    }

  // utk temp
  if (temp <= 37 && state == 1){
    myservo.write(90);
    lcd.setCursor(4, 0);
    lcd.print("       ");
    lcd.setCursor(4, 0);
    lcd.print("OPEN");
    delay(5000); // 5 sec
    myservo.write(0);
    }
  if (temp >= 37 && state == 1) {
    lcd.setCursor(4, 0);
    lcd.print("       ");
    lcd.setCursor(4, 0);
    lcd.print("CLOSE");
    myservo.write(0);
    delay(1000);
    }

  // utk ultrasonic
  if (distance <= 7){
    myservo2.write(90);
    delay(1000);
    myservo2.write(0);
    }

}

void ultrasonic(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  }

void Lcd(){
  lcd.setCursor(4, 0);
  lcd.print("       ");
  lcd.setCursor(4, 0);
  lcd.print("WELCOME");
  if (state2 == 1){
    lcd.setCursor(1, 1);
    lcd.print("Temp = ");
    lcd.setCursor(8, 1);
    lcd.print(tempDisplay, 1);
    }
  else {
    delay(2000);
    lcd.setCursor(1, 1);
    lcd.print("          ");
    }
  
  }
