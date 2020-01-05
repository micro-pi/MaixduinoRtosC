#include <devices.h>
#include <portmacro.h>
#include <task.h>
#include <utils.h>

static size_t timer2Millis;
static handle_t timer2;

static void onTick(void *arg);

void initMillis(void) {
  timer2 = io_open("/dev/timer2");
  timer_set_interval(timer2, 1000000L);
  timer_set_on_tick(timer2, &onTick, NULL);
  timer_set_enable(timer2, true);
}

size_t millis(void) {
  return timer2Millis;
}

static void onTick(void *arg) {
  timer2Millis += 1U;
}