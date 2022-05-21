#include <SPI.h>
#include <mcp2515.h>

float avg = 0;
float v1 = 0;

unsigned long c1 = 0;
unsigned long c2 = 0;
unsigned long c3 = 0;
unsigned long c4 = 0;

int counter = 0;
int flag = 0;
bool sensor = false;
int iter = 0;
int sum = 0;

struct can_frame canAuto;
MCP2515 mcp2515(10);

void setup() {
  pinMode(A0, INPUT);
  pinMode(3, OUTPUT);
  
  Serial.begin(9600);
  
  canAuto.can_id  = 0x0AA;
  canAuto.can_dlc = 8;
  canAuto.data[0] = 0xAA;
  canAuto.data[1] = 0x00;
  canAuto.data[2] = 0x00;
  canAuto.data[3] = 0x00;
  canAuto.data[4] = 0x00;
  canAuto.data[5] = 0x00;
  canAuto.data[6] = 0x00;
  canAuto.data[7] = 0x00;
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
  v1 = analogRead(A0);
  avg = v1;
}

void loop() {
  v1 = analogRead(A0);

  if (v1 > 1.2 * avg) {
    v1 = 1.2 * avg;
  }
  if (v1 < 0.8 * avg) {
    v1 = 0.8 * avg;
  }
  iter++;
  avg = ((avg * iter) + v1) / (iter + 1);


  if (iter == 101) {
    iter = 0;
  }

  if ((v1 > 1.05 * avg) || (v1 < 0.95 * avg)) {
    if (sensor == false) {

      counter = (counter + 1) * flag;
      if (flag == 0) {
        c2 = millis();
        flag = 1;
        counter++ ;
      }
    }
  }
  if (flag == 1) {
    c1 = millis();
  }

  if (c1 - c2 > 3000) {
    flag = 0;
    c1 = 0;
    c2 = 0;
    if (counter > 5 && counter < 50) {
      sensor = true;
      c3 = millis();
    }
    counter = 0;
  }


  if (sensor == true) {
    c4 = millis();
    digitalWrite(3, HIGH);
    mcp2515.sendMessage(&canAuto);
    if (c4 - c3 > 10000) {
      digitalWrite(3, LOW);
      c3 = 0;
      c4 = 0;
      sensor = false;
    }
  }

  Serial.print(v1);
  Serial.print(" ");
  Serial.print(avg);
  Serial.print(" ");
  Serial.print(1.05*avg);
  Serial.print(" ");
  Serial.println(0.95*avg);

  delay(100);

}
