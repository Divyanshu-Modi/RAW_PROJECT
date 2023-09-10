/*
 * copyright (c) 2023, TEAM RAW
 * Divyanshu-Modi <divyan.m05@student.sfit.ac.in>
 * Date: 10-09-2023
 */

#include "PS4Controller.h"
#include "stdbool.h"

/*
 * struct motor
 * dir_pin - Set the pin to be used to set direction of motor.
 * pwm_pin - Set the pin to be used to send pwm signals to motor.
 * state - To set the state to HIGH or LOW for the dir_pin.
 * pwm - To set the pulse width on the pwm pin.
 * invert - invert the state pins.
 */
struct motor {
  uint8_t dir_pin;
  uint8_t pwm_pin;
  uint8_t state;
  uint8_t pwm;
  uint8_t channel;
  bool invert;
};

/*
 * Macros
 */
#define DEBUG
#define PWM_RES 8
#define PWM_FREQ 5000
#define MAX(x) map(x, 20, 128, 0, 200)
#define MIN(x) map(x, -20, -128, 0, 200)
#define MT_CTRL(id, d_pin, p_pin) \
struct motor mot##id {  \
  .dir_pin = d_pin,     \
  .pwm_pin = p_pin,     \
  .state = LOW,         \
  .pwm = 0,             \
  .channel = id,        \
  .invert = false,      \
}

MT_CTRL(1, 5, 27);
MT_CTRL(2, 13, 18);

/*
 * Functions
 */
void __motor_write(struct motor mot)
{
  if (mot.invert) {
    if (mot.state == HIGH)
      mot.state = LOW:
    else
      mot.state = HIGH:
  }
  ledcWrite(mot.channel, mot.pwm);
  digitalWrite(mot.dir_pin, mot.state);
}

void forward(int pwm)
{
#ifdef DEBUG
  Serial.printf("%s\n", __func__);
#endif

  mot1.state = HIGH;
  mot2.state = HIGH;
  mot1.pwm = pwm;
  mot2.pwm = pwm;

  __motor_write(mot1);
  __motor_write(mot2);
}

void backward(int pwm)
{
#ifdef DEBUG
  Serial.printf("%s\n", __func__);
#endif

  mot1.state = LOW;
  mot2.state = LOW;
  mot1.pwm = pwm;
  mot2.pwm = pwm;

  __motor_write(mot1);
  __motor_write(mot2);
}

void left(int pwm)
{
#ifdef DEBUG
  Serial.printf("%s\n", __func__);
#endif

  mot1.state = HIGH;
  mot2.state = LOW;
  mot1.pwm = pwm;
  mot2.pwm = pwm;

  __motor_write(mot1);
  __motor_write(mot2);
}

void right(int pwm)
{
#ifdef DEBUG
  Serial.printf("%s\n", __func__);
#endif

  mot1.state = LOW;
  mot2.state = HIGH;
  mot1.pwm = pwm;
  mot2.pwm = pwm;

  __motor_write(mot1);
  __motor_write(mot2);
}

void stop()
{
#ifdef DEBUG
  Serial.printf("%s\n", __func__);
#endif

  mot1.state = LOW;
  mot2.state = LOW;
  mot1.pwm = 0;
  mot2.pwm = 0;

  __motor_write(mot1);
  __motor_write(mot2);
}

static inline void __pin_init(struct motor mot)
{
  pinMode(mot.dir_pin, OUTPUT);
  pinMode(mot.pwm_pin, OUTPUT);
}

static inline void __ledc_init(struct motor mot)
{
  ledcAttachPin(mot.pwm_pin, mot.channel);
  ledcSetup(mot.channel, PWM_FREQ, PWM_RES);
}

void setup() {
  __pin_init(mot1);
  __pin_init(mot2);
  __ledc_init(mot1);
  __ledc_init(mot2);
#ifdef DEBUG
  Serial.begin(9600);
#endif
  PS4.begin("78:21:84:DF:23:E8");
}

void loop() {
  int LX = PS4.LStickX();
  int LY = PS4.LStickY();

  if (LX > 20) {
    forward(MAX(LX));
  } else if (LX < -20) {
    backward(MIN(LX));
  } else if (LY > 20) {
    left(MAX(LY));
  } else if (LY < -20) {
    right(MIN(LY));
  } else {
    stop();
  }
}
