#define BLYNK_TEMPLATE_ID "TMPL6O4VHSCXG"
#define BLYNK_TEMPLATE_NAME "LED"
#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial
#define APP_DEBUG
#include "BlynkEdgent.h"
#include <TimeLib.h>
const int LED1 = 2;
const int LED2 = 4;
const int LED3 = 15;
const int ldr1 = 34;
const int ldr2 = 36;
const int ldr3 = 39;
const int bu1 = 26;








BlynkTimer timer;

bool led_set[3];/*bool led_set[3] คือการประกาศตัวแปรชนิดข้อมูล boolean 
ชื่อ led_set ที่มีขนาดเก็บข้อมูล 3 ช่อง (หรือ 3 ค่า) 
โดยแต่ละช่องเก็บข้อมูลสามารถเป็นได้เพียง 2 ค่าเท่านั้นคือ true หรือ false 
ซึ่งมีความหมายว่าตัวแปร led_set นี้จะใช้เก็บสถานะของ 3 ไฟ LED 
หรืออุปกรณ์ควบคุมการทำงานอื่นๆ โดย true หมายถึงเปิด LED หรือเปิดการทำงาน 
และ false หมายถึงปิด LED หรือหยุดการทำงาน*/
long timer_start_set[3] = {0xFFFF, 0xFFFF, 0xFFFF};
long timer_stop_set[3] = {0xFFFF, 0xFFFF, 0xFFFF};

/*เป็นการประกาศตัวแปรชนิดข้อมูล long ชื่อ timer_start_set และ timer_stop_set
 โดยตัวแปรทั้งสองมีขนาดเก็บข้อมูล 3 ช่อง โดยแต่ละช่องมีค่าเริ่มต้นเป็น 0xFFFF
 ซึ่งเป็นค่าที่มากที่สุดที่ตัวแปรชนิด long สามารถเก็บได้ (unsigned long จะเก็บได้มากกว่า)
ตัวแปร timer_start_set
 จะใช้สำหรับเก็บเวลาที่เริ่มต้นของการนับเวลาในการทำงานของอุปกรณ์หรือโปรแกรม
  ส่วนตัวแปร timer_stop_set จะใช้สำหรับเก็บเวลาที่สิ้นสุดการนับเวลา 
  โดยสามารถนำเวลาที่เริ่มต้นและเวลาสิ้นสุดนี้มาคำนวณหาเวลาที่ใช้ในการทำงานของอุปกรณ์หรือโปรแกรมได้*/
unsigned char weekday_set[3];
/*unsigned char weekday_set[3] เป็นการประกาศตัวแปรชนิดข้อมูล unsigned char ชื่อ weekday_set
โดยมีขนาดเก็บข้อมูล 3 ช่อง เป็นตัวแปรที่ใช้เก็บค่าวันในสัปดาห์ 
เช่น 1 ถึง 7 โดยที่ 1 แทนวันจันทร์, 2 แทนวันอังคาร, 3 แทนวันพุธ ฯลฯ จนถึง 7 แทนวันอาทิตย์*/
long rtc_sec;/*
long rtc_sec เป็นตัวแปรชนิดข้อมูล long ซึ่งใช้เก็บค่าเวลา (วินาที) ที่ได้มาจาก RTC (Real Time Clock) หรือนาฬิกา
แสดงเวลาจริง โดยค่าของตัวแปรนี้จะเป็นเวลาที่ผ่านไปตั้งแต่วันที่ 1 มกราคม ค.ศ. 1970 จนถึงปัจจุบัน*/
unsigned char day_of_week;
/*unsigned char day_of_week เป็นตัวแปรชนิดข้อมูล unsigned char
 ที่ใช้เก็บข้อมูลวันในสัปดาห์ 
 เช่น 1 ถึง 7 โดยที่ 1 แทนวันจันทร์, 2 แทนวันอังคาร, 3 แทนวันพุธ ฯลฯ 
 จนถึง 7 แทนวันอาทิตย์ ตัวแปรนี้สามารถใช้กับ RTC เพื่อเช็คว่าเป็นวันที่เท่าไหร่ในสัปดาห์ 
 ซึ่งจะมีค่าอยู่ระหว่าง 1 ถึง 7 โดยที่ค่า 1 จะแทนวันจันทร์ ค่า 2 จะแทนวันอังคาร ฯลฯ*/
