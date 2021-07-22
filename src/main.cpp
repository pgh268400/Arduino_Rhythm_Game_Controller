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
  int reading = analogRead(axis); 
  reading = map(reading, 0, 1023, 0, 12); // 0~12 사이의 값으로 변환
  int distance = reading - 6; //축값은 0~12인데 중심값이 6을 기준으로 +방향과 -방향값을 갖음
  if (abs(distance) < 3) { //미세한 떨림은 무시함.
    distance = 0;
  } else {
    //너무 급격하게 움직이므로 속도를 고정함.
    if(distance < 0){
      distance = -1;
    } else {
      distance = 1;
    }
  }
  return distance; //해당 축의 마우스 이동값을 반환
}