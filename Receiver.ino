#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <HID-Project.h>

RF24 radio(9, 10); // CE, CSN pins
const byte address[6] = "00001"; // Address for communication

struct JoystickData {
  int xAxis;
  int yAxis;
  int zAxis;
  int rYaxis;
  int rXaxis;
  int rZaxis;
  int switch1;
  int switch2;
  int switch3;
  int switch4;
  int switch5;
  int switch6;
  int switch7;
  int switch8;
};

JoystickData joystick;

void setup() {
  Gamepad.begin(); // Initialize the gamepad
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(true);
  radio.setDataRate(RF24_2MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
}

void loop() {
  if (radio.available()) {

    radio.read(&joystick, sizeof(JoystickData));

    // Map joystick values (0-1023) to 8-bit HID joystick range (0-255)
    int16_t mappedX = map(joystick.xAxis, 0, 1023, -32768, +32767); // X-axis
    int16_t mappedY = map(joystick.yAxis, 0, 1023, -32768, +32767); // Y-axis
    int8_t mappedZ = map(joystick.zAxis, 0, 1023, -127, 127); // Z-axis
    int16_t mappedRy = map(joystick.rYaxis, 0, 1023, -32768, +32767); // right Y-axis
    int16_t mappedRx = map(joystick.rXaxis, 0, 1023, -32768, +32767); // right X-axis
    int8_t mappedRz = map(joystick.rZaxis, 0, 1023, -127, 128); // right Z-axis

    // Update Gamepad axes with 8-bit resolution
    Gamepad.xAxis(mappedX);
    Gamepad.yAxis(mappedY);
    Gamepad.zAxis(mappedZ);
    Gamepad.ryAxis(mappedRy);
    Gamepad.rxAxis(mappedRx); 
    Gamepad.rzAxis(mappedRz);

    // Map buttons to HID Gamepad
    uint8_t buttonStates = 0; // 8-bit value for buttons
    if (joystick.switch1 == LOW) buttonStates |= (1 << 4); // Button 5
    if (joystick.switch2 == LOW) buttonStates |= (1 << 5); // Button 6
    if (joystick.switch3 == LOW) buttonStates |= (1 << 3); // Button 4
    if (joystick.switch4 == LOW) buttonStates |= (1 << 0); // Button 1
    if (joystick.switch5 == LOW) buttonStates |= (1 << 1); // Button 2
    if (joystick.switch6 == LOW) buttonStates |= (1 << 2); // Button 3
    if (joystick.switch7 == LOW) buttonStates |= (1 << 6); // Button 7
    if (joystick.switch8 == LOW) buttonStates |= (1 << 7); // Button 8

    Gamepad.buttons(buttonStates);
    // Send updated gamepad state
    Gamepad.write();
  }
}
