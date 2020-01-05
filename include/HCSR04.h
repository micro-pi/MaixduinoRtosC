#ifndef HCSR04_h
#define HCSR04_h

#include <Device.h>
#include <devices.h>
#include <osdefs.h>

class HCSR04 : public Device {
private:
  uint32_t timeout;
  uint32_t trigPin;
  uint32_t echoPin;
  handle_t gpio;

public:
  HCSR04(handle_t gpio, uint32_t trigPin, uint32_t echoPin, uint32_t timeout = 1000000);
  virtual ~HCSR04();

  void begin();

  unsigned int getDistanceMm();
  unsigned int getDistanceCm();
  unsigned int getDistanceInch();

  float getDistanceMmFloat();
  float getDistanceCmFloat();
  float getDistanceInchFloat();

private:
  uint32_t getDuration();
  uint32_t pulseIn(uint32_t pin, gpio_pin_value_t value, uint32_t timeout);
};

#endif