bool led_status[3];
/*bool led_status[3] เป็นตัวแปรชนิดข้อมูล boolean
 ที่ใช้เก็บสถานะ (เปิดหรือปิด) ของ LED 3 ตัว 
 โดยตัวแปรนี้จะมีค่าเป็น true หาก LED ตัวที่ i เปิดอยู่ และ false หาก LED ตัวที่ i ปิดอยู่*/
bool update_blynk_status[3];
/*bool update_blynk_status[3] เป็นตัวแปรชนิดข้อมูล boolean 
ที่ใช้เก็บสถานะการอัพเดทของแอปพลิเคชัน Blynk สำหรับ LED 3 ตัว โดยตัวแปรนี้จะมีค่าเป็น true 
เมื่อสถานะของ LED ตัวที่ i มีการเปลี่ยนแปลงและส่งค่าให้แอปพลิเคชัน Blynk แล้ว
 และ false เมื่อสถานะของ LED ตัวที่ i ไม่มีการเปลี่ยนแปลงหรือไม่ได้ส่งค่าให้แอปพลิเคชัน Blynk*/
bool led_timer_on_set[3];
/*bool led_timer_on_set[3] เป็นตัวแปรชนิดข้อมูล boolean ที่ใช้เก็บสถานะการเปิดใช้งานของตั้งเวลา
สำหรับ LED 3 ตัว โดยตัวแปรนี้จะมีค่าเป็น true เมื่อตั้งเวลาสำหรับ LED ตัวที่ i ได้เปิดใช้งานแล้ว 
และ false เมื่อตั้งเวลาสำหรับ LED ตัวที่ i ยังไม่ได้เปิดใช้งาน



*/
// #########################################################################################################
// LED 1
BLYNK_WRITE(V0)
{
  int val = param.asInt();

  if ( led_timer_on_set[0] == 0 )
    led_set[0] = val;
  else
    update_blynk_status[0] = 1;
}
/*ฟังก์ชัน BLYNK_WRITE(V0) ใช้สำหรับรับค่าที่ถูกส่งมาจากแอปพลิเคชัน Blynk ผ่านพิน V0 โดยค่าที่ส่งมาจะถูกเก็บไว้ในตัวแปร param
และแปลงเป็นชนิดข้อมูล integer ด้วยฟังก์ชัน asInt()ในตัวอย่างนี้ 
ฟังก์ชันจะทำการตรวจสอบสถานะของตั้งเวลาสำหรับ LED ตัวที่ 1 
ด้วยการตรวจสอบค่า led_timer_on_set[0] ว่ามีค่าเป็น 0 หรือไม่ หากเป็น 0 แสดงว่าตั้งเวลาสำหรับ LED ตัวที่ 1 ยังไม่ได้เปิดใช้งาน 
ดังนั้นฟังก์ชันจะเปลี่ยนสถานะ LED ตัวที่ 1 ตามค่าที่ส่งมาจากแอปพลิเคชัน Blynk โดยเก็บค่านี้ไว้ในตัวแปร led_set[0]
ในกรณีที่ led_timer_on_set[0] เป็น 1 ฟังก์ชันจะเปลี่ยนค่าของตัวแปร update_blynk_status[0] เป็น 1 
เพื่อบอกให้ระบบทราบว่าสถานะของ LED ตัวที่ 1 มีการเปลี่ยนแปลงและต้องส่งค่าไปยังแอปพลิเคชัน Blynk 
ในการอัพเดทสถานะของ LED ตัวนี้ในภายหลัง



*/
// #########################################################################################################
// LED 2
BLYNK_WRITE(V1)
{
  int val = param.asInt();

  if ( led_timer_on_set[1] == 0 )
    led_set[1] = val;
  else
    update_blynk_status[1] = 1;  
}
// LED 3
BLYNK_WRITE(V2)
{
  int val = param.asInt();

  if ( led_timer_on_set[2] == 0 )
    led_set[2] = val;
  else
    update_blynk_status[2] = 1;  
}

