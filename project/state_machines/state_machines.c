#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct State {
  char *name;
  int is_terminal;  // 1 if this is a terminating state; 0 otherwise
} State;

typedef struct Transition {
  char *name;
  struct State *origin;
  struct State *destination;
} Transition;

typedef struct StateMachine {
  struct State *current_state;

  int state_capacity;  // How many states can this state machine hold?
  int num_states;  // How many states does the state machine currently hold
  struct State **states;

  int transition_capacity;  // How many transitions can this state machine hold?
  int num_transitions;  // How many transitions does the state machine currently hold
  struct Transition **transitions;

} StateMachine;
/************************************
 *
 *   CREATE & DESTROY FUNCTIONS
 *
 ************************************/

/*****
 * Allocate memory for a new  state machine
 *
 * TODO: FILL THIS IN
 *****/
StateMachine *create_state_machine (int state_capacity, int transition_capacity) {
  // Allocate memory for state machine struct
  StateMachine *sm = malloc(sizeof(StateMachine));
  // Current state should default to NULL
  sm->current_state = NULL;
  // num_states and num_transitions should default to 0
  sm->num_states = 0;
  sm->num_transitions = 0;
  // Allocate memory for states
  sm->state_capacity = state_capacity;
  sm->states = calloc(state_capacity, sizeof(State *));
  // Allocate memory for transitions
  sm->transition_capacity = transition_capacity;
  sm->transitions = calloc(transition_capacity, sizeof(Transition *));

  return sm;

}
/*****
 * Allocate memory for a new state and initialize values
 *
 * TODO: FILL THIS IN
 *****/
State *create_state(char *name) {
  // Allocate memory for state struct
  State *state = malloc(sizeof(State));
  // Allocate memory and copy state name (hint: use strdup)
  state->name = strdup(name);
  // Set is_terminal to default of 0
  state->is_terminal = 0;

  return state;
}
/*****
 * Allocate memory for a new transition and initialize values
 *
 * TODO: FILL THIS IN
 *****/
Transition *create_transition(char *name, State *origin, State *destination){
  // Allocate memory for transition struct
  Transition *transition = malloc(sizeof(Transition));
  // Allocate memory and copy transition name (hint: use strdup)
  transition->name = strdup(name);
  // Set origin and destination states
  transition->origin = origin;
  transition->destination = destination;

  return transition;
}
/*****
 * Free memory for a state
 *
 * TODO: FILL THIS IN
 *****/
void destroy_state(State *state) {
  if(state != NULL){
    free(state->name);
    free(state);
  }
}
/*****
 * Free memory for a transition
 *
 * TODO: FILL THIS IN
 *****/
void destroy_transition(Transition *transition) {
  if(transition != NULL){
    free(transition->name);
    free(transition);
  }
}
/*****
 * Free memory for a state machine and all of its states and transitions
 *
 * TODO: FILL THIS IN
 *****/
void destroy_state_machine(StateMachine *sm) {

  // Free all transitions
  for(int i = 0; i < sm->transition_capacity; i++){
    destroy_transition(sm->transitions[i]);
  }
  // Free all states
  for(int j = 0; j < sm->state_capacity; j++){
    destroy_state(sm->states[j]);
  }
  // Free state machine
  free(sm->transitions);
  free(sm->states);
  free(sm);
}
/************************************
 *
 *   STATE MACHINE FUNCTIONS
 *
 ************************************/
/*****
 * Add a state to the state machine
 *
 * TODO: FILL THIS IN
 *****/
State *sm_add_state(StateMachine *sm, char *state_name) {
  // Return NULL and print an error if number of states is over capacity
  //if state_capacity is equal to num_states, then it is at capacity.
  if(sm->state_capacity == sm->num_states){
    printf("At Capacity");
    return NULL;
  }
  // Return NULL and print an error if state name is not unique
    //loop through all the states
    //compare the names
    //if not unique return name taken.
    //return NULL
    for(int i = 0; i < sm->num_states; i++){
      if(strcmp(sm->states[i]->name, state_name)== 0){
        printf("Name has been taken");
         return NULL;
      }
    }
  // Create a new state and add it to the state machine
    //create a new pointer called new that is create_state(state_name). 
    State *state = create_state(state_name);
    //the new state must be given a space in the array.
    //so by accessing states[sm->num_states] you will be able to place the state.
    sm->states[sm->num_states] = state;
    //you also need to increment the current state so that it will fit 
    //all of the new states that are added.
    sm->current_state++; 

  // Initialize the state machine's current state if it hasn't been set yet
  //if current_state is equal to NULL then change it to state.
    if(sm->current_state == NULL){
      sm->current_state = state; 
    }
  // Return the state
  return state;
}

/*****
 * Add a terminal state to the state machine
 * (terminal states will exit the state machine)
 *
 * TODO: FILL THIS IN
 *****/
State *sm_add_terminal_state(StateMachine *sm, char *state_name) {
  // Add a state to the state machine
  // HINT: you can do this via the sm_add_state() function
  State *state = sm_add_state(sm, state_name);
  // If the new state is valid, set is_terminal to 1
  if(state == NULL){
    state->is_terminal = 1; 
  }
  return state;
}


/*****
 * Add a transition to the state machine
 *
 * TODO: FILL THIS IN
 *****/
