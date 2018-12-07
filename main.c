/*********************************************************************
*
*   MODULE:
*       main.c
*
*   Copyright 2018 by Matthew Cooper Healy <mhealy@mst.edu>
*
*********************************************************************/

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/

#include "tinny_tim.h"

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/


/* string constants to avoid repetition */
const char prompt[27] = "\nEnter No of Iterations: ";
const char h_bar[74] =
  "+--------+--------+--------+--------+--------+--------+--------+--------+";

/* Starting location */
const int START[2] = { 5, 5 };

/* GAMMA */
const float GAMMA = 0.8f;

const int reward[10][10] =          /* map of rewards per location  */
{ { WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL },
  { WALL,    0,    0,    0,    0,    0,    0,    0,    0, WALL },
  { WALL,    0,    0,    0,    0, FIRE, WALL,    0,    0, WALL },
  { WALL,    0,  ONI, WALL,    0,    0, WALL,    0,    0, WALL },
  { WALL,    0,    0, WALL,    0,    0,    0,    0,    0, WALL },
  { WALL,    0,    0, WALL, WALL, WALL,    0, DNUT,    0, WALL },
  { WALL,    0,    0,    0,    0, WALL,    0,    0,    0, WALL },
  { WALL,    0,    0, CAKE,    0,    0,    0,    0,    0, WALL },
  { WALL,    0,    0,    0,    0,    0,    0,    0,    0, WALL },
  { WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL } };

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

float q_table[10][10][4] = { 0 };   /* Table of Q(s,a)              */
float q_table_prime[10][10][4] = { 0 };
                                    /* Table to contain new q values*/
char policy[10][10];                /* Table to contain strings to
                                      represent policy and rewards  */

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

/*********************************************************************
 * PROCEDURE NAME:
 *     main()
 * 
 * DESCRIPTION:
 *     Main entry point into program. Continually asks for user input
 *     to determine the number of value iterations to perform, and
 *     outputs values after that many iterations. If a number less
 *     than zero is entered, the optimal policy is displayed and the
 *     program exits.
 ********************************************************************/
int main
  (
  void
  )
{
  /*----------------------------------------------------------
  Local variables
  ----------------------------------------------------------*/
  int count;
  int n;

  /*----------------------------------------------------------
  Output program information
  ----------------------------------------------------------*/
  printf(
    "CS-5001: HW#2\n"
    "Programmer: Matthew Healy <mhrh3>\n"
    "Discount GAMMA = %3.1f\n",
    GAMMA);

  /*----------------------------------------------------------
  Prompt user input and perform value iterations
  ----------------------------------------------------------*/
  printf(prompt);
  scanf("%d", &n);
  while(n > 0) {
    value_iterate(n);
    count = count + n;
    printf("\nValues after %d iterations:\n\n", count);
    print_values();
    printf(prompt);
    scanf("%d", &n);
  }
  print_policy();
  return 0;
}   /* main() */

/************************************
* HELPER FUNCTIONS
************************************/

/*********************************************************************
 * PROCEDURE:
 *     get_probs()
 *
 * DESCRIPTION:
 *     Loads passed probabilities with the likelihood that Tinny Tim
 *     will move in that direction, given the passed action. An error
 *     is output if an illegal action is passed.
 ********************************************************************/
void get_probs
  (
  float * p_up,                     /* probability for moving up    */
  float * p_down,                   /* probability for moving down  */
  float * p_left,                   /* probability for moving left  */
  float * p_right,                  /* probability for moving right */
  int a                             /* action taken by Tinny Tim    */
  )
{
  /* For each action, moving in the given direction has a probability
      of 85%, moving in the opposite direction has a probability
      of 0%, and moving in an orthogonal direction has a probability
      of 9%                                                         */
  switch(a) {
    case UP:
      *p_up    = 0.85f;
      *p_down  = 0.0f;
      *p_left  = 0.09f;
      *p_right = 0.09f;
      break;

    case LEFT:
      *p_up    = 0.09f;
      *p_down  = 0.09f;
      *p_left  = 0.85f;
      *p_right = 0.0f;
      break;

    case RIGHT:
      *p_up    = 0.09f;
      *p_down  = 0.09f;
      *p_left  = 0.0f;
      *p_right = 0.85f;
      break;

    case DOWN:
      *p_up    = 0.0f;
      *p_down  = 0.85f;
      *p_left  = 0.09f;
      *p_right = 0.09f;
      break;

    default:                        /* Should be unreachable. Error
                                        should be logged.           */
      printf("ILLEGAL ACTION REACHED!\n");
      break;
  }
  return;
}   /* get_probs() */


/*********************************************************************
 * PROCEDURE NAME:
 *     print_policy()
 *
 * DESCRIPTION:
 *     Prints a grid of string representations showing the optimal
 *     policy per location, or its explicit reward.
 ********************************************************************/
void print_policy
  (
  void
  )
{
  printf(h_bar);
  printf("\n");
  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x < 9; x++) {
      printf("|");
      if (reward[x][y] == 0) {
        printf("    %c   ", policy[x][y]);
      } else {
        switch(reward[x][y]) {
          case WALL:
            printf(" ###### ");
            break;
          case DNUT:
            printf(" DUNUT  ");
            break;
          case ONI:
            printf("   ONI  ");
            break;
          case FIRE:
            printf("  FIRE  ");
            break;
          case CAKE:
            printf("  CAKE  ");
            break;
          default:
            printf(" ?????? ");
            break;
        }
      }
    }
    printf("|\n");
  }
  printf(h_bar);
  printf("\n");
}   /* print_policy() */

