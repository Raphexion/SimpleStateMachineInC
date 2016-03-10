#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define SUCCESS true
#define FAILURE false

/**
 * Define the states
 */
typedef enum {
  ERROR,
  DOOR_IS_CLOSED,
  DOOR_IS_OPEN,
  NB_STATES
} state_t;

/**
 * Define the events
 */
typedef enum {
  OPEN_DOOR,
  CLOSE_DOOR,
  NB_EVENTS
} event_t;

/**
 * Define the "look/feel" of what an action is.
 */
typedef bool (* action_t) (void);

/**
 * Known actions
 */
static bool nothing(void)
{
  return SUCCESS;
}

static bool open_the_door(void)
{
  fprintf(stdout, "Let's open the door\n");
  return SUCCESS;
}

static bool close_the_door(void)
{
  fprintf(stdout, "Let's close the door\n");
  return SUCCESS;
}

/**
 * Define the actions to take
 * STATE x EVENT -> ACTION
 */
static const action_t actions[NB_STATES][NB_EVENTS] = {
  [ERROR] = {
    [OPEN_DOOR] = nothing,
    [CLOSE_DOOR] = nothing,
  },
  [DOOR_IS_CLOSED] = {
    [OPEN_DOOR] = open_the_door,
    [CLOSE_DOOR] = nothing,
  },
  [DOOR_IS_OPEN] = {
    [OPEN_DOOR] = nothing,
    [CLOSE_DOOR] = close_the_door
  }
};

/**
 * Define transitions between states
 *
 * STATE x EVENT -> STATE
 */
static const state_t transitions[NB_STATES][NB_EVENTS] = {
  [ERROR] = {
    [OPEN_DOOR] = ERROR,
    [CLOSE_DOOR] = ERROR,
  },
  [DOOR_IS_CLOSED] = {
    [OPEN_DOOR] = DOOR_IS_OPEN,
    [CLOSE_DOOR] = DOOR_IS_CLOSED,
  },
  [DOOR_IS_OPEN] = {
    [OPEN_DOOR] = DOOR_IS_OPEN,
    [CLOSE_DOOR] = DOOR_IS_CLOSED
  }
};

/**
 * State-machine execution
 */
static void state_machine(event_t event)
{
  static state_t current_state = DOOR_IS_CLOSED;

  const action_t action = actions[current_state][event];

  const bool status = action ? action() : FAILURE;
  const state_t new_state = (status == SUCCESS) ? transitions[current_state][event] : ERROR;

  // TODO: possibility to do pre/post state actions

  current_state = new_state;
}

/**
 * Main function
 */
int main(__attribute__((unused)) int argc, __attribute__((unused)) char *argv[])
{
  event_t events[] = {
    CLOSE_DOOR,
    CLOSE_DOOR,
    OPEN_DOOR,
    OPEN_DOOR,
    CLOSE_DOOR
  };

  for (size_t ii = 0; ii < sizeof(events)/sizeof(event_t); ii++) {
    state_machine(events[ii]);
  }

  return 0;
}
