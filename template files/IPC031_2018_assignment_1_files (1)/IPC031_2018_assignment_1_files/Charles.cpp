#include "Robot.h"
/*
	Charles is a framework for manipulating Charles the robot.
	Charles is a subclass of ROBOT from the library Robot.
	Original version created by Sjaak Smetsers and Pieter Koopman, Radboud University.
	Last Modified: August 31 2018, by Peter Achten, Radboud University.
*/

// =========================================================================

// Here are a number of auxiliary functions. You do not need to study or use them.

// put_a_ball_somewhere adds a single ball to Charles' world on a random location
void put_a_ball_somewhere ()
{
    const int LANE   = 1 + rand () % (WORLDHEIGHT - 2) ;
    const int STREET = 1 + rand () % (WORLDWIDTH  - 2) ;
    create_ball (STREET, LANE) ;
}

// steps (no_of_steps) makes Charles move no_of_steps steps forward (without checking if he hits a wall)
void steps ( int no_of_steps )
{
    while (no_of_steps > 0)
    {
        step () ;
        no_of_steps-- ;
    }
}

// make_string_with_balls places a ball besides each wall in an initial Charles world
void make_string_with_balls ()
{
    place_balls (1,1,WORLDHEIGHT-3,false) ;
    place_balls (WORLDWIDTH-2,1,WORLDHEIGHT-3,false) ;
    place_balls (1,1,WORLDWIDTH-3,true) ;
    place_balls (1,WORLDHEIGHT-2,WORLDWIDTH-3,true) ;
}

// make chaos_with_balls creates a 'pile' of balls at the north east side of Charles world
void make_chaos_with_balls ()
{
    const int BOTTOM_LANE = rand () % (WORLDHEIGHT / 2) + 2 ;
    for (int lane = WORLDHEIGHT-2; lane > BOTTOM_LANE; lane--)
    {
        const int NO_OF_BALLS = rand () % (WORLDWIDTH/2) + 3 ;
        place_balls (WORLDWIDTH-NO_OF_BALLS-2,lane,NO_OF_BALLS,true) ;
    }
}

// make_block creates an arbitrary block inside Charles' world and places a ball
// on the 'north' most lane at the 'west' most street corner.
void make_block ()
{
    const int LEFT   = rand () % (WORLDWIDTH  / 3) + 5 ;
    const int BOTTOM = rand () % (WORLDHEIGHT / 4) + 5 ;
    const int WIDTH  = rand () % (WORLDWIDTH  / 2) + 5 ;
    const int HEIGHT = rand () % (WORLDHEIGHT / 2) + 2 ;
    place_rectangle (LEFT, BOTTOM, WIDTH, HEIGHT) ;
    create_ball (LEFT, WORLDHEIGHT - 2) ;
}

// =========================================================================

// Here are the examples that have been explained during the lecture. You can study them.
void swap_ball ()
{
    if (on_ball ())
        get_ball () ;
    else
	    put_ball () ;
    step () ;
}

// turn_north makes Charles look north
void turn_north ()
{
    while (!north())
        turn_left();
}

// turn_around makes Charles turn '180 degrees'
void turn_around ()
{
    turn_left() ;
    turn_left() ;
}

// walk_to_wall makes Charles walk ahead and stop immediately before the wall
void walk_to_wall ()
{
    while (!in_front_of_wall())
        step () ;
}

// sweep_lane makes Charles walk ahead until it is on top of a ball or immediately before a wall
void sweep_lane ()
{
    while (!on_ball() && !in_front_of_wall())
        step () ;
}

// go_down_east_if_not_on_ball moves Charles one lane to the south if he is not on a ball.
// it is assumed that Charles is facing east and is not on the southernmost lane
void go_down_east_if_not_on_ball ()
{
    if (!on_ball())
    {
        turn_right() ;
        step() ;
        turn_right() ;
    }
}

// go_down_west_if_not_on_ball moves Charles one lane to the south if he is not on a ball.
// it is assumed that Charles is facing west and is not on the southernmost lane
void go_down_west_if_not_on_ball ()
{
    if (!on_ball())
    {
        turn_left () ;
        step () ;
        turn_left () ;
    }
}

// sweep_lanes makes Charles move to the east and find the ball, followed by
// going on lane south and moving to the west to find the ball.
// Assumptions: Charles is standing at the north-west corner and is looking east, there are only the exterior walls, and a ball is present.
// Result:      Charles is standing on the ball
void sweep_lanes ()
{
    while (!on_ball())
    {
        sweep_lane () ;
        go_down_east_if_not_on_ball () ;
        sweep_lane () ;
        go_down_west_if_not_on_ball () ;
    }
}

// This example has been discussed during the lecture
// The effect of this function is that Charles will search for a ball in its universe.
// Assumptions: Charles is standing on its base position, there are only the exterior walls, and a ball is present.
// Result:      Charles is standing on the ball.
void find_ball ()
{
    sweep_lanes () ;
}

// =========================================================================

// here starts the part with code to be completed by yourself

// Name / student number / study student 1 :
// Name / student number / study student 2 :

// give one or more comment lines about what will happen in this function
void clean_string_with_balls ()
{
    // enter your Charles code here
}

// give one or more comment lines about what will happen in this function
void clean_chaos_with_balls ()
{
    // enter your Charles code here
}

// give one or more comment lines about what will happen in this function
void around_the_block ()
{
    // enter your Charles code here
}

// For testing purposes, you can define your own function here:
void test ()
{
    // enter your Charles code here
}

