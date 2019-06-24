#include "Robot.h"
/*
	Charles is a framework for manipulating Charles the robot.
	Charles is a subclass of ROBOT from the library Robot.
	Original version created by Sjaak Smetsers and Pieter Koopman, Radboud University.
	Last Modified: August 31 2018, by Peter Achten, Radboud University.
*/

// =========================================================================

// Here are a number of auxiliary functions. You do not need to study or use them yourself.

// steps (no_of_steps) makes Charles move no_of_steps steps forward (without checking if he hits a wall)
void steps ( int no_of_steps )
{
    while (no_of_steps > 0)
    {
        step () ;
        no_of_steps-- ;
    }
}

// safe_put_ball makes Charles put a ball on an empty spot only
void safe_put_ball ()
{
    if (!on_ball())
        put_ball () ;
}

// balls (no_of_balls) makes Charles move no_of_balls forward (without checking if he hits a wall) and put balls down safely.
void balls (int no_of_balls)
{
    while (no_of_balls > 0)
    {
        safe_put_ball () ;
        no_of_balls-- ;
        if (no_of_balls > 0)
            step () ;
    }
}

// make_grandstand puts walls in Charles' world that resembles a grandstand
void make_grandstand ()
{
    const int FIELD_WIDTH  = 5 + rand () % (WORLDWIDTH / 5) ;
    const int NO_OF_STAIRS = 1 + rand () % (WORLDHEIGHT / 4) ;
    place_walls (1, WORLDHEIGHT / 2, FIELD_WIDTH, true) ;
    for (int stair = 0 ; stair < NO_OF_STAIRS ; stair++)
    {
        place_walls (1+FIELD_WIDTH+stair, WORLDHEIGHT / 2 + stair, 1, false) ;
        place_walls (1+FIELD_WIDTH+stair, WORLDHEIGHT / 2 + stair+1,1,true) ;
    }
    place_walls (1+FIELD_WIDTH+NO_OF_STAIRS, WORLDHEIGHT / 2 + NO_OF_STAIRS,1,true) ;
}

// make_path_with_balls creates a path that Charles must follow from begin to end
void make_path_with_balls ()
{
    const int L1 = WORLDWIDTH * 6 / 10 + rand () % (WORLDWIDTH/3) ;
    const int L2 = WORLDHEIGHT / 2 - 2 - rand () % (WORLDHEIGHT/6) ;
    const int L3 = WORLDWIDTH / 3 + rand () % (WORLDWIDTH/5) ;
    const int L4 = 2 * L2 ;
    const int L5 = (L1-L3)/2 ;
    const int L6 = L4 * 4 / 5 ;
    place_balls (1,WORLDHEIGHT-2,2,true) ;
    place_balls (1,WORLDHEIGHT-2,2,true) ;
    place_balls (3,WORLDHEIGHT/2,WORLDHEIGHT/2-3,false) ;
    place_balls (3,WORLDHEIGHT/2,L1,true) ;
    place_balls (L1+4,WORLDHEIGHT/2,L2,false) ;
    place_balls (L1-L3+4,WORLDHEIGHT/2+L2,L3,true) ;
    place_balls (L1-L3+4,WORLDHEIGHT/2+L2-L4,L4,false) ;
    place_balls (L1-L3-L5+4,WORLDHEIGHT/2+L2-L4,L5,true) ;
    place_balls (L1-L3-L5+4,WORLDHEIGHT/2+L2-L4,L6,false) ;
}

void make_cave_wall (bool at_north_edge)
{
    const int MARGIN    = WORLDHEIGHT / 3 ;
    const int MAX_WIDTH = WORLDWIDTH / 10 ;

    for (int ix = 2; ix <= WORLDWIDTH-3; )
    {
        const int WIDTH = rand() % (min (MAX_WIDTH, WORLDWIDTH - ix - 2)) + 1 ;
        const int DY = rand() % MARGIN + 2 ;
        int y ;
        if (at_north_edge)
            y = DY ;
        else
            y = WORLDHEIGHT - DY - 1 ;
        place_walls (ix,y,WIDTH-1,true) ;
        ix += WIDTH ;
    }
}

