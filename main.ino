
#include "ChainableLED.h"
#include "pitches.h"  //音符库
#define NUM_OF_LED  1
#define YELLOW_INTERVAL 200

ChainableLED leds(19, 18, NUM_OF_LED);

int sw1pin = 6; //黄灯闪烁开关
int sw2pin = 7; //这个是顺序模式开关
int sw3pin = 15;//报警音乐停止按钮
int buzzlepin=10;//音乐I/O输出口
int infraredpin=2;//红外接口
//休止符
#define X -1
//乐谱
int note[]={X,X,NOTE_E6,NOTE_D6,NOTE_G6,NOTE_G6,NOTE_A6,NOTE_A6,NOTE_B6,NOTE_G6,NOTE_D6,
NOTE_E6,NOTE_D6,NOTE_G6,NOTE_G6,NOTE_A6,NOTE_A6,NOTE_B6,NOTE_A6,NOTE_E6,NOTE_D6,NOTE_G6,
NOTE_G6,NOTE_A6,NOTE_A6,NOTE_B6,NOTE_G6,NOTE_D6,NOTE_E6,NOTE_D6,NOTE_G6,NOTE_G6,NOTE_A6,NOTE_G6};

int dura[]={2,2,1,1,2,2,2,2,2,2,2,1,1,2,2,2,2,2,4,1,1,2,2,2,2,2,2,2,1,1,2,2,2,8};

//音乐2
int tune_2[] = 
{
  NOTE_F4,NOTE_F4,NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F6,NOTE_F6,NOTE_F6,
  NOTE_F5,NOTE_F5,NOTE_F5,NOTE_F5,NOTE_FH1,NOTE_FH1,NOTE_FH1,NOTE_FH1,
  NOTE_FH2,NOTE_FH2,NOTE_FH2,NOTE_FH2,NOTE_FH2,NOTE_FH2,NOTE_FH2,NOTE_FH2,
  NOTE_F5,NOTE_F4,NOTE_F3,NOTE_F1,NOTE_F2,X,NOTE_F2,NOTE_F6,
  NOTE_F5,X,NOTE_F4,X,NOTE_F3,X,NOTE_F3,NOTE_F3,NOTE_F5,X,NOTE_F4,NOTE_F3,NOTE_F2,X,NOTE_F2,NOTE_FH4,
  NOTE_FH3, NOTE_FH4, NOTE_FH3, NOTE_FH4, NOTE_F2, X,NOTE_F2,NOTE_FH4, NOTE_FH3, NOTE_FH4, NOTE_FH3, NOTE_FH4, NOTE_F2, X,NOTE_F2,NOTE_F6,
  NOTE_F5,X,NOTE_F4,X,NOTE_F3,X,NOTE_F3,NOTE_F3,NOTE_F5,X,NOTE_F4,NOTE_F3,NOTE_F2,X,NOTE_F2,NOTE_FH4,
  NOTE_FH3, NOTE_FH4, NOTE_FH3, NOTE_FH4, NOTE_F2, X,NOTE_F2,NOTE_FH4,NOTE_FH3, NOTE_FH4, NOTE_FH3, NOTE_FH4};
float durt_2[]=                  
{
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5, 
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5
};

//音乐1的长度
int len=sizeof(note)/sizeof(int);
//音乐2的长度
int length=sizeof(tune_2)/sizeof(tune_2[0]);

void setup() {
  pinMode(sw1pin, INPUT);
  pinMode(sw2pin, INPUT);
  pinMode(sw3pin,INPUT);
  pinMode(infraredpin,INPUT);
  leds.init();
  leds.setColorRGB(0, 0, 0, 0);
  pinMode(buzzlepin,OUTPUT);
  for(int i=0;i<len;++i)
  {
    tone(buzzlepin,note[i]);
    delay(200*dura[i]);
  }
  noTone(buzzlepin);
}
//使能信号
int key1val = 0;
int key2val = 0;

//循环次数
int count1 = 0;
int count2 = 0;

void loop() {

  //使能信号切换
  if (!digitalRead(sw1pin)) {
    key1val = 1;
    if (key2val) {
      key2val = 0;
      count2 = 0;
    }
  }
  if (!digitalRead(sw2pin)) {
    key2val = 1;
    if (key1val) {
      key1val = 0;
      count1 = 0;
    }
  }
  //报警功能
  if(!digitalRead(infraredpin)){
    for(int x=0;x<length;x++)
  {
    tone(buzzlepin,tune_2[x]);
    //一次亮，一次灭
    if(x%2)
    leds.setColorRGB(0, 255, 0,0);  //红灯亮
    else
    leds.setColorRGB(0,0, 0, 0);  //灭
    if(!digitalRead(sw3pin))break;
    delay(500*durt_2[x]);  
  }
    noTone(buzzlepin);
    leds.setColorRGB(0,0,0,0);
    }
  if (key1val && !((count1 / YELLOW_INTERVAL) % 2))
    leds.setColorRGB(0, 255, 0, 180);  //黄灯亮
  if (key1val && ((count1 / YELLOW_INTERVAL) % 2))
    leds.setColorRGB(0, 0, 0, 0);  //灭
  if (key1val)
    ++count1;
  //正常切换
  if (key2val) {
    switch ((count2/500)%5) {
      case 0:
      case 1:
      leds.setColorRGB(0, 255, 0, 0); break;  //红灯
      case 2:
        leds.setColorRGB(0, 255, 0, 180); break;  //黄灯
      case 3:
      case 4:
        leds.setColorRGB(0, 0, 0, 255);break;  //绿灯
    }
    //循环次数增加
    ++count2;
  }
  


}