// end of part with code to be completed by yourself

// =========================================================================

// The remainder of this program only sets up the correct infrastructure. You do not need to study or use it.

void quick  () { rest(FAST_STEP_TIME); };
void normal () { rest(INITIAL_STEP_TIME); };
void slow   () { rest(SLOW_STEP_TIME); };
void very_slow () { rest(VERY_SLOW_STEP_TIME); };

void start_step ()
{
    store_world () ;
    try {step (); }
    catch (...) {}
    retrieve_world () ;
    start_replay_for_timer () ;
    charles_world().startTimer(FAST_STEP_TIME) ;
}

void start_turn_left ()
{
    store_world () ;
    try {turn_left (); }
    catch (...) {}
    retrieve_world () ;
    start_replay_for_timer () ;
    charles_world().startTimer(FAST_STEP_TIME) ;
}

void start_turn_right ()
{
    store_world () ;
    try {turn_right (); }
    catch (...) {}
    retrieve_world () ;
    start_replay_for_timer () ;
    charles_world().startTimer(FAST_STEP_TIME) ;
}

void start_put_ball ()
{
    store_world () ;
    try {put_ball (); }
    catch (...) {}
    retrieve_world () ;
    start_replay_for_timer () ;
    charles_world().startTimer(FAST_STEP_TIME) ;
}

void start_get_ball ()
{
    store_world () ;
    try {get_ball (); }
    catch (...) {}
    retrieve_world () ;
    start_replay_for_timer () ;
    charles_world().startTimer(FAST_STEP_TIME) ;
}

void start_swap_ball ()
{
    store_world () ;
    try {swap_ball (); }
    catch (...) {}
    retrieve_world () ;
    start_replay_for_timer () ;
    charles_world().startTimer(FAST_STEP_TIME) ;
}

void start_search_the_ball ()
{
    reset () ;
    put_a_ball_somewhere () ;
    store_world () ;
    try {find_ball ();}
    catch (...) {}
    retrieve_world () ;
    start_replay_for_timer () ;
    charles_world().startTimer(FAST_STEP_TIME) ;
}

void start_clean_string_with_balls ()
{
    reset () ;
    make_string_with_balls () ;                   // create the string of balls for Charles
    store_world () ;
    try {clean_string_with_balls (); }            // let Charles attempt to solve this challenge invisibly,
    catch (...) {}                                // while ignoring any error
    retrieve_world () ;
    start_replay_for_timer () ;                   // now replay all actions visibly, this time not ignoring errors
    charles_world().startTimer(FAST_STEP_TIME) ;
}

void start_clean_chaos_with_balls ()
{
    reset () ;
    make_chaos_with_balls () ;                    // create the chaos of balls starting state for Charles
    store_world () ;
    try {clean_chaos_with_balls (); }             // let Charles attempt to solve this challenge invisibly,
    catch (...) {}                                // while ignoring any error
    retrieve_world () ;
    start_replay_for_timer () ;                   // now replay all actions visibly, this time not ignoring errors
    charles_world().startTimer(FAST_STEP_TIME) ;
}

void start_around_the_block ()
{
    reset () ;
    make_block () ;                               // create the block and ball for Charles
    store_world () ;
    try {around_the_block (); }                   // let Charles attempt to solve this challenge invisibly,
    catch (...) {}                                // while ignoring any error
    retrieve_world () ;
    start_replay_for_timer () ;                   // now replay all actions visibly, this time not ignoring errors
    charles_world().startTimer(FAST_STEP_TIME) ;
}

void start_test ()
{
    store_world () ;
    try {test(); }                                // let Charles attempt to solve test invisibly,
    catch (...) {}                                // while ignoring any error
    retrieve_world () ;
    start_replay_for_timer () ;                   // now replay all actions visibly, this time not ignoring errors
    charles_world().startTimer(FAST_STEP_TIME) ;
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    Menu charles_menu ((char*)"Charles") ;
    charles_menu.add ((char*)"Reset", reset)
                .add ((char*)"Stop",  stop) ;

	Menu ch_menu ((char*)"Basic Charles Actions") ;
	ch_menu.add ((char*)"step ()", start_step)
	       .add ((char*)"turn_left ()", start_turn_left)
	       .add ((char*)"turn_right ()", start_turn_right)
	       .add ((char*)"put_ball ()", start_put_ball)
	       .add ((char*)"get_ball ()", start_get_ball);

	Menu l1_menu ((char*)"Examples Lecture 1");
	l1_menu.add ((char*)"Swap ball", start_swap_ball)
	       .add ((char*)"Find ball", start_search_the_ball);

	Menu a1_menu ((char*)"Assignment 1");
	a1_menu.add ((char*)"Balls: a string...", start_clean_string_with_balls )
		   .add ((char*)"Balls: chaos...", start_clean_chaos_with_balls )
		   .add ((char*)"Bonus: around the block...", start_around_the_block )
	       .add ((char*)"Test a function", start_test);

	Menu sn_menu ((char*)"Velocity");
	sn_menu.add ((char*)"Quick", quick)
		   .add ((char*)"Normal",normal)
		   .add ((char*)"Slow",slow)
		   .add ((char*)"Very slow",very_slow);

	try
	{
		charles_world().Run (charles_menu,WINARGS(hInstance, hPrevInstance, szCmdLine, iCmdShow));
	}
	catch (IllegalAction thisIllegalAction )
	{
		thisIllegalAction.report ();
	}
	catch (...)
	{
		makeAlert ("Something went terribly wrong!");
	}

	return 0;
}