void make_cave ()
{
    make_cave_wall (true) ;
    make_cave_wall (false) ;
}

int shamrock_length (int dimension, int margin)
{
    return (dimension / 4) - margin ;//- (rand () % (dimension / 8)) ;
}

// make_shamrock creates a shamrock-like figure with 1, 2, 3, or 4 'leaves' inside Charles' world and places a ball
// on the 'north' most lane so that Charles knows where to find the shamrock.
void make_shamrock ()
{
    const int NO_OF_LEAVES = rand () % 4 + 1 ;
    const int MARGIN  = 4 ;
    const int MW = WORLDWIDTH / 2 ;
    const int MH = WORLDHEIGHT / 2 ;
    const int NW = shamrock_length (WORLDWIDTH, MARGIN) ;
    const int SW = shamrock_length (WORLDWIDTH, MARGIN) ;
    const int WW = shamrock_length (WORLDWIDTH, MARGIN) ;
    const int EW = shamrock_length (WORLDWIDTH, MARGIN) ;
    const int NH = shamrock_length (WORLDHEIGHT, MARGIN) ;
    const int SH = shamrock_length (WORLDHEIGHT, MARGIN) ;
    const int WH = shamrock_length (WORLDHEIGHT, MARGIN) ;
    const int EH = shamrock_length (WORLDHEIGHT, MARGIN) ;
    if (NO_OF_LEAVES != 2)
    {
        place_rectangle (MW-NW/2,MH+max(WH,EH)/2+MARGIN,NW,NH) ;
        place_walls (MW,MH,max(WH,EH)/2+MARGIN,false) ;
    }
    if (NO_OF_LEAVES != 1)
    {
        place_rectangle (MW-max(NW,SW)/2-MARGIN-WW,MH-WH/2,WW,WH) ;
        place_rectangle (MW+max(NW,SW)/2+MARGIN,   MH-EH/2,EW,EH) ;
    }
    if (NO_OF_LEAVES == 4)
        place_rectangle (MW-SW/2,MH-max(WH,EH)/2-MARGIN-SH,SW,SH) ;
    if (NO_OF_LEAVES != 1)
        place_walls (MW-max(NW,SW)/2-MARGIN,MH,max(NW,SW)+2*MARGIN,true) ;
    place_walls (MW,MH-max(WH,EH)/2-MARGIN,max(WH,EH)/2+MARGIN,false) ;
    if (NO_OF_LEAVES == 1)
        create_ball (MW-NW/2+rand () % NW,WORLDHEIGHT-2) ;
    else
        create_ball (MW-max(NW,SW)/2-MARGIN-WW+rand () % (WW+max(NW,SW)+EW+2*MARGIN),WORLDHEIGHT-2) ;
}

// =========================================================================

// Here are the examples that have been explained during the lecture
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

// Charles wants to watch a match

// climb_one_step makes Charles climb one 'step'
// Assumptions: steps are one wall high and one wall wide, and Charles is immediately in front of a step
// Result:      Charles has moved one step up
void climb_one_step ()
{
    turn_left () ;
    step () ;
    turn_right () ;
    step () ;
}

// climb_all_steps makes Charles climb all 'steps'
// Assumptions: steps are one wall high and one wall wide, and Charles is immediately in front of a step
// Result:      Charles has moved all the way up
void climb_all_steps ()
{
   while (in_front_of_wall ())
      climb_one_step () ;
}

// have_a_seat makes Charles 'sit down' at the top of the 'grandstand'
// Assumption: Charles is already at the topmost 'stair'
// Result:     Charles has moved two places to the east and faces west
void have_a_seat ()
{
    step () ;
    step () ;
    turn_left () ;
    turn_left () ;
}

