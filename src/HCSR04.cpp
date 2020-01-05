#include <HCSR04.h>
#include <sleep.h>
#include <sysctl.h>
#include <utils.h>

HCSR04::HCSR04(handle_t gpio, uint32_t trigPin, uint32_t echoPin, uint32_t timeout) {
  this->gpio = gpio;
  this->trigPin = trigPin;
  this->echoPin = echoPin;
  this->timeout = timeout;
}

HCSR04::~HCSR04() {
}

void HCSR04::begin(void) {
  gpio_set_drive_mode(this->gpio, this->trigPin, GPIO_DM_OUTPUT);
  gpio_set_drive_mode(this->gpio, this->echoPin, GPIO_DM_INPUT);
}

uint32_t HCSR04::getDuration() {
  gpio_set_pin_value(this->gpio, this->trigPin, GPIO_PV_LOW);
  usleep(2);
  gpio_set_pin_value(this->gpio, this->trigPin, GPIO_PV_HIGH);
  usleep(10);
  gpio_set_pin_value(this->gpio, this->trigPin, GPIO_PV_LOW);
  return pulseIn(echoPin, GPIO_PV_HIGH, timeout);
}
unsigned int HCSR04::getDistanceMm() {
  unsigned int duration = (unsigned int)getDuration();
  if (duration) {
    return (unsigned int)((duration * 5) / 29);
  } else {
    return 0;
  }
}

float HCSR04::getDistanceMmFloat() {
  unsigned int duration = (unsigned int)getDuration();
  if (duration) {
    return duration / 5.8f;
  } else {
    return 0.0f;
  }
}

unsigned int HCSR04::getDistanceCm() {
  unsigned int duration = (unsigned int)getDuration();
  if (duration) {
    return (unsigned int)(duration / 58);
  } else {
    return 0;
  }
}

float HCSR04::getDistanceCmFloat() {
  unsigned int duration = (unsigned int)getDuration();
  if (duration) {
    return duration / 58.0f;
  } else {
    return 0.0f;
  }
}

unsigned int HCSR04::getDistanceInch() {
  unsigned int duration = (unsigned int)getDuration();
  if (duration) {
    return (unsigned int)(duration / 148);
  } else {
    return 0;
  }
}

float HCSR04::getDistanceInchFloat() {
  unsigned int duration = (unsigned int)getDuration();
  if (duration) {
    return duration / 148.0f;
  } else {
    return 0;
  }
}

uint32_t HCSR04::pulseIn(uint32_t pin, gpio_pin_value_t state, uint32_t timeout) {
  uint32_t cpu_freq = sysctl_clock_get_freq(SYSCTL_CLOCK_CPU);
  uint32_t numloops = 0;
  uint32_t maxloops = timeout * (cpu_freq / 1000000L) / 16; // microsecondsToClockCycles(timeout) / 16; ???
  uint32_t start, end;

  /* wait for any previous pulse to end */
  while (gpio_get_pin_value(this->gpio, pin) == state) {
    if (numloops++ == maxloops) {
      return 0;
    }
  }

  /* wait for the pulse to start */
  while (gpio_get_pin_value(this->gpio, pin) != state) {
    if (numloops++ == maxloops) {
      return 0;
    }
  }
  /* wait for the pulse to stop */
  //start = micros();
  while (gpio_get_pin_value(this->gpio, pin) == state) {
    if (numloops++ == maxloops) {
      return 0;
    }
  }
  //end = micros();

  return (end - start);
}
