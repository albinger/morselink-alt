/*
  Install https://github.com/DeqingSun/ch55xduino in your arduino IDE

  select the board type as CH55xDuino as a CH552 and choose your com port.
  "Verify" the code in the arduino IDE before uploading
  You will need to unplug the USB device, hold down the physical button on the board and plug it back in with the button held while the arduino upload is in progress.
  Timing is everything.  If it times out you didn't get it unplugged and plugged in with the button pressed fast enough.  Try again!  Once you have done it the first time it will be easier when you need to reload again.

  This code works for me using "https://hamradio.solutions/vband/"  No warranty, guarantees or support implied or granted!  Use at your own risk!

  If you don't already own the device for this it would be better to purchase the proper device from the store link on "https://hamradio.solutions/vband/"

  I am not affiliated with hamradio solutions nor do I represent them.
  
  CH552 iambic paddle -> USB HID Control keys
  Left paddle  (P1.5 / pin identifier 15) -> Left Ctrl
  Right paddle (P1.6 / pin identifier 16) -> Right Ctrl
*/

#ifndef USER_USB_RAM
#error "Compile with a USER USB setting"
#endif

#include "src/userUsbHidKeyboard/USBHIDKeyboard.h"

#define PADDLE_LEFT  15
#define PADDLE_RIGHT 16

#define CONTROL_LEFT  0x80
#define CONTROL_RIGHT 0x84

bool leftWasPressed = false;
bool rightWasPressed = false;

void setup() {
  USBInit();

  pinMode(PADDLE_LEFT, INPUT_PULLUP);
  pinMode(PADDLE_RIGHT, INPUT_PULLUP);
}

void loop() {
  // Paddle closures connect the inputs to ground.
  bool leftPressed  = (digitalRead(PADDLE_LEFT) == LOW);
  bool rightPressed = (digitalRead(PADDLE_RIGHT) == LOW);

  // Left paddle: hold Left Ctrl until released.
  if (leftPressed != leftWasPressed) {
    leftWasPressed = leftPressed;

    if (leftPressed) {
      Keyboard_press(CONTROL_LEFT);
    } else {
      Keyboard_release(CONTROL_LEFT);
    }
  }

  // Right paddle: hold Right Ctrl until released.
  if (rightPressed != rightWasPressed) {
    rightWasPressed = rightPressed;

    if (rightPressed) {
      Keyboard_press(CONTROL_RIGHT);
    } else {
      Keyboard_release(CONTROL_RIGHT);
    }
  }

  delay(10);
}
