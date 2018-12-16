#include <SPI.h>

/* Interrupts:
 *  PIN2 Left Button
 *  PIN3 Right Button
 * SPI:
 *  PIN10 CS
 *  PIN11 DIN
 *  PIN13 CLK
 */

//LED MATRIX REGISTERS
#define DECODE_MODE_REG 0x09
#define INTENSITY_REG 0x0A
#define SCAN_LIMIT_REG 0x0B
#define SHUTDOWN_REG 0x0C
#define TEST_REG 0x0F

//PINS
#define CS 10
#define LEFT_BUTTON 2
#define RIGHT_BUTTON 3

//Other
#define LEFT 0xFF
#define RIGHT 0x1
#define ANIMATION_SPEED 50

void matrix_write_register(uint8_t address, uint8_t value){
  //noInterrupts();
  digitalWrite(CS, LOW);
  SPI.transfer(address);      // Send address.
  SPI.transfer(value);        // Send the value.
  digitalWrite(CS, HIGH);
  //interrupts();
}

void matrix_init(){
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  matrix_write_register(DECODE_MODE_REG, 0x00);
  matrix_write_register(SCAN_LIMIT_REG, 0x0F);
  matrix_write_register(INTENSITY_REG, 0x0F);
  matrix_write_register(SHUTDOWN_REG, 0x01);
  matrix_write_register(TEST_REG, 0x00);
}

void matrix_reset(){
  matrix_write_register(0x1, 0x00);
  matrix_write_register(0x2, 0x00);
  matrix_write_register(0x3, 0x00);
  matrix_write_register(0x4, 0x00);
  matrix_write_register(0x5, 0x00);
  matrix_write_register(0x6, 0x00);
  matrix_write_register(0x7, 0x00);
  matrix_write_register(0x8, 0x00);
}

void matrix_print_arrow(uint8_t dir){
  uint8_t addr;
  if(dir==RIGHT){
    addr = 0x0;
  }else{
    addr = 0x9;
  }

  //if RIGHT, addr = addr +1
  //if LEFT, addr = addr + 0xFF == addr - 1
  delay(ANIMATION_SPEED);
  matrix_write_register(addr+=dir, 0b00111100);
  delay(ANIMATION_SPEED);
  matrix_write_register(addr+=dir, 0b00100100);
  delay(ANIMATION_SPEED);
  matrix_write_register(addr+=dir, 0b00100100);
  delay(ANIMATION_SPEED);
  matrix_write_register(addr+=dir, 0b00100100);
  delay(ANIMATION_SPEED);
  matrix_write_register(addr+=dir, 0b11100111);
  delay(ANIMATION_SPEED);
  matrix_write_register(addr+=dir, 0b01000010);
  delay(ANIMATION_SPEED);
  matrix_write_register(addr+=dir, 0b00100100);
  delay(ANIMATION_SPEED);
  matrix_write_register(addr+=dir, 0b00011000);
  delay(700);
  matrix_reset();
    
}

byte left_state = 0;
byte right_state = 0;

void turn_left(){
  left_state = not left_state;
  right_state = 0;
}

void turn_right(){
  right_state = not right_state;
  left_state = 0;
}

void setup(){
  matrix_init();
  matrix_reset();

  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(LEFT_BUTTON), turn_left, FALLING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_BUTTON), turn_right, FALLING);

}

void loop(){
  if(left_state) matrix_print_arrow(LEFT);
  if(right_state) matrix_print_arrow(RIGHT);
}
