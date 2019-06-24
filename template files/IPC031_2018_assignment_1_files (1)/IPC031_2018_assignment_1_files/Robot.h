#include "gui_kernel.h"

/*
	Robot is a library to draw and manipulate a simple robot in a window.
	It provides the class ROBOT. Implementation is built on the gui_kernel.
	First version created in may 2000 by Sjaak Smetsers and Pieter Koopman, Radboud University.
	Last Modified: June 15 2018, by Peter Achten, Radboud University.
*/

// these are the Charles actions that have to be used in the assignments
extern void step () ;
extern void turn_left () ;
extern void turn_right () ;
extern bool on_ball () ;
extern void get_ball () ;
extern void put_ball () ;
extern bool in_front_of_wall () ;
extern bool north () ;

const int SIZE_OF_STONE	= 6;
const int SIZE_OF_BALL	= 11;
const int SIZE_OF_ROBOT	= 18;
const int SIZE_OF_EDGE	= 12;
const int WORLDWIDTH    = 50;
const int WORLDHEIGHT	= 30;

const int SCREENWIDTH	= WORLDWIDTH  * 2 * SIZE_OF_STONE + SIZE_OF_EDGE;
const int SCREENHEIGHT	= WORLDHEIGHT * 2 * SIZE_OF_STONE + SIZE_OF_EDGE;

const int FAST_STEP_TIME      = 1;
const int INITIAL_STEP_TIME   = 60;
const int SLOW_STEP_TIME      = 250;
const int VERY_SLOW_STEP_TIME = 1000;

class RobotGUI : public GUI
{
public:
	virtual void Window (const RECT& area);
	virtual void Timer (const int dt);
	RobotGUI();
};

extern RobotGUI& charles_world();

enum IllegalActions { IA_Charles, IA_Step, IA_GetBall, IA_PutBall, IA_Open, IA_TooManySteps} ;

class IllegalAction
{
public:
	IllegalAction (IllegalActions ia) : _action (ia) {}
	void report () ;
private:
	IllegalActions _action;
} ;

// These operations are for internal use only.
// With them Charles world can be changed
// Each function immediately renders the new state of Charles' world
extern void place_rectangle (int, int, int, int) ;
extern void place_walls (int, int, int, bool) ;
extern void place_balls (int, int, int, bool) ;
extern void make_labyrinth () ;
extern void create_ball (int x, int y) ;

// These operations are for internal use only.
// With them you control the operational behavior of Charles.
extern void reset ();
extern void clear_world () ;
extern void start_replay_for_timer ();
extern void clear_actions ();
extern void store_world ();
extern void retrieve_world ();
extern void rest (int n) ;
extern void new_world (const char world []) ;
extern void stop ();
