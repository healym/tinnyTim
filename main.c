#include <stdio.h>

#define MAX(a,b) (((a)>(b))?(a):(b))
#define BOOLEAN int
#define FALSE 0
#define TRUE 1

/* string constants to avoid repetition */
const char prompt[25] = "\nEnter No of Iterations: ";
const char h_bar[74] =
    "+--------+--------+--------+--------+--------+--------+--------+--------+";

/* Starting location */
const int START[2] = { 5, 5 };

/* GAMMA */
const float GAMMA = 0.8f;

/* Rewards for cells in grid */
#define DNUT 3
#define CAKE 10
#define WALL -1
#define FIRE -5
#define ONI -10

/* Directions that Tinny Tim can move */
enum DIRECTIONS {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

float q_table[10][10][4] = { 0 };/* Table of Q(s,a)                 */
float q_table_prime[10][10][4] = { 0 };
                                 /* Table to contain new q values   */
char policy[10][10];             /* Table to contain strings to
                                      represent policy and rewards  */


float exp_reward(int x, int y, int a);
void get_probs(float * p_up, float * p_down, float * p_left, float * p_right, int a);
void init_policy();
void print_policy();
void print_values();
float value(int x, int y);
void value_iterate(int n);
void q_copy(float dest[10][10][4], float src[10][10][4]);

const int reward[10][10] =
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

/**************************************
 * PROCEDURE: main()
 *************************************/
int main
  (
  void
  )
{
  int count;
  int n;

  printf(
    "CS-5001: HW#2\nProgrammer: Matthew Healy <mhrh3>\nDiscount GAMMA = %3.1f\n",
    GAMMA);
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
} /* main() */

/*************************************************
 * PROCEDURE: value_iterate()
 * DESCRIPTION: Iterates Q-Tables given number
 *               of times
 ************************************************/
void value_iterate
  (
  int n                           /* number of times to iterate     */
  )
{
  float val_tmp;
  for (int i = 0; i < n; i++) {
    for (int x = 1; x < 9; x++) {    // For every possible
      for (int y = 1; y < 9; y++) {  // location <x,y> and
        for (int a = 0; a < 4; a++) { // action a
          val_tmp = 0.0f;
          val_tmp += prob( 1,  0, a) * value(x + 1, y    );
          val_tmp += prob(-1,  0, a) * value(x - 1, y    );
          val_tmp += prob( 0,  1, a) * value(x,     y + 1);
          val_tmp += prob( 0, -1, a) * value(x,     y -1 );
          q_table_prime[x][y][a] = exp_reward(x, y, a) + (GAMMA * val_tmp);
        }
      }
    }
    q_copy(q_table, q_table_prime);
  }
} /* value_iterate() */

/*************************************************
 * PROCEDURE: value()
 * DESCRIPTION: Returns maximum value of
 *               Q(<x,y>, a) for every a
 ************************************************/
float value
  (
  int x,                          /* x-coordinate of position       */
  int y                           /* y-coordinate of position       */
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
} /* value() */

float exp_reward(int x, int y, int a) {
  float p_up, p_down, p_left, p_right;
  float res = 0.0f;

  get_probs(&p_up, &p_down, &p_left, &p_right, a);
  res += (p_up    * reward[x][y + 1]);
  res += (p_down  * reward[x][y - 1]);
  res += (p_left  * reward[x - 1][y]);
  res += (p_right * reward[x + 1][y]);

  return res;
} /* value() */

/************************************
* HELPER FUNCTIONS
************************************/

/*************************************************
 * PROCEDURE: get_probs()
 * DESCRIPTION: 
 ************************************************/
void get_probs
  (
  float * p_up,                   /* probability for moving up      */
  float * p_down,                 /* probability for moving down    */
  float * p_left,                 /* probability for moving left    */
  float * p_right,                /* probability for moving right   */
  int a                           /* action taken by Tinny Tim      */
  )
{
  /* For each action, moving in the given direction has a probability
      of 85%, moving in the opposite direction has a probability
      of 0%, and moving in an orthogonal direction has a probability
      of 9%                                                          */
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

    default:
      /* Should be unreachable. Error should be logged. */
      printf("ILLEGAL ACTION REACHED!\n");
      break;
  }
  return;
} /* get_probs() */

/*************************************************
 * PROCEDURE: 
 * DESCRIPTION: 
 ************************************************/
void print_values()
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
} /* print_values() */


/*************************************************
 * PROCEDURE: 
 * DESCRIPTION: 
 ************************************************/
void print_policy()
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
} /* print_policy() */

/*************************************************
 * PROCEDURE: 
 * DESCRIPTION: 
 ************************************************/
void q_copy
  (
  float dest[10][10][4],          /* q-table to write to            */
  float src[10][10][4]            /* q-table to read from           */
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
} /* q_copy() */