// #########################################################################################################
// Timer 1
BLYNK_WRITE(V3) 
{
  unsigned char week_day;
 
  TimeInputParam t(param);

  if (t.hasStartTime() || t.hasStopTime() ) 
  {
    timer_start_set[0] = (t.getStartHour() * 60 * 60) + (t.getStartMinute() * 60) + t.getStartSecond();
    timer_stop_set[0] = (t.getStopHour() * 60 * 60) + (t.getStopMinute() * 60) + t.getStopSecond();
    
    Serial.println(String("Start Time: ") +
                   t.getStartHour() + ":" +
                   t.getStartMinute() + ":" +
                   t.getStartSecond());
                   
    Serial.println(String("Stop Time: ") +
                   t.getStopHour() + ":" +
                   t.getStopMinute() + ":" +
                   t.getStopSecond());
                   
    for (int i = 1; i <= 7; i++) /*for (initialization; condition; increment) {
  // statements
}
โดย initialization จะเป็นการกำหนดค่าเริ่มต้นของตัวแปร loop ซึ่งในตัวอย่างนี้คือ int i = 1 หมายความว่าตัวแปร i จะถูกกำหนดค่าเริ่มต้นเป็น 1

condition จะเป็นเงื่อนไขที่ต้องเป็นจริงเพื่อทำให้ loop ยังคงทำงาน ในตัวอย่างนี้เงื่อนไขคือ i <= 7 หมายความว่า loop จะยังทำงานไปเรื่อยๆ ถ้า i ยังไม่เกิน 7

increment จะเป็นการกำหนดการเปลี่ยนแปลงของตัวแปร loop หลังจากทำงานเสร็จในแต่ละรอบ ในตัวอย่างนี้เป็น i++ หมายความว่าค่า i จะถูกเพิ่มขึ้นทีละหนึ่งหลังจบการทำงานในแต่ละรอบของ loop

ดังนั้นเมื่อ i เท่ากับ 1 จนถึง 7 คำสั่งใน block ของ loop body จะถูกทำงานตามลำดับ โดย i จะเป็นตัวชี้ว่าตอนนี้ loop กำลังอยู่ที่รอบใด ในที่นี้ใช้ i เพื่อเช็ควันเดือนของการตั้งค่าเวลา (time input) ว่าวันไหนถูกเลือก (isWeekdaySelected)*/
    {
      if (t.isWeekdaySelected(i)) 
      {
        week_day |= (0x01 << (i-1));
        Serial.println(String("Day ") + i + " is selected");
      }
      else
      {
        week_day &= (~(0x01 << (i-1)));
      }
    } 

    weekday_set[0] = week_day;
  }
  else
  {
    timer_start_set[0] = 0xFFFF;
    timer_stop_set[0] = 0xFFFF;
  }
}
/*ฟังก์ชัน BLYNK_WRITE(V3) ใช้สำหรับรับค่าที่ถูกส่งมาจากแอปพลิเคชัน Blynk ผ่านพิน V3 โดยค่าที่ส่งมาจะถูกเก็บไว้ในตัวแปร param และจะถูกแปลงเป็นข้อมูลชนิด TimeInputParam โดยใช้ฟังก์ชัน t(param)

ในตัวอย่างนี้ ฟังก์ชันจะทำการตรวจสอบว่ามีการตั้งค่าเวลาเริ่มต้นหรือเวลาสิ้นสุดการทำงานหรือไม่ โดยใช้เงื่อนไข if (t.hasStartTime() || t.hasStopTime() )

ถ้ามีการตั้งค่าเวลาเริ่มต้นหรือเวลาสิ้นสุดการทำงาน ฟังก์ชันจะทำการอ่านค่าเวลาเริ่มต้นและเวลาสิ้นสุดที่ตั้งค่ามาจากแอปพลิเคชัน Blynk และแปลงเป็นหน่วยเวลาที่วัดได้เพื่อเก็บไว้ในตัวแปร timer_start_set[0] และ timer_stop_set[0] ตามลำดับ

ฟังก์ชันยังทำการตรวจสอบว่ามีวันใดบ้างที่ถูกเลือกในการตั้งค่าการทำงานในช่วงวันดังกล่าวหรือไม่ โดยใช้เงื่อนไข if (t.isWeekdaySelected(i)) และการกำหนดช่วงวันที่ถูกเลือกในตัวแปร week_day และเก็บค่าเว้นแต่ละวันลงในตัวแปร weekday_set[0]

ในกรณีที่ไม่มีการตั้งค่าเวลาเริ่มต้นหรือเวลาสิ้นสุดการทำงาน ฟังก์ชันจะกำหนดค่า timer_start_set[0] และ timer_stop_set[0] เป็นค่าสูงสุดของชนิดข้อมูล long (0xFFFF) เพื่อบอกให้ระบบทราบว่าไม่มีการตั้งค่าเวลาในช่วงนั้*/
// #########################################################################################################
// Timer 2
BLYNK_WRITE(V4) 
{
unsigned char week_day;
 
  TimeInputParam t(param);

  if (t.hasStartTime() || t.hasStopTime() ) 
  {
    timer_start_set[1] = (t.getStartHour() * 60 * 60) + (t.getStartMinute() * 60) + t.getStartSecond();
    timer_stop_set[1] = (t.getStopHour() * 60 * 60) + (t.getStopMinute() * 60) + t.getStopSecond();
    
    Serial.println(String("Start Time: ") +
                   t.getStartHour() + ":" +
                   t.getStartMinute() + ":" +
                   t.getStartSecond());
                   
    Serial.println(String("Stop Time: ") +
                   t.getStopHour() + ":" +
                   t.getStopMinute() + ":" +
                   t.getStopSecond());
                   
    for (int i = 1; i <= 7; i++) 
    {
      if (t.isWeekdaySelected(i)) 
      {
        week_day |= (0x01 << (i-1));
        Serial.println(String("Day ") + i + " is selected");
      }
      else
      {
        week_day &= (~(0x01 << (i-1)));
      }
    } 

    weekday_set[1] = week_day;
  }
  else
  {
    timer_start_set[1] = 0xFFFF;
    timer_stop_set[1] = 0xFFFF;
  }
}

