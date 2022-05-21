#include <SPI.h>
#include <mcp2515.h>
#include <Servo.h>

struct can_frame canMsg;
MCP2515 mcp2515(10);
Servo sr;

int pos = 0;
bool cw = true;
int val = 0x00;

void setup() {
  sr.attach(5);
  Serial.begin(9600);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();




}
void loop() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
    val = canMsg.data[0];
  }
  else {
    val = 0x00;
  }
  if (val == 0x00 && pos > 0) {
    pos = pos - 20;
    sr.write(pos);
  }

  if (val == 0x99) {
    if (cw == true) {
      pos = pos + 10;
    }
    else {
      pos = pos - 10;
    }

    sr.write(pos);
  }
  if (val == 0xAA) {
    if (cw == true) {
      pos = pos + 20;
    }
    else {
      pos = pos - 20;
    }

    sr.write(pos);
  }
  if (val == 0xBB) {
    if (cw == true) {
      pos = pos + 30;
    }
    else {
      pos = pos - 30;
    }

    sr.write(pos);
  }
  if (pos >= 180) {
    cw = false;
  }
  if (pos <= 0) {
    cw = true;
  }
  Serial.println(val);
  delay(100);
















}
