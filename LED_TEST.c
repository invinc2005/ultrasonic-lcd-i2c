#include <reg51.h>
#include <intrins.h>
#include <stdio.h>

// I2C LCD Pins
sbit SDA = P0^7;
sbit SCL = P0^6;

// Ultrasonic Sensor Pins
sbit TRIG = P3^2;
sbit ECHO = P3^3;

void i2c_start(void);
void i2c_stop(void);
void i2c_ACK(void);
void i2c_write(unsigned char);
void lcd_send_cmd(unsigned char);
void lcd_send_data(unsigned char);
void lcd_send_str(unsigned char *);
void lcd_slave(unsigned char);
void delay_ms(unsigned int);
void send_trigger();
unsigned int measure_distance();
void int_to_string(unsigned int, char *);

unsigned char slave1 = 0x4E;
unsigned char slave_add;

//  I2C Start Condition 
void i2c_start(void) {
    SDA = 1; _nop_(); _nop_();
    SCL = 1; _nop_(); _nop_();
    SDA = 0; _nop_(); _nop_();
}

//  I2C Stop Condition 
void i2c_stop(void) {
    SCL = 0;
    SDA = 0;
    SCL = 1;
    SDA = 1;
}

//  Set LCD Slave Address 
void lcd_slave(unsigned char slave) {
    slave_add = slave;
}

//  I2C ACK 
void i2c_ACK(void) {
    SCL = 0;
    SDA = 1;
    SCL = 1;
    while(SDA);
}

//  I2C Write Byte 
void i2c_write(unsigned char dat) {
    unsigned char i;
    for(i = 0; i < 8; i++) {
        SCL = 0;
        SDA = (dat & (0x80 >> i)) ? 1 : 0;
        SCL = 1;
    }
}

//  Send Command to LCD via I2C 
void lcd_send_cmd(unsigned char cmd) {
    unsigned char cmd_l, cmd_u;
    cmd_l = (cmd << 4) & 0xF0;
    cmd_u = (cmd & 0xF0);
    
    i2c_start();
    i2c_write(slave_add);
    i2c_ACK();
    i2c_write(cmd_u | 0x0C);
    i2c_ACK();
    delay_ms(1);
    i2c_write(cmd_u | 0x08);
    i2c_ACK();
    delay_ms(10);
    i2c_write(cmd_l | 0x0C);
    i2c_ACK();
    delay_ms(1);
    i2c_write(cmd_l | 0x08);
    i2c_ACK();
    delay_ms(10);
    i2c_stop();
}

//  Send Data to LCD via I2C 
void lcd_send_data(unsigned char dataw) {
    unsigned char dataw_l, dataw_u;
    dataw_l = (dataw << 4) & 0xF0;
    dataw_u = (dataw & 0xF0);
    
    i2c_start();
    i2c_write(slave_add);
    i2c_ACK();
    i2c_write(dataw_u | 0x0D);
    i2c_ACK();
    delay_ms(1);
    i2c_write(dataw_u | 0x09);
    i2c_ACK();
    delay_ms(10);
    i2c_write(dataw_l | 0x0D);
    i2c_ACK();
    delay_ms(1);
    i2c_write(dataw_l | 0x09);
    i2c_ACK();
    delay_ms(10);
    i2c_stop();
}

//  Send String to LCD 
void lcd_send_str(unsigned char *p) {
    while(*p != '\0') {
        lcd_send_data(*p++);
    }
}

//  Delay Function 
void delay_ms(unsigned int n) {
    unsigned int m;
    for(; n > 0; n--) {
        for(m = 121; m > 0; m--);
        _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
    }
}

//  LCD Initialization 
void lcd_init() {
    lcd_send_cmd(0x02);  // Return home
    lcd_send_cmd(0x28);  // 4-bit mode
    lcd_send_cmd(0x0C);  // Display On, Cursor Off
    lcd_send_cmd(0x06);  // Increment Cursor (Shift cursor to right)
    lcd_send_cmd(0x01);  // Clear Display
}

//  Send Trigger Pulse to Ultrasonic Sensor 
void send_trigger() {
    TRIG = 1;
    delay_ms(1);
    TRIG = 0;
}

//  Measure Distance Using Timer 
unsigned int measure_distance() {
    unsigned int time;
    
    send_trigger();
    
    while (!ECHO);  // Wait for Echo HIGH
    TH0 = 0;
    TL0 = 0;
    TR0 = 1;  // Start Timer
    
    while (ECHO);  // Wait for Echo LOW
    TR0 = 0;  // Stop Timer
    
    time = (TH0 << 5) | TL0;
    return (time / 58);  // Convert to cm
}

//  Convert Integer to String (Replaces sprintf()) 
void int_to_string(unsigned int num, char *str) {
    str[0] = (num / 100) + '0';
    str[1] = ((num / 10) % 10) + '0';
    str[2] = (num % 10) + '0';
    str[3] = 'c';
    str[4] = 'm';
    str[5] = '\0';
}

//  Main Function 
void main(void) {
    unsigned int distance;
    char distance_str[6];

    lcd_slave(slave1);
    lcd_init();
    
    lcd_send_cmd(0x80);
    lcd_send_str(" Distance: ");
    
    while(1) {
        distance = measure_distance();
        int_to_string(distance, distance_str);

        lcd_send_cmd(0xC0);  // Move cursor to second row
        lcd_send_str(distance_str);
        
        delay_ms(500);  // Wait before next measurement
    }
}