Transition *sm_add_transition(StateMachine *sm, char *transition_name,
                              char *origin_state_name, char *destination_state_name){

  // Return NULL and print an error if number of transitions is over capacity
  if(sm->transition_capacity == sm->num_transitions){
    printf("At Capacity");
    return NULL;
  }
  // Declare origin_state and destination_state
  State *origin = create_state(origin_state_name);
  State *destination = create_state(destination_state_name); 
  // Search the state machine for states with matching names for both origin and destination 
  for(int i = 0; i <sm->num_states; i++){
    if(strcmp(origin_state_name ,sm->states[i]->name)== 0){
      origin = sm->states[i];
    }
    if(strcmp(destination_state_name, sm->states[i]->name)== 0){
      destination = sm->states[i];
    }
  }
  if(origin && destination){
      // If both origin and destination states have been found,
      // Create a new transition and add it to the state machine
    Transition *transition = create_transition(transition_name, origin, destination);
    sm->transitions[sm->num_transitions] = transition;
    sm->num_transitions++;
  }else{
    // Otherwise, print an error and return NULL
    printf("Error");
  }
  return NULL;
}
/*****
 * Execute the transition that matches the transition_name
 * then update the state machine's current_state accordingly
 *
 * TODO: FILL THIS IN
 *****/
State *sm_do_transition(StateMachine *sm, char *transition_name) {
  Transition *transition;
  // Search the state machine for a valid transition:
  // The transition's origin state should match the state machine's current_state
  // and the transition's name should match the given name

  for(int i = 0; i < sm->num_transitions; i++){
    //looping through to gather the transitions.
    if(strcmp(transition_name, sm->transitions[i]->name)){
      //if there isstr comparison in each transition name.
      if(strcmp(sm->transitions[i]->origin->name, sm->current_state->name)){
        //move onto if there is comparison between tranisition origin name or the current state name.
        transition = sm->transitions[i];
        //then transition will assume the value of transition[i].
      }
    }
  }
 
  // If a valid transition is found, update the state machine's current state
  if(transition){
    sm->current_state = transition->destination;
    return sm->current_state;
  }
  // If a valid transition is not found, print an error and return NULL;
  else{
    printf("Error");
    return NULL;
  }
}


/************************************
 *
 *   UTILITY FUNCTIONS FOR INPUT AND TESTING
 *
 ************************************/

/*****
 * Utility function to print the current states and its transitions
 *****/
void sm_print_state_and_transitions(StateMachine *sm) {
  printf("---------\n\nCurrent State: %s\n", sm->current_state->name);
  if (sm->current_state->is_terminal) {
    printf("Terminating");
  } else {
    printf("\nTransitions:\n");
    for (int i = 0 ; i < sm->transition_capacity ; i++) {
      if (strcmp(sm->transitions[i]->origin->name, sm->current_state->name) == 0) {
        printf("%s -> %s\n", sm->transitions[i]->name, sm->transitions[i]->destination->name);
      }
    }
  }
  printf("\n");
}

/***
 * Utility function to accept user inputs for state transitions
 *****/
void process_input(StateMachine *sm) {

  int max = 128;
  char *input_buffer = malloc(max * sizeof(char));

  while (1) {
    printf("Enter a transition\n~> ");

    fgets(input_buffer, max, stdin);
    for (int i = 0 ; input_buffer[i] != 0 ; i++) {
      if (input_buffer[i] == '\r' || input_buffer[i] == '\n') input_buffer[i] = 0;
    }

    sm_do_transition(sm, input_buffer);
    sm_print_state_and_transitions(sm);

    if (sm->current_state->is_terminal) {
      break;
    }
  }

  free(input_buffer);

}



#ifndef TESTING
int main(void)
{

  StateMachine *sm = create_state_machine(10, 15);

  sm_add_state(sm, "START");
  sm_add_state(sm, "5");
  sm_add_state(sm, "10");
  sm_add_state(sm, "15");
  sm_add_state(sm, "20");
  sm_add_terminal_state(sm, "SODA");
  sm_add_terminal_state(sm, "SODA + NICKEL");
  sm_add_terminal_state(sm, "SODA + DIME");
  sm_add_terminal_state(sm, "SODA + NICKEL + DIME");
  sm_add_terminal_state(sm, "SODA + DIME + DIME");

  sm_add_transition(sm, "NICKEL", "START", "5");
  sm_add_transition(sm, "DIME", "START", "10");
  sm_add_transition(sm, "QUARTER", "START", "SODA");
  sm_add_transition(sm, "NICKEL", "5", "10");
  sm_add_transition(sm, "DIME", "5", "15");
  sm_add_transition(sm, "QUARTER", "5", "SODA + NICKEL");
  sm_add_transition(sm, "NICKEL", "10", "15");
  sm_add_transition(sm, "DIME", "10", "20");
  sm_add_transition(sm, "QUARTER", "10", "SODA + DIME");
  sm_add_transition(sm, "NICKEL", "15", "20");
  sm_add_transition(sm, "DIME", "15", "SODA");
  sm_add_transition(sm, "QUARTER", "15", "SODA + NICKEL + DIME");
  sm_add_transition(sm, "NICKEL", "20", "SODA");
  sm_add_transition(sm, "DIME", "20", "SODA + NICKEL");
  sm_add_transition(sm, "QUARTER", "20", "SODA + DIME + DIME");

  sm_print_state_and_transitions(sm);

  process_input(sm);

  destroy_state_machine(sm);


  return 0;
}
#endif
