#include <avr/eeprom.h>

bool lastStatus = false;
long lastWriteTime = 0;
long num = 0;
#define SEG_A 2
#define SEG_B 3
#define SEG_C 4
#define SEG_D 5
#define SEG_E 6
#define SEG_F 7
#define SEG_G 8
#define SEG_H 9

#define COM1 10
#define COM2 11
#define COM3 12
#define COM4 13

#define M_OR_K 15

unsigned char table[10][8] =
{
  {0, 0, 1, 1, 1, 1, 1, 1}, //0
  {0, 0, 0, 0, 0, 1, 1, 0}, //1
  {0, 1, 0, 1, 1, 0, 1, 1}, //2
  {0, 1, 0, 0, 1, 1, 1, 1}, //3
  {0, 1, 1, 0, 0, 1, 1, 0}, //4
  {0, 1, 1, 0, 1, 1, 0, 1}, //5
  {0, 1, 1, 1, 1, 1, 0, 1}, //6
  {0, 0, 0, 0, 0, 1, 1, 1}, //7
  {0, 1, 1, 1, 1, 1, 1, 1}, //8
  {0, 1, 1, 0, 1, 1, 1, 1} //9
};

void setup()
{
  Serial.begin (9600);

  pinMode(SEG_A, OUTPUT); //设置为输出引脚
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);
  pinMode(SEG_H, OUTPUT);
  pinMode(COM1, OUTPUT);
  pinMode(COM2, OUTPUT);
  pinMode(COM3, OUTPUT);
  pinMode(COM4, OUTPUT);

  eeprom_read_block(&num, 0, 4);
}

void loop()
{
  bool nowStatus = (analogRead(0) > 100);
  long nowTime = millis();
  if (nowTime - lastWriteTime > 60000)
  {
    eeprom_write_block(&num, 0, 4);
    lastWriteTime =  nowTime;
  }

  if (nowStatus ^ lastStatus) {
    lastStatus = nowStatus;
    num ++;
  }
  long n = num / 23 * 44 / 100;

  if (digitalRead(M_OR_K) == HIGH)
    n = n / 1000;

  Display(1, n % 10);
  Display(2, (n % 100) / 10);
  Display(3, (n % 1000) / 100);
  Display(4, (n % 10000) / 1000);
  if (Serial.read() == '\n')
    Serial.println(n);
}



void Display(unsigned char com, unsigned char num)
{
  digitalWrite(SEG_A, LOW); //去除余晖
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
  digitalWrite(SEG_H, LOW);
  switch (com) //选通位选
  {
    case 1:
      digitalWrite(COM1, LOW); //选择位 1
      digitalWrite(COM2, HIGH);
      digitalWrite(COM3, HIGH);
      digitalWrite(COM4, HIGH);
      break;
    case 2:
      digitalWrite(COM1, HIGH);
      digitalWrite(COM2, LOW); //选择位 2
      digitalWrite(COM3, HIGH);
      digitalWrite(COM4, HIGH);
      break;
    case 3:
      digitalWrite(COM1, HIGH);
      digitalWrite(COM2, HIGH);
      digitalWrite(COM3, LOW); //选择位 3
      digitalWrite(COM4, HIGH);
      break;
    case 4:
      digitalWrite(COM1, HIGH);
      digitalWrite(COM2, HIGH);
      digitalWrite(COM3, HIGH);
      digitalWrite(COM4, LOW); //选择位 4
      break;
    default: break;
  }
  digitalWrite(SEG_A, table[num][7]); //a 查询码值表
  digitalWrite(SEG_B, table[num][6]);
  digitalWrite(SEG_C, table[num][5]);
  digitalWrite(SEG_D, table[num][4]);
  digitalWrite(SEG_E, table[num][3]);
  digitalWrite(SEG_F, table[num][2]);
  digitalWrite(SEG_G, table[num][1]);
  digitalWrite(SEG_H, table[num][0]);
}
