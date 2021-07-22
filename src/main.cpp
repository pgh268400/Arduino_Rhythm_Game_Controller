//라이브러리 참고 :: https://github.com/dmadison/HID_Buttons

#include "Arduino.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "HID_Buttons.h"  // 반드시 Keyboard.h Import 후 추가해야함.

const uint8_t JoyStickBtn = 7;
const uint8_t ButtonPin = 8, ButtonPin2 = 9, ButtonPin3 = 10, ButtonPin4 = 11; 
const uint8_t touch1 = 5, touch2 = 6;

const uint8_t Key = 'a', Key2 = 's', Key3 = ';', Key4 = '\''; 
KeyboardButton myButton(Key), myButton2(Key2), myButton3(Key3), myButton4(Key4);
KeyboardButton leftside(KEY_LEFT_SHIFT), rightside(KEY_RIGHT_SHIFT);

const uint8_t MouseInput = MOUSE_LEFT;
MouseButton mouseButton(MouseInput);

int readJoystick(int axis);

void setup() {

  //키보드&마우스 제어 시작
  Keyboard.begin();
  Mouse.begin();

  //풀업 저항 설정 - 눌렀을때가 LOW / 평상시가 HIGH
  pinMode(JoyStickBtn, INPUT_PULLUP);
  pinMode(ButtonPin, INPUT_PULLUP);
  pinMode(ButtonPin2, INPUT_PULLUP);
  pinMode(ButtonPin3, INPUT_PULLUP);
  pinMode(ButtonPin4, INPUT_PULLUP);

  //터치센서 설정 (내부 풀다운 있으니 SW적으로 설정할 필요 없음.)
  pinMode(touch1, INPUT);
  pinMode(touch2, INPUT);

}

void loop() {

 //키보드 처리 부분
   boolean pressed = !digitalRead(ButtonPin);
   myButton.set(pressed);

  boolean pressed2 = !digitalRead(ButtonPin2);
  myButton2.set(pressed2);

  boolean pressed3 = !digitalRead(ButtonPin3);
  myButton3.set(pressed3);

  boolean pressed4 = !digitalRead(ButtonPin4);
  myButton4.set(pressed4);

  //조이스틱 마우스 처리 부분
  int xVal=readJoystick(A0);   //x축값 읽어 저장 
  int yVal=readJoystick(A1);   //Y축값 읽어 저장

  //주의 할점 : x축, y축이 배선과 반대로 설정해야함.
  Mouse.move(-xVal, yVal, 0);      //해당 좌표로 마우스 이동

  //조이스틱 클릭 처리
  boolean joystick = !digitalRead(JoyStickBtn);
  mouseButton.set(joystick);

  //터치센서 처리
  boolean side1 = digitalRead(touch1);
  leftside.set(side1);

  boolean side2 = digitalRead(touch2);
  rightside.set(side2);
  
}

int readJoystick(int axis) {
  int val=analogRead(axis);
  val=map(val, 0, 1023, -4, 4);  //들어온 아날로그 값을 -4~4로 조정한다.
  if(val <=3 && val >= -3)   // 미세한 이동 값은 무시한다. (떨림 방지)
    return 0;
  else
  //마우스가 너무 급격하게 움직이므로 움직이는 양을 조절한다.
    if(val < 0){
      val = -2;
    } else {
      val = 2;
    }
    return val;  //맵핑된 값 전달 
}