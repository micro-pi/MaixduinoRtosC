#ifndef LED_H
#define LED_H

#include <Device.h>
#include <devices.h>
#include <osdefs.h>

class Led : public Device {
private:
  int pin;
  gpio_pin_value_t value;
  handle_t gpio;

public:
  Led(handle_t gpio, int pin);
  virtual ~Led();

  virtual void begin(void);
  virtual void on(void);
  virtual void off(void);
  virtual void blink(void);
  virtual bool isOn(void);

private:
  void setValue(gpio_pin_value_t value);
};

#endif