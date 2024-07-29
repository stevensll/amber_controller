#include <Arduino.h>
#include <FlexCAN_T4.h>

const uint32_t BAUD_RATE = 115200;

const uint32_t CAN_RX_1 = 23;
const uint32_t CAN_TX_1 = 22;

const uint32_t CAN_SPEED = 100000;


FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> can_bus;
CAN_message_t msg;

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(LED_BUILTIN, OUTPUT);
  while (!Serial) ;       // wait for serial monitor
  can_bus.begin();
  can_bus.setBaudRate(BAUD_RATE);
}

void loop() {
  if (can_bus.read(msg)) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.printf("0x%08X", msg.id); Serial.print(" ");
    Serial.printf("0x%04X", msg.timestamp); Serial.print(" ");
    Serial.printf("0x%02X", msg.flags); Serial.print(" ");
    Serial.printf("0x%02X", msg.len); Serial.print(" ");
    for (byte i = 0; i < sizeof(msg.buf); i++) {
      Serial.printf("0x%02X", msg.buf[i]); Serial.print(" ");
    }
    Serial.println("");
    digitalWrite(LED_BUILTIN, LOW);
  }
}