// Timer 3
BLYNK_WRITE(V5) 
{
unsigned char week_day;
 
  TimeInputParam t(param);

  if (t.hasStartTime() || t.hasStopTime() ) 
  {
    timer_start_set[2] = (t.getStartHour() * 60 * 60) + (t.getStartMinute() * 60) + t.getStartSecond();
    timer_stop_set[2] = (t.getStopHour() * 60 * 60) + (t.getStopMinute() * 60) + t.getStopSecond();
    
    Serial.println(String("Start Time: ") +
                   t.getStartHour() + ":" +
                   t.getStartMinute() + ":" +
                   t.getStartSecond());
                   
    Serial.println(String("Stop Time: ") +
                   t.getStopHour() + ":" +
                   t.getStopMinute() + ":" +
                   t.getStopSecond());
                   
    for (int i = 1; i <= 7; i++) 
    {
      if (t.isWeekdaySelected(i)) 
      {
        week_day |= (0x01 << (i-1));
        Serial.println(String("Day ") + i + " is selected");
      }
      else
      {
        week_day &= (~(0x01 << (i-1)));
      }
    } 

    weekday_set[2] = week_day;
  }
  else
  {
    timer_start_set[2] = 0xFFFF;
    timer_stop_set[2] = 0xFFFF;
  }
}
// #########################################################################################################
BLYNK_WRITE(InternalPinRTC) //เพื่อติดต่อกับ Blynk server ผ่านbหลังจากที่เกิดการตั้งค่าเวลาใน RTC แล้ว ก็มีการคำนวณค่า rtc_sec โดยจะนับเวลาจากการตั้งค่าของ RTC แล้วแปลงเป็นวินาทีเพื่อเก็บไว้ในตัวแปร rtc_sec เพื่อนำไปใช้ในการควบคุมการเปิดปิดไฟตามเวลาที่กำหนดได้

