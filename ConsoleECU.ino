
#include <SPI.h>
#include <mcp2515.h>
const int L_switch = 8;
const int M_switch = 3;
const int H_switch = 4;

const int L_lamp = 5;
const int M_lamp = 6;
const int H_lamp = 7;

int L_state = 0;
int M_state = 0;
int H_state = 0;

int L_out = 0;
int M_out = 0;
int H_out = 0;

int state = 0;
int flag = 0;

struct can_frame canMan;
struct can_frame canAuto;
MCP2515 mcp2515(10);

void setup() {
  Serial.begin(9600);
  pinMode(L_switch, INPUT);
  pinMode(M_switch, INPUT);
  pinMode(H_switch, INPUT);

  pinMode(L_lamp, OUTPUT);
  pinMode(M_lamp, OUTPUT);
  pinMode(H_lamp, OUTPUT);

  canMan.can_id  = 0x09A;
  canMan.can_dlc = 8;
  canMan.data[0] = 0x99;
  canMan.data[1] = 0x00;
  canMan.data[2] = 0x00;
  canMan.data[3] = 0x00;
  canMan.data[4] = 0x00;
  canMan.data[5] = 0x00;
  canMan.data[6] = 0x00;
  canMan.data[7] = 0x00;

  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  mcp2515.setNormalMode();
}

void loop() {

  L_state = digitalRead(L_switch);
  M_state = digitalRead(M_switch);
  H_state = digitalRead(H_switch);

  state = (L_state || M_state || H_state);
  if (state == LOW) {
    flag = LOW;
  }

  if ((state == !flag) && (state == HIGH)) {
    L_out = (!(L_out)) && L_state;
    M_out = (!(M_out)) && M_state;
    H_out = (!(H_out)) && H_state;
    flag = HIGH;
  }

  digitalWrite(L_lamp, L_out);
  digitalWrite(M_lamp, M_out);
  digitalWrite(H_lamp, H_out);

  if (L_out == 1) {
    canMan.data[0] = 0x99;
    mcp2515.sendMessage(&canMan);
  }

  if (M_out == 1) {
    canMan.data[0] = 0xAA;
    mcp2515.sendMessage(&canMan);
  }

  if (H_out == 1) {
    canMan.data[0] = 0xBB;
    mcp2515.sendMessage(&canMan);
  }

  delay(100);
}
