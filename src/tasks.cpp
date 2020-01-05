#include <FreeRTOS.h>
#include <ITG3200.h>
#include <Led.h>
#include <cmath>
#include <project_cfg.h>
#include <stdio.h>
#include <task.h>
#include <tasks.h>
#include <utils.h>

static float aX;
static float aY;
static float aZ;
static float gyroAngleX;
static float gyroAngleY;
static float gyroAngleZ;

static float elapsedTime;
static float currentTime;
static float previousTime;

static int16_t itg3200CalibrationSamples = CALIBRATION_SAMPLES;

void vPowerBlinkTaskCore0(void *arg) {
  const TickType_t xFrequency = 500;
  handle_t gpio0;
  TickType_t xLastWakeTime;

  /* Initialise the xLastWakeTime variable with the current time. */
  xLastWakeTime = xTaskGetTickCount();
  gpio0 = io_open("/dev/gpio0");

  Led blinker(gpio0, LED_PIN);
  blinker.begin();
  while (1) {
    blinker.blink();

    /* Wait for the next cycle. */
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(xFrequency));
  }
}

void vI2cTaskCore1(void *arg) {
  const TickType_t xFrequency = 1000;
  TickType_t xLastWakeTime;
  /* Initialise the xLastWakeTime variable with the current time. */
  xLastWakeTime = xTaskGetTickCount();

  while (1) {
    //printf("%07.2f x %07.2f x %07.2f\r\n", gyroAngleX, gyroAngleY, gyroAngleZ);
    printf("%lu\r\n", millis());

    /* Wait for the next cycle. */
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(xFrequency));
  }
}

void vITG3200TaskCore1(void *arg) {
  handle_t i2c0;
  int32_t xOffsetTemp = 0;
  int32_t yOffsetTemp = 0;
  int32_t zOffsetTemp = 0;
  int16_t gX;
  int16_t gY;
  int16_t gZ;

  i2c0 = io_open("/dev/i2c0");

  ITG3200 gyro(i2c0);
  gyro.begin();
  gyro.setFullScaleSelection(RANGE_2000_DEG_PER_SEC);
  gyro.setDigitalLowPassFilter(BANDWIDTH_5HZ_RATE_1KHZ);
  gyro.setRawDataReadyEnabled(true);

  printf("Who Am I             : 0x%02x\r\n", gyro.whoAmI());
  printf("\r\n----Sample Rate Divider----\r\n");
  printf("Sample Rate Divider  : 0x%02x\r\n", gyro.getSampleRateDivider());
  printf("\r\n------DLPF, Full Scale-----\r\n");
  printf("Full scale selection : 0x%02x\r\n", gyro.getFullScaleSelection());
  printf("Digital low pass     : 0x%02x\r\n", gyro.getDigitalLowPassFilter());
  printf("\r\n--Interrupt Configuration--\r\n");
  printf("Logic level          : 0x%02x\r\n", gyro.getLogicLevelIntOutputPin());
  printf("Drive type           : 0x%02x\r\n", gyro.getDriveTypeIntOutputPin());
  printf("Latch mode           : 0x%02x\r\n", gyro.getLatchMode());
  printf("Latch clear method   : 0x%02x\r\n", gyro.getLatchClearMethod());
  printf("Interrupt Enabled    : 0x%02x\r\n", gyro.isInterruptEnabled());
  printf("Raw data ready       : 0x%02x\r\n", gyro.isRawDataReadyEnabled());
  printf("\r\n------Interrupt Status-----\r\n");
  printf("PLL ready            : 0x%02x\r\n", gyro.isPllReady());
  printf("Raw data is ready    : 0x%02x\r\n", gyro.isRawDataReady());
  printf("\r\n------Power Management-----\r\n");
  printf("Low power sleep mode : 0x%02x\r\n", gyro.isSleepMode());
  printf("X in standby mode    : 0x%02x\r\n", gyro.isStandbyModeX());
  printf("Y in standby mode    : 0x%02x\r\n", gyro.isStandbyModeY());
  printf("Z in standby mode    : 0x%02x\r\n", gyro.isStandbyModeZ());
  printf("Clock source         : 0x%02x\r\n", gyro.getClockSource());

  while (1) {
    if (gyro.isRawDataReady()) {
      if (itg3200CalibrationSamples > 0) {
        taskENTER_CRITICAL();
        gyro.getXYZ(&gX, &gY, &gZ);
        taskEXIT_CRITICAL();
        xOffsetTemp += gX;
        yOffsetTemp += gY;
        zOffsetTemp += gZ;
        itg3200CalibrationSamples--;
      } else {
        if (itg3200CalibrationSamples == 0) {
          gyro.setXoffset(abs(xOffsetTemp) / CALIBRATION_SAMPLES);
          gyro.setYoffset(abs(yOffsetTemp) / CALIBRATION_SAMPLES);
          gyro.setZoffset(abs(zOffsetTemp) / CALIBRATION_SAMPLES);
          if (xOffsetTemp > 0) {
            gyro.setXoffset(-gyro.getXoffset());
          }
          if (yOffsetTemp > 0) {
            gyro.setYoffset(-gyro.getYoffset());
          }
          if (zOffsetTemp > 0) {
            gyro.setZoffset(-gyro.getZoffset());
          }
          itg3200CalibrationSamples--;
        } else {
          taskENTER_CRITICAL();
          gyro.getAngularVelocity(&aX, &aY, &aZ);
          previousTime = currentTime;
          currentTime = millis();                            // Current time actual time read
          elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds

          gyroAngleX = gyroAngleX + aX * elapsedTime;
          gyroAngleY = gyroAngleY + aY * elapsedTime;
          gyroAngleZ = gyroAngleZ + aZ * elapsedTime;
          taskEXIT_CRITICAL();
        }
      }
    }

    /* Wait for the next cycle. */
    vTaskDelay(1);
  }
}