/***********************************************************************
 * Filename: pin_map.h
 * Author: Pavel Kejik
 * Date: 2024-04-12
 * Description:
 *     Defines pin mappings for various components like RS485 communication,
 *     motor control, encoder inputs, and sensors.
 *
 ***********************************************************************/


#pragma once

#define DE485 14
#define TXD485 17
#define RXD485 18
#define RS485_SERIAL_PORT 1
#define MOTOR_UP 9
#define MOTOR_DOWN 8
#define MOTOR_EN 10
#define MOTOR_RELAY 12
#define MOTOR_ISENSE 2
#define ENCODER_PWR 13
#define ENCODER_A 5 // Encoder on the left
#define ENCODER_B 6 // Encoder on the right
#define BTN 39
#define LED_R 47
#define LED_G 48
#define PHOTO_R 1
#define TEMP_R 4