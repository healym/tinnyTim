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

/* end rewards */

/* Directions that Tinny Tim can move */
enum DIRECTIONS {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

float q_table[10][10][4] = { 0 };  /* Table of Q(s,a) values to be updated */
float q_table_prime[10][10][4] = { 0 };
                                  /* Table to contain new q values */
char policy[10][10];              /* Table to contain strings to represent
                                      policy and rewards                  */

void init_policy();
void print_values();
void print_policy();
float max_value(int x, int y);
void run_iterations(int n);
float er(int x, int y, int a);
void q_copy(float dest[10][10][4], float src[10][10][4]);
void get_probs(float * p_up, float * p_down, float * p_left, float * p_right, int a);

int main() {
  printf(
    "CS-5001: HW#2\nProgrammer: Matthew Healy <mhrh3>\nDiscount GAMMA = %3.1f\n",
    GAMMA);
  init_policy();
  int n = 0;
  int count = 0;
  do {
    printf(prompt);
    scanf("%d", &n);
    if (n > 0) {
      count += n;
      printf("\nValues after %d iterations:\n\n", count);
      run_iterations(n);
      print_values();
    }
  } while (n > 0);
  printf("\nPolicy:\n\n");
  print_policy();
  return 0;
}

void run_iterations(int n) {
  float temp_val;
  float p_up, p_down, p_left, p_right;
  for(int i = 0; i < n; i++) {
    for (int x = 1; x < 9; x++) { /* all steppable xs */
      for (int y = 1; y < 9; y++) { /* all stappable ys */
        if (reward[x][y] == 0) { /* only consider valid locations */
          for (int a = 0; a < 4; a++) {
            temp_val = 0;
            get_probs(&p_up, &p_down, &p_left, &p_right, a);
            temp_val +=   (p_up    * max_value(x, (y + 1)))
                        + (p_down  * max_value(x, (y - 1)))
                        + (p_left  * max_value((x - 1), y))
                        + (p_right * max_value((x + 1), y));

            q_table_prime[x][y][a] = er(x, y, a) + (GAMMA * temp_val);
          } /* end action iteration */
        } /* end if */
      } /* end y iteration */
    } /* end x iteration */
    q_copy(q_table, q_table_prime); // copy new table into old table
  } /* end iterations */
  return;
}

float er(int x, int y, int a) {
  float p_up, p_down, p_left, p_right;
  float res = 0.0f;

  for (int a = 0; a < 4; a++) {
    get_probs(&p_up, &p_down, &p_left, &p_right, a);
    res += (p_up    * reward[x][y + 1])
         + (p_down  * reward[x][y - 1])
         + (p_left  * reward[x - 1][y])
         + (p_right * reward[x + 1][y]);
  }

  return res;
}


/***********************************************
 * HELPER FUNCTIONS
 ***********************************************/

void get_probs(float * p_up, float * p_down, float * p_left, float * p_right, int a) {
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
      printf("ILLEGAL ACTION REACHED!\n");
      break;
  }
  return;
}

void init_policy() {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      policy[i][j] = ' ';
    }
  }
}


void print_values() {
  printf(h_bar);
  printf("\n");
  for (int y = 8; y >= 1; y--) {
    for (int x = 1; x < 9; x++) {
      printf("|");
      if (reward[x][y] == 0) {
        printf(" %6.3f ", max_value(x, y));
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


void print_policy() {
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

float max_value(int x, int y) {
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
} /* max_value() */

void q_copy(float dest[10][10][4], float src[10][10][4]) {
  for (int x = 0; x < 10; x++) {
    for (int y = 0; y < 10; y++) {
      for (int a = 0; a < 4; a++) {
        dest[x][y][a] = src[x][y][a];
      }
    }
  }
  return;
}
