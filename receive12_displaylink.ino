
// 7seg CLK  ->  A1 (15)
// 7seg DIO  ->  A0 (14)
// 7seg CLK  ->  D7 (7)
// 7seg DIO  ->  D6 (6)

// can ユーザー定義　500kbit/s　OBD　OFF
// ch 1  transmit displaylink ID:1000 200Hz

#include <mcp_can.h>            // can
#include <SPI.h>                // can
#include <TM1637.h>             // 7seg

#define CLK 15                  // A1pinをCLK信号に設定
#define DIO 14                  // A0をDIO信号に設定
TM1637 disp(CLK,DIO);
TM1637 disp2(7,6);              // CLK D7とDIO D6に設定

#define CS_PIN  (10)            // CSを10ピンとする can
#define INT_PIN (9)             // INTを9ピンとする can
MCP_CAN CAN0(CS_PIN);           // can

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];

void setup(){

  disp.set(7);                // 7segの明るさを0から7で指定
  disp.init(D4056A);
  disp2.set(7);               // 7segの明るさを0から7で指定
  disp2.init(D4056A);
delay(1500);

  disp.display(-1);           // 7segにも値を表示
  disp2.display(-2);          // 7segにも値を表示

  Serial.begin(115200);
  CAN0.begin(CAN_500KBPS);    // CANの通信速度を500kbpsにする
  pinMode(INT_PIN, INPUT);    // 割り込みのためのピンを設定
}

void loop()
{
    CAN0.readMsgBuf(&len, rxBuf);   rxId = CAN0.getCanId();
    if (rxId == 0x3E8) {                //もしID1000からのデータだったら

// Water temp
  if (rxBuf[0] == 0x02) {disp2.display(rxBuf[6] - 50);}

// Air/Fuel
  if (rxBuf[0] == 0x06) {disp.display((rxBuf[4]+rxBuf[5]*256)*0.0147);}

// Boost　(kpa)
// if (rxBuf[0] == 0x00) {disp.display(rxBuf[6] - 100);}
}
delay(1);
}
