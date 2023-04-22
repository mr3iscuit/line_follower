#define led_buzzer 13
#define button 7 // button
#define ir_pin 2 // ir receiver 
#define dip1 4 // mod anahtarı 1 - mod anahtarlarının bağlı olduğu pin numarasını tanımlıyoruz
#define dip2 5 // mod anahtarı 2 - mod anahtarlarının bağlı olduğu pin numarasını tanımlıyoruz
#define dip3 6  // mod anahtarı 3 - mod anahtarlarının bağlı olduğu pin numarasını tanımlıyoruz
#define left_motor1 3 // sol motor hız1 -  sol motoru kontrol etmek için gerekli pinler
#define left_motor2 11  // sol motor hız2 - bu iki pinden birine analogwrite ile çıkış yaparsanız motor o yönde döner . 2 pin aynı anda 0 yada 1 olursa motor durur.
#define right_motor1 9  // sag motor hız1 -  sag motoru kontrol etmek için gerekli pinler
#define right_motor2 10 // sag motor hız2 - bu iki pinden birine analogwrite ile çıkış yaparsanız motor o yönde döner . 2 pin aynı anda 0 yada 1 olursa motor durur.
#define abs(x) (x>0?x:-x)

namespace utils {
  struct Edge {
    // straight line as a Edge of graph model
    int weight;
    int connections[6] = {-1};
  };

  struct DistSensModel {
    // 4k distance sensor value model
    int s1 = 0;
    int s2 = 0;
    int s3 = 0;
    int s4 = 0;
  };

  Edge edges[100];
};

void right() {
  analogWrite(left_motor1, 1023);
  analogWrite(left_motor2, 0);
  analogWrite(right_motor1, 0);
  analogWrite(right_motor2, 1023);
}

void left() {
  analogWrite(left_motor1, 0);
  analogWrite(left_motor2, 1023);
  analogWrite(right_motor1, 1023);
  analogWrite(right_motor2, 0);
}

void forward() {
  analogWrite(left_motor1, 1023);
  analogWrite(left_motor2, 0);
  analogWrite(right_motor1, 1023);
  analogWrite(right_motor2, 0);
}

void backward() {
  analogWrite(left_motor1, 0);
  analogWrite(left_motor2, 1023);
  analogWrite(right_motor1, 0);
  analogWrite(right_motor2, 1023);
}

void stop() {
  analogWrite(left_motor1, 0);
  analogWrite(left_motor2, 0);
  analogWrite(right_motor1, 0);
  analogWrite(right_motor2, 0);
}

void printDistValues(utils::DistSensModel values) {
  Serial.print(values.s1);
  Serial.print(" ");
  Serial.print(values.s2);
  Serial.print(" ");
  Serial.print(values.s3);
  Serial.print(" ");
  Serial.print(values.s4);
  Serial.print(" ");
  Serial.println();
  Serial.println();
}

utils::DistSensModel getDisValues() 
{
  utils::DistSensModel values;
  values.s1 = analogRead(A3);
  values.s2 = analogRead(A2);
  values.s3 = analogRead(A1);
  values.s4 = analogRead(A0);
  return values;
}

void setup() {
  Serial.begin(9600);
  
  pinMode(button, INPUT_PULLUP);
  pinMode(ir_pin, INPUT_PULLUP);

  pinMode(dip1, INPUT_PULLUP);
  pinMode(dip2, INPUT_PULLUP);
  pinMode(dip3, INPUT_PULLUP);

  pinMode(led_buzzer, OUTPUT);
  
  pinMode(left_motor1, OUTPUT);
  pinMode(left_motor2, OUTPUT);
  pinMode(right_motor1, OUTPUT);
  pinMode(right_motor2, OUTPUT);

  tone(led_buzzer, 500, 100);
  delay(50);
  noTone(led_buzzer);
/*
  left();
  delay(1000);
  right();
  delay(1000);
  forward();
  delay(1000);
  stop();
*/
}

utils::DistSensModel dsv;

void loop() {  
  while(true) {
    dsv = getDisValues();
    printDistValues(dsv);
    if(dsv.s1 > 200) {
      left();
      continue;
    }
    if(dsv.s4 > 200) {
      right();
      continue;
    }
    
    if(dsv.s2 > 200 || dsv.s3 > 200) {
      forward();
      continue;      
    }
    if(dsv.s1 < 200 && dsv.s2 < 200 && 
        dsv.s3 < 200 && dsv.s3 < 200) 
    {
      //backward();
      //continue;      
    }
    if(dsv.s1 > 200 && dsv.s2 > 200 && 
        dsv.s3 > 200 && dsv.s3 > 200) 
    {
      stop();
      continue;
    }
    
  }
  
}