// The effect of this function is that Charles attempts to find its seat at the 'top' of the 'grandstand'
// Assumptions: Charles is standing on its base position, and a 'field' and 'grandstand' can be found at the western-most exterior wall.
// Result:      Charles stands at the 'top' of the 'grandstand'
void charles_goes_to_his_seat ()
{
    walk_to_wall () ;
    climb_all_steps () ;
    have_a_seat () ;
}

// walk_to_field makes Charles 'decend' to the field in front of the grandstand
void walk_to_field ()
{
    turn_right () ;
    walk_to_wall () ;
    turn_left () ;
}

// Placing a 'rectangle' of balls and create 'art' with it
void rectangle ( int width, int height )
{
    balls ( width ) ;
    turn_right ( ) ;
    balls ( height ) ;
    turn_right ( ) ;
    balls ( width ) ;
    turn_right ( ) ;
    balls ( height ) ;
    turn_right ( ) ;
}

void art ()
{
    rectangle ( 3, 2 ) ;
    rectangle ( 5, 4 ) ;
    rectangle ( 7, 6 ) ;
}


// =========================================================================

// here starts the part with code to be completed by yourself

// Name / student number :
// Name / student number :

// ********** ASSIGNMENT 2 *****************

// give one or more comment lines about what will happen in this function
void hansl_and_gretl ()
{
    // enter your Charles code here
}

// give one or more comment lines about what will happen in this function
// note that you are allowed to add formal parameters to fill_cave_with_balls if that is necessary for your solution
void fill_cave_with_balls ()
{
    // enter your Charles code here
}

// give one or more comment lines about what will happen in this function
void cave ()
{
	// if necessary for your solution, you are allowed to give actual parameters to fill_cave_with_balls
	fill_cave_with_balls () ;
	fill_cave_with_balls () ;
}

// give one or more comment lines about what will happen in this function
void shamrock ()
{
    // enter your Charles code here
}

// end of part with code to be completed by yourself
// =========================================================================


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

void start_watch_match ()
{
    reset () ;
    make_grandstand () ;                          // create the grandstand
    store_world () ;
    try {
        walk_to_field () ;
        charles_goes_to_his_seat(); }             // let Charles go to his seat invisibly,
    catch (...) {}                                // while ignoring any error
    retrieve_world () ;
    start_replay_for_timer () ;                   // now replay all actions visibly, this time not ignoring errors
    charles_world().startTimer(FAST_STEP_TIME) ;
}

void start_art ()
{
    reset () ;
    store_world () ;
    try {
        art(); }                                  // let Charles create art,
    catch (...) {}                                // while ignoring any error
    retrieve_world () ;
    start_replay_for_timer () ;                   // now replay all actions visibly, this time not ignoring errors
    charles_world().startTimer(FAST_STEP_TIME) ;
}

void start_hansl_and_gretl ()
{
    reset () ;
    make_path_with_balls () ;
    store_world () ;
    try {hansl_and_gretl ();}
    catch (...) {}
    retrieve_world () ;
    start_replay_for_timer () ;
    charles_world().startTimer(FAST_STEP_TIME) ;
}

void start_cave ()
{
    reset () ;
    make_cave () ;
    store_world () ;
    try {cave ();}
    catch (...) {}
    retrieve_world () ;
    start_replay_for_timer () ;
    charles_world().startTimer(FAST_STEP_TIME) ;
}

void start_shamrock ()
{
    reset () ;
    make_shamrock () ;
    store_world () ;
    try {shamrock (); }
    catch (...) {}
    retrieve_world () ;
    start_replay_for_timer () ;
    charles_world().startTimer (FAST_STEP_TIME) ;
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

	Menu l2_menu ((char*)"Examples Lecture 2");
	l2_menu.add ((char*)"Watch match", start_watch_match)
	       .add ((char*)"Art",start_art);

	Menu a2_menu ((char*)"Assignment 2");
	a2_menu.add ((char*)"Hansl and Gretl", start_hansl_and_gretl )
		   .add ((char*)"Cave", start_cave )
		   .add ((char*)"Bonus: follow shamrock...", start_shamrock );

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
