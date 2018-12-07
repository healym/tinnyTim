/*********************************************************************
*
*   HEADER:
*       tinny_tim.h - Definitions for Tinny Tim simulation
*
*   Copyright 2018 by Matthew Cooper Healy <mhealy@mst.edu>
*
*********************************************************************/

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/

#include <stdio.h>

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

#define CAKE  10
#define DNUT   5
#define WALL  -1
#define FIRE  -5
#define ONI  -10

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/

enum DIRECTION
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

#define MAX(a,b) ((a)>(b)?(a):(b))

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

void get_probs
  (
  float * p_up,                     /* probability for moving up    */
  float * p_down,                   /* probability for moving down  */
  float * p_left,                   /* probability for moving left  */
  float * p_right,                  /* probability for moving right */
  int a                             /* action taken by Tinny Tim    */
  );

void print_policy
  (
  void
  );

  void print_values
  (
  void
  );

  float value
  (
  int x,                            /* x-coordinate of position     */
  int y                             /* y-coordinate of position     */
  );

  float exp_reward
  (
  int x,                            /* x-coordinate of position     */
  int y,                            /* y-coordinate of position     */
  int a                             /* action taken by Tinny Tim    */
  );

  void value_iterate
  (
  int n                             /* number of times to iterate   */
  );

  void q_copy
  (
  float dest[10][10][4],            /* q-table to write to          */
  float src[10][10][4]              /* q-table to read from         */
  );