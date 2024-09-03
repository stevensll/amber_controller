const int BAUD_RATE = 19200;


/* SPI */

/* Hardware */
const int SSI_SPI_sel = 10;
const int SSI_NSL_SPI_DIN = 11; 
const int SSI_DO_SPI_DO = 12;
const int SSI_SCL_SPI_SCLK = 13;


const int SSI_ENABLE = 1;

const int SSI_CLCK_INACTIVE = 1;  // clock inactive when 1
const int SSI_CLCK_ACTIVE = 0;    // active when 1
const int SSI_LOAD_MODE = 1;      // loading when 1
const int SSI_SHIFT_MODE = 0;      // shift mode
const int SSI_NSL_HOLD_TIME_MS = 3; // 3 ms


// Timing constants
const int TSW_us = 1;
const int T_REQ_ns = 300;
const int T_REQ2_ns = 200;
const int T_NSLH_ns = 200;

// Read constants
const int READ_SIZE = 16;
const int READY_BIT = READ_SIZE + 1;
const int MHI_ERROR_BIT = READY_BIT + 1;
const int MLO_ERROR_BIT = MHI_ERROR_BIT + 1;
const int PARITY_BIT = MLO_ERROR_BIT + 1;



void printEncoderData(unsigned long data){
  Serial.print("Data: ");
  Serial.print(data >> 4);
  Serial.print(" Ready: ");
  Serial.print((data & 0b1000) >> 3);
  Serial.print(" MHI : ");
  Serial.print((data & 0b100) >> 2);
  Serial.print(" MLO : ");
  Serial.print((data & 0b10) >> 1);
  Serial.print(" Parity: ");
  Serial.println(data & 0b01);
}
void setup() {
  // Configure the I/O pins
  pinMode(SSI_SCL_SPI_SCLK, OUTPUT);
  pinMode(SSI_NSL_SPI_DIN, OUTPUT);
  pinMode(SSI_SPI_sel, OUTPUT);
  pinMode(SSI_DO_SPI_DO, INPUT);
  // Enable SSI with inactive clock in load mode
  digitalWrite(SSI_SPI_sel, SSI_ENABLE);
  digitalWrite(SSI_SCL_SPI_SCLK, SSI_CLCK_INACTIVE);
  digitalWrite(SSI_NSL_SPI_DIN, SSI_LOAD_MODE);
  // Hold after powering up
  delay(SSI_NSL_HOLD_TIME_MS + 1);          
  // Serial
  Serial.begin(BAUD_RATE);
}


void loop() {
  // put your main code here, to run repeatedly:
  unsigned long data = data_shift(SSI_NSL_SPI_DIN, SSI_DO_SPI_DO, SSI_SCL_SPI_SCLK, READ_SIZE + 4);
  printEncoderData(data);
}

unsigned long data_shift(const int enable_pin, const int data_pin, const int clock_pin, const int bit_count){
  unsigned long data = 0;
  digitalWrite(clock_pin, SSI_CLCK_INACTIVE);
  digitalWrite(enable_pin, SSI_LOAD_MODE);
  delayMicroseconds(TSW_us + 0.5);
  digitalWrite(enable_pin, SSI_SHIFT_MODE);
  delayMicroseconds((T_REQ_ns + 50) / 1000.0);
  for (int i = 0; i < bit_count; i++){
    data <<= 1;
    digitalWrite(clock_pin, HIGH);
    delayMicroseconds(1);
    digitalWrite(clock_pin, LOW);
    delayMicroseconds(1);
    data |= digitalRead(data_pin);
  }
  delayMicroseconds((T_REQ2_ns + 50) / 1000.0);
  digitalWrite(enable_pin, SSI_LOAD_MODE);
  delayMicroseconds((T_NSLH_ns + 50) / 1000.0);
  return data;
}