/*********************************************************************
 * PROCEDURE NAME:
 *     print_values()
 *
 * DESCRIPTION:
 *     Prints a grid of values corresponding to the V(s) for each
 *     location on the map that can be reached by Tinny Tim. Spaces
 *     with an explicit reward are printed as string representations
 *     of what occupies them.
 ********************************************************************/
void print_values
  (
  void
  )
{
  printf(h_bar);
  printf("\n");
  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x < 9; x++) {
      printf("|");
      if (reward[x][y] == 0) {
        printf(" %6.3f ", value(x, y));
      } else {
        switch(reward[x][y]) {
          case WALL:
            printf(" ###### ");
            break;
          case DNUT:
            printf("  DUNUT ");
            break;
          case ONI:
            printf("   ONI  ");
            break;
          case FIRE:
            printf("  FIRE  ");
            break;
          case CAKE:
            printf("  CAKE  ");
            break;
          default:
            printf(" ?????? ");
            break;
        }
      }
    }
    printf("|\n");
  }
  printf(h_bar);
  printf("\n");
}   /* print_values() */

/*********************************************************************
 * PROCEDURE NAME:
 *     value()
 *
 * DESCRIPTION:
 *     Returns V(s), where V(s) = MAX_{a}(Q(s,a)). While determining
 *     the max value of Q(s,a), the global policy table is loaded
 *     with the current optimal policy.
 ********************************************************************/
float value
  (
  int x,                            /* x-coordinate of position     */
  int y                             /* y-coordinate of position     */
  )
{
  float max = 0.0f;
  if (q_table[x][y][UP] > max) {
    max = q_table[x][y][UP];
    policy[x][y] = '^';
  }
  if (q_table[x][y][DOWN] > max) {
    max = q_table[x][y][DOWN];
    policy[x][y] = 'V';
  }
  if (q_table[x][y][LEFT] > max) {
    max = q_table[x][y][LEFT];
    policy[x][y] = '<';
  }
  if (q_table[x][y][RIGHT] > max) {
    max = q_table[x][y][RIGHT];
    policy[x][y] = '>';
  }
  return max;
}   /* value() */

float exp_reward
  (
  int x,                            /* x-coordinate of position     */
  int y,                            /* y-coordinate of position     */
  int a                             /* action taken by Tinny Tim    */
  )
{
  float p_up, p_down, p_left, p_right;
  float res = 0.0f;

  get_probs(&p_up, &p_down, &p_left, &p_right, a);
  res += (p_up    * reward[x][y + 1]);
  res += (p_down  * reward[x][y - 1]);
  res += (p_left  * reward[x - 1][y]);
  res += (p_right * reward[x + 1][y]);

  return res;
}   /* value() */

/*********************************************************************
 * PROCEDURE NAME:
 *     value_iterate()
 *
 * DESCRIPTION:
 *     Iterates Q-Tables a number of times equal to the passed value.
 *     iteration follows the formula:
 *     Q_{k+1}(s, a) = R(s,a) + GAMMA * SUM_{s'}[P(s'|s,a) * V_{k}(s)]
 *     where
 *         V_{k}(s) = MAX_{a}(Q_{k}(s,a))
 *         R(s,a) = SUM_{s'}[P(s'|s,a) * R(s,a,s')]
 *     NOTE: R(s,a,s') is equal to the reward given for passing from
 *     state s to state s' via action a.
 ********************************************************************/
void value_iterate
  (
  int n                             /* number of times to iterate   */
  )
{
  float val_tmp;
  float p_up;
  float p_down;
  float p_left;
  float p_right;
  for (int i = 0; i < n; i++) {
    for (int x = 1; x < 9; x++) {     // For every possible
      for (int y = 1; y < 9; y++) {   // location <x,y> and
        for (int a = 0; a < 4; a++) { // action a
          val_tmp = 0.0f;
          get_probs(&p_up, &p_down, &p_left, &p_right, a);
          val_tmp += p_right * value(x + 1, y    );
          val_tmp += p_left  * value(x - 1, y    );
          val_tmp += p_up    * value(x,     y + 1);
          val_tmp += p_down  * value(x,     y -1 );
          q_table_prime[x][y][a] = exp_reward(x, y, a) + (GAMMA * val_tmp);
        }
      }
    }
    q_copy(q_table, q_table_prime);
  }
}   /* value_iterate() */

/*********************************************************************
 * PROCEDURE NAME:
 *     q_copy()
 *
 * DESCRIPTION:
 *     Copies all values from the source Q table into the destination
 *     Q table. Q tables are required to be 10x10x4.
 ********************************************************************/
void q_copy
  (
  float dest[10][10][4],            /* q-table to write to          */
  float src[10][10][4]              /* q-table to read from         */
  )
{
  for (int x = 0; x < 10; x++) {
    for (int y = 0; y < 10; y++) {
      for (int a = 0; a < 4; a++) {
        dest[x][y][a] = src[x][y][a];
      }
    }
  }
  return;
}   /* q_copy() */