/*จากนั้นก็มีการแสดงผลค่าของ blynkTime, RTC Server (เวลาที่ตั้งค่าใน RTC ปัจจุบัน), และ Day of Week (วันในสัปดาห์) ผ่านทาง Serial Monitor โดยใช้คำสั่ง Serial.println()



ที่กำหนดไว้ มีการใช้งานฟังก์ชัน BLYNK_WRITE(InternalPinRTC) เพื่อรับค่าเวลาจาก RTC Server ที่ถูกเซตโดย Blynk ผ่าน Internal Pin ที่กำหนดไว้ในโค้ด */
{
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013
  unsigned long blynkTime = param.asLong(); /*โดยในส่วนของโค้ดจะมีการกำหนดค่าคงที่ DEFAULT_TIME เพื่อเป็นเวลาเริ่มต้นในกรณีที่ไม่มีการเชื่อมต่อกับ RTC Server
   หรือเกิดข้อผิดพลาดในการรับค่า ต่อมาก็จะมีการรับค่าเวลาจาก Blynk server ผ่าน param.asLong() ซึ่งจะถูกเก็บไว้ในตัวแปร blynkTime*/
  
  if (blynkTime >= DEFAULT_TIME) 
  {
    setTime(blynkTime);

    day_of_week = weekday();
  
    if ( day_of_week == 1 )
      day_of_week = 7;
    else
      day_of_week -= 1; /*ในส่วนนี้จะเป็นการกำหนดค่าตัวแปร day_of_week 
      โดยดูจากค่าที่ได้จากฟังก์ชัน weekday() ว่าเท่ากับ 1 หรือไม่ 
      ถ้าเท่ากันแสดงว่าวันเป็นวันอาทิตย์ จะทำการกำหนดให้ day_of_week เป็น 7 
      แทนที่จะเป็น 1 ตามฟังก์ชัน weekday() ส่วนถ้าไม่เท่ากัน 
      ก็จะนำค่า day_of_week ลบด้วย 1 ซึ่งเป็นการเปลี่ยนค่าจาก 
      index ของวันในสัปดาห์จากฟังก์ชัน weekday() เป็น index ที่ใช้งานจริงโดยเริ่มจาก 
      1 แทนวันจันทร์ 2 แทนวันอังคาร 3 แทนวันพุธ 4 แทนวันพฤหัสบดี 5 แทนวันศุกร์ 
      6 แทนวันเสาร์ และ 7 
      แทนวันอาทิตย์ ซึ่งจะช่วยให้การจัดการกับวันในสัปดาห์ง่ายขึ้น*/




    /*โค้ดนี้ตรวจสอบว่าค่าเวลาที่ได้รับจาก Blynk มีค่ามากกว่าหรือเท่ากับค่าเริ่มต้นที่กำหนดไว้หรือไม่ 
    โดยค่าเริ่มต้นคือวันที่ 1 มกราคม ค.ศ. 2013 ถ้าค่าเวลาที่ได้รับมากกว่าหรือเท่ากับค่านี้ โปรแกรมจะใช้ค่าเวลานั้นในการตั้งเวลาของ RTC โดยใช้ฟังก์ชัน setTime() 
    เพื่อกำหนดค่าเวลาให้กับ RTC และคำนวณค่าวันในสัปดาห์จากค่าเวลานี้ด้วยฟังก์ชัน weekday() โดยในกรณีที่วันเป็นวันอาทิตย์จะแทนที่ค่าวันในสัปดาห์ด้วย 7 และในกรณีอื่นๆ 
    จะลดค่าวันในสัปดาห์ลง 1 วัน โดยเก็บค่าเวลาของ RTC เป็นวินาทีในตัวแปร rtc_sec ด้วยการคำนวณจากชั่วโมง นาที และวินาทีที่ได้จากฟังก์ชัน hour(), minute(), และ second() ตามลำดับ*/
    rtc_sec = (hour()*60*60) + (minute()*60) + second();
   
    Serial.println(blynkTime);
    Serial.println(String("RTC Server: ") + hour() + ":" + minute() + ":" + second());
    Serial.println(String("Day of Week: ") + weekday()); 
  }
}
/*หลังจากที่เกิดการตั้งค่าเวลาใน RTC แล้ว ก็มีการคำนวณค่า rtc_sec โดยจะนับเวลาจากการตั้งค่าของ RTC แล้วแปลงเป็นวินาทีเพื่อเก็บไว้ในตัวแปร rtc_sec เพื่อนำไปใช้ในการควบคุมการเปิดปิดไฟตามเวลาที่กำหนดได้

จากนั้นก็มีการแสดงผลค่าของ blynkTime, RTC Server (เวลาที่ตั้งค่าใน RTC ปัจจุบัน), และ Day of Week (วันในสัปดาห์) ผ่านทาง Serial Monitor โดยใช้คำสั่ง Serial.println()



*/
// #########################################################################################################
BLYNK_CONNECTED() 
{
  Blynk.sendInternal("rtc", "sync"); 
}

