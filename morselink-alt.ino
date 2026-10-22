/*
  Install https://github.com/DeqingSun/ch55xduino in your arduino IDE

  select the board type as CH55xDuino as a CH552 and choose your com port.
  "Verify" the code in the arduino IDE before uploading
  You will need to unplug the USB device, hold down the physical button on the board and plug it back in with the button held while the arduino upload is in progress.
  Timing is everything.  If it times out you didn't get it unplugged and plugged in with the button pressed fast enough.  Try again!  Once you have done it the first time it will be easier when you need to reload again.

  This code works for me using "https://hamradio.solutions/vband/"  No warranty, guarantees or support implied or granted!  Use at your own risk!

  If you don't already own the device for this it would be better to purchase the proper device from the store link on "https://hamradio.solutions/vband/"

  I am not affiliated with hamradio solutions nor do I represent them.
*/


/*
  CH552 iambic paddle / straight key -> USB HID

  Left paddle  (P1.5 / pin identifier 15)
    Keyboard mode: Left Ctrl
    Mouse mode:    Left mouse button

  Right paddle (P1.6 / pin identifier 16)
    Keyboard mode: Right Ctrl
    Mouse mode:    Right mouse button

  Startup mode selection:
    Either input pressed at startup -> Mouse mode
    Neither input pressed           -> Keyboard mode

  Startup sequence:
    1. Sample inputs and select mode.
    2. Initialize USB HID.
    3. Wait 3 seconds.  (if using a straight key that shorts the 'ring' on the jack plug in your key after pluggin in USB to be in keyboard mode)
    4. Capture current input states as the initial baseline.
    5. Begin normal operation.
*/

#ifndef USER_USB_RAM
#error "Compile with a USER USB setting"
#endif

#include "src/userUsbHidKeyboardMouse/USBHIDKeyboardMouse.h"

#define PADDLE_LEFT  15
#define PADDLE_RIGHT 16

#define CONTROL_LEFT  0x80
#define CONTROL_RIGHT 0x84

bool mouseMode = false;

bool leftWasPressed = false;
bool rightWasPressed = false;


void setup() {
  // Configure both inputs with internal pull-ups.
  pinMode(PADDLE_LEFT, INPUT_PULLUP);
  pinMode(PADDLE_RIGHT, INPUT_PULLUP);

  // Allow the inputs to settle.
  delay(30);

  // Select the mode immediately at startup.
  // Paddle closures connect inputs to ground.
  bool leftStartupPressed =
      (digitalRead(PADDLE_LEFT) == LOW);

  bool rightStartupPressed =
      (digitalRead(PADDLE_RIGHT) == LOW);

  // Holding either input at startup selects mouse mode.
  mouseMode = leftStartupPressed || rightStartupPressed;

  // Initialize USB HID.
  USBInit();

  // Allow 3 seconds for key connection or settling.
  // Mode selection is already locked.
  delay(3000);

  // Treat the current input states as the released baseline.
  // No HID press is generated solely because an input is LOW.
  leftWasPressed =
      (digitalRead(PADDLE_LEFT) == LOW);

  rightWasPressed =
      (digitalRead(PADDLE_RIGHT) == LOW);
}


void loop() {
  bool leftPressed =
      (digitalRead(PADDLE_LEFT) == LOW);

  bool rightPressed =
      (digitalRead(PADDLE_RIGHT) == LOW);

  if (mouseMode) {
    // Left input -> left mouse button.
    if (leftPressed != leftWasPressed) {
      leftWasPressed = leftPressed;

      if (leftPressed) {
        Mouse_press(MOUSE_LEFT);
      } else {
        Mouse_release(MOUSE_LEFT);
      }
    }

    // Right input -> right mouse button.
    if (rightPressed != rightWasPressed) {
      rightWasPressed = rightPressed;

      if (rightPressed) {
        Mouse_press(MOUSE_RIGHT);
      } else {
        Mouse_release(MOUSE_RIGHT);
      }
    }
  } else {
    // Left input -> Left Ctrl.
    if (leftPressed != leftWasPressed) {
      leftWasPressed = leftPressed;

      if (leftPressed) {
        Keyboard_press(CONTROL_LEFT);
      } else {
        Keyboard_release(CONTROL_LEFT);
      }
    }

    // Right input -> Right Ctrl.
    if (rightPressed != rightWasPressed) {
      rightWasPressed = rightPressed;

      if (rightPressed) {
        Keyboard_press(CONTROL_RIGHT);
      } else {
        Keyboard_release(CONTROL_RIGHT);
      }
    }
  }

  delay(10);
}