// #########################################################################################################
void checkTime() 
{
  Blynk.sendInternal("rtc", "sync"); 
}

// #########################################################################################################
void led_mng()
{
  bool time_set_overflow;
  bool led_status_buf[3];
  /*ฟังก์ชัน led_mng() เป็นฟังก์ชันที่รับผิดชอบการควบคุมไฟ LED โดยมีตัวแปร local ทั้งหมด 2 ตัว ได้แก่ time_set_overflow และ led_status_buf ซึ่งถูกประกาศเป็น boolean

ฟังก์ชันจะวนลูป 3 รอบเพื่อตรวจสอบสถานะของไฟ LED ทุกตัว โดยเก็บสถานะเดิมของ LED ไว้ใน led_status_buf และตรวจสอบเวลาตาม timer_start_set และ timer_stop_set ของ LED ที่กำลังตรวจสอบ ถ้าเวลาตามที่ตั้งไว้ตรงกับเวลาปัจจุบัน และวันของสัปดาห์ตรงกับที่ตั้งไว้ จะเปิดไฟ LED นั้น ถ้าไม่ตรงกัน LED จะปิด

หลังจากนั้นฟังก์ชันจะเปรียบเทียบสถานะ LED ก่อนหน้าและ LED ปัจจุบัน ถ้าสถานะของ LED นั้นเปลี่ยนแปลง จะตั้งค่า update_blynk_status เป็น 1 เพื่อแจ้งเตือน Blynk ให้รู้ว่าสถานะของ LED นั้นมีการเปลี่ยนแปลง*/
  for (int i=0; i<3; i++)/*นี่คือการเข้าไปวนลูป 3 รอบเพื่อจัดการการควบคุมไฟ LED สำหรับแต่ละ LED โดยการเข้าไปตรวจสอบสถานะของการตั้งเวลาและวันใช้งาน LED และตัดสินใจว่าจะเปิดหรือปิด LED ในแต่ละรอบ ดังนั้นตัวแปร i จะเป็นตัวแปรคอนโทลการวนลูป ที่ค่าเริ่มต้นเท่ากับ 0 และเพิ่มขึ้นทีละ 1 จนถึง 2 ตามจำนวนของ LED ที่ต้องจัดการ*/
  {
    led_status_buf[i] = led_status[i]; //คัดลอกค่า led_status[i] ไปยัง led_status_buf[i]
    time_set_overflow = 0;
    
    if ( timer_start_set[i] != 0xFFFF && timer_stop_set[i] != 0xFFFF)
    {
      if ( timer_stop_set[i] < timer_start_set[i] ) time_set_overflow = 1;

      if ((((time_set_overflow == 0 && (rtc_sec >= timer_start_set[i]) && (rtc_sec < timer_stop_set[i])) ||
        (time_set_overflow  && ((rtc_sec >= timer_start_set[i]) || (rtc_sec < timer_stop_set[i])))) && 
        (weekday_set[ i] == 0x00 || (weekday_set[i] & (0x01 << (day_of_week - 1) )))) )
        {
          led_timer_on_set[i] = 1;
        }
        else
          led_timer_on_set[i] = 0;
    }
    else
      led_timer_on_set[i] = 0;

    if ( led_timer_on_set[i] )
    {
      led_status[i] = 1;
      led_set[i] = 0;
    }
    else
    {
      led_status[i] = led_set[i];
    }

    if ( led_status_buf[i] != led_status[i] )
      update_blynk_status[i] = 1;  
  }
    // HARDWARE CONTROL
  digitalWrite(2, led_status[0]);  
  digitalWrite(15, led_status[1]);
  digitalWrite(4, led_status[2]);  
}

// #########################################################################################################
void blynk_update()
{
  if ( update_blynk_status[0] )
  {
      update_blynk_status[0] = 0;
      Blynk.virtualWrite(V0, led_status[0]);
  }  

  if ( update_blynk_status[1] )
  {
      update_blynk_status[1] = 0;
      Blynk.virtualWrite(V1, led_status[1]);
  } 
  if ( update_blynk_status[2] )
  {
      update_blynk_status[2] = 0;
      Blynk.virtualWrite(V2, led_status[2]);
  } 
}

// #########################################################################################################
void setup()
{
    
  Serial.begin(115200);
  delay(1000);
  pinMode(ldr1, INPUT);
  pinMode(ldr2, INPUT);
  pinMode(ldr3, INPUT);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(26, OUTPUT);
  BlynkEdgent.begin();
  timer.setInterval(10L, checkTime);
}

// #########################################################################################################
void loop() 
{
  int cl1 = analogRead(ldr1);
  int cl2 = analogRead(ldr2);
  int cl3 = analogRead(ldr3);

  BlynkEdgent.run();
  timer.run();
  led_mng();
  blynk_update();
  
   if (digitalRead(4) == 1 || digitalRead(2) == 1 || digitalRead(15) == 1){
     tone(bu1, 1000);
  }
  if (digitalRead(4) == 0 && digitalRead(2) == 0 && digitalRead(15) == 0)  
  {
     noTone(bu1);
  }
  // ldr1
  if(digitalRead(2) == 1)
  { if(cl1 > 2 ){
      digitalWrite(LED1,  0);
      }
  }
 //ldr2
  if(digitalRead(4) == 1)
  { if(cl2 > 2 ){
      digitalWrite(LED2,  0);
      }
  }
  //ldr3
    if(digitalRead(15) == 1)
  { if(cl3 > 2 ){
      digitalWrite(LED3,  0);
      }
  }
  Serial.println(cl1);
  Serial.println(cl2);
  Serial.println(cl3);
  delay(1000);
}