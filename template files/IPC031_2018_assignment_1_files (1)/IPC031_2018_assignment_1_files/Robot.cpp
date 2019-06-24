//#include <strstream>
#include <fstream>
#include <math.h>
#include <vector>

/*	Robot is a library to draw and manipulate a simple robot in a window.
	It provides the class ROBOT. Implementation is built on the gui_kernel.
	First version created in may 2000 by Sjaak Smetsers and Pieter Koopman, Radboud University.
	Last Modified: June 15 2018, by Peter Achten, Radboud University.
*/

#include "Robot.h"

using namespace std;

enum Cell { Empty, Ball, Wall };

enum Direction { North, West, South, East };

enum ACTION {STEP,TURN_LEFT,TURN_RIGHT,ON_BALL,GET_BALL,PUT_BALL,IN_FRONT_OF_WALL,NORTH};

int size (vector<ACTION>& v)
{
    return static_cast<int>(v.size()) ;
}

void rectangle (int lx, int ly, int rx, int ry, Cell cell, Cell w[WORLDWIDTH][WORLDHEIGHT] )
{
	for (int b = lx; b < rx; b++)
		w [b][ly] = w [b][ry-1] = cell;
	for (int h = ly + 1; h < ry - 1; h++)
		w [lx][h] = w [rx-1][h] = cell;
}

void IllegalAction :: report ()
{
	switch (_action)
	{
	case IA_Charles:
		makeAlert ("Charles is not allowed at this position.");
		break;
	case IA_GetBall:
		makeAlert ("Charles can not pick up a ball because there is no ball at this position.");
		break;
	case IA_PutBall:
		makeAlert ("Charles can not put a ball here because there is already a ball.");
		break;
	case IA_Step:
		makeAlert ("Charles bumped into a wall...");
		break;
	case IA_Open:
		makeAlert ("It was not possible to open the labyrinth.\nIt is probably in the wrong directory.");
		break;
    case IA_TooManySteps:
        makeAlert ("Charles has used too many steps.\nSelect a larger number of steps or check your algorithm for termination.");
        break;
	}
}

class ROBOT
{
public:
	void step			();
	void turn_left 		();
	void turn_right		();
	bool on_ball		();
	void get_ball		();
	void put_ball		();
	bool in_front_of_wall();
	bool north			();

	void rest			(int);
	void max_no_of_actions (int);

	void clear_world ();
	void new_world	(const char world []);
	void reset		();
    void clear_actions ();
	void redraw		(Canvas&);
	void redraw		();
	ROBOT (int st, Direction r, int x, int y);
	void draw_world	(Canvas&);
	void draw_robot	(Canvas&);
	void draw_robot	();

	void place_rectangle (int left, int bottom, int width, int height) ;
	void place_walls ( int left, int bottom, int no_of_walls, bool horizontal ) ;
	void place_balls ( int left, int bottom, int no_of_balls, bool horizontal ) ;
	void create_ball (int x, int y);

	bool actions_available ();
	void render_next_action ();
    void start_replay_for_timer ();
    int get_time_step ();
    void store_world ();
    void retrieve_world ();

protected:
	Direction	current_direction;
	int			x_pos;
	int			y_pos;
	int			time_step;
	int         max_number_of_actions;   // the maximum number of actions Charles can perform
	vector<ACTION> actions;              // the actions Charles is performing
	int         no_of_actions_replayed;  // the number of actions that have been replayed
private:
	void do_step ();
	void do_turn_left ();
	void do_turn_right ();
	bool do_on_ball	();
	void do_get_ball ();
	void do_put_ball ();
	bool do_in_front_of_wall ();
	bool do_north ();

	void draw_world_section  (Canvas &canvas, int from_x, int from_y, int to_x, int to_y);
	void clear_world_section (Canvas &canvas, int from_x, int from_y, int to_x, int to_y);

	void draw_wall  (Canvas &canvas, int x, int y);
	void draw_stone (Canvas &canvas, int x, int y);
	void draw_empty_cell (Canvas &canvas, int x, int y);
	void draw_ball (Canvas &canvas, int x, int y);

    void add_charles_action (ACTION action);
    void render_charles_action (ACTION action);
    void copy_world (Cell source [WORLDWIDTH][WORLDHEIGHT], Cell dest [WORLDWIDTH][WORLDHEIGHT]);

	GPOINT screenpos (int sin_dir, int cos_dir, int shape_edge);

	Cell World [WORLDWIDTH][WORLDHEIGHT];  // the 'real' world that gets manipulated
	Cell World2[WORLDWIDTH][WORLDHEIGHT];  // backup world that is used to restore the real world after creating Charles actions
	Direction	current_direction2;
	int			x_pos2;
	int			y_pos2;
	double charles_shape [4][2];
} ;

const int INITIAL_X_POS = 1;
const int INITIAL_Y_POS = WORLDHEIGHT-2;
const Direction INITIAL_DIRECTION = East;

void rectangle (int lx, int ly, int rx, int ry, Cell cell,Cell w[WORLDWIDTH][WORLDHEIGHT] );

class Robot : public ROBOT
{
public:
	Robot (int st = INITIAL_STEP_TIME, Direction r = INITIAL_DIRECTION, int x = INITIAL_X_POS, int y = INITIAL_Y_POS)
		: ROBOT (st, r, x, y) {}
} ;

void ROBOT :: add_charles_action (ACTION action)
{
    if (size(actions) < max_number_of_actions)
        actions.push_back(action);
    else
        throw IllegalAction (IA_TooManySteps) ;
}

void ROBOT :: start_replay_for_timer ()
{
    no_of_actions_replayed = 0 ;   // start replaying the first action
    copy_world (World2,World) ;    // restore the world to its starting situation
}

int ROBOT :: get_time_step ()
{
    return time_step ;
}

void ROBOT :: copy_world (Cell source [WORLDWIDTH][WORLDHEIGHT], Cell dest [WORLDWIDTH][WORLDHEIGHT])
{
    for (int ix=0 ; ix < WORLDWIDTH; ix++)
        for (int iy=0 ; iy < WORLDHEIGHT; iy++)
            dest[ix][iy] = source[ix][iy] ;
}

void ROBOT :: store_world ()
{
    copy_world (World,World2) ;
    x_pos2 = x_pos ;
    y_pos2 = y_pos ;
    current_direction2 = current_direction ;
}

void ROBOT :: retrieve_world ()
{
    copy_world (World2,World);
    x_pos = x_pos2 ;
    y_pos = y_pos2 ;
    current_direction = current_direction2 ;
	Canvas canvas (charles_world());
	clear_world_section	(canvas, 0, 0, WORLDWIDTH-1, WORLDHEIGHT-1);
	draw_world	(canvas);
    draw_robot	(canvas);
}

void ROBOT :: clear_world ()
{
	for (int b = 0; b < WORLDWIDTH; b++)
		for (int h = 0; h < WORLDHEIGHT; h++)
			 World [b][h] = Empty;
}

ROBOT :: ROBOT (int st, Direction r, int x, int y)
	: current_direction (r), x_pos (x), y_pos (y), time_step (st), max_number_of_actions(WORLDWIDTH*WORLDHEIGHT*10), no_of_actions_replayed (0)
{
	clear_world ();

	rectangle (0,0,WORLDWIDTH,WORLDHEIGHT,Wall,World);

	const double SCALE        = sqrt(3.0) / 3.0 ;
	const double SIZE_OF_TAIL = (SCALE + 0.2) * SIZE_OF_ROBOT / 2;

	charles_shape [0][0] = - SIZE_OF_ROBOT / 2;
	charles_shape [0][1] = - SIZE_OF_TAIL;
	charles_shape [1][0] = 0;
	charles_shape [1][1] = 0;
	charles_shape [2][0] = SIZE_OF_ROBOT / 2;
	charles_shape [2][1] = - SIZE_OF_TAIL;
	charles_shape [3][0] = 0;
	charles_shape [3][1] = SCALE * SIZE_OF_ROBOT;

	if (x_pos < 1 || y_pos < 1 || x_pos >= WORLDWIDTH - 1 || y_pos >= WORLDHEIGHT - 1 || World [1][y_pos] == Wall)
		throw IllegalAction (IA_Charles);
}

void ROBOT :: do_turn_left ()
{
	current_direction = static_cast<Direction> ((current_direction + 1) % 4);
}

void ROBOT :: do_turn_right ()
{
	current_direction = static_cast<Direction> ((current_direction + 3) % 4);
}

bool ROBOT :: do_on_ball ()
{
    return (World [x_pos][y_pos] == Ball);
}

void ROBOT :: do_get_ball ()
{
    if (on_ball())
		World [x_pos][y_pos] = Empty;
	else
		throw IllegalAction (IA_GetBall);
}

void ROBOT :: do_put_ball ()
{
	if (World [x_pos][y_pos] == Empty)
		World [x_pos][y_pos]  = Ball;
	else
		throw IllegalAction (IA_PutBall);
}

void ROBOT :: do_step ()
{
	switch (current_direction)
	{
		case North:
			if (World [x_pos][y_pos+1] == Wall)
				throw IllegalAction (IA_Step);
			else
				y_pos++;
			break;
		case East:
			if (World [x_pos+1][y_pos] == Wall)
				throw IllegalAction (IA_Step);
			else
				x_pos++;
			break;
		case South:
			if (World [x_pos][y_pos-1] == Wall)
				throw IllegalAction (IA_Step);
			else
				y_pos--;
			break;
		case West:
			if (World [x_pos-1][y_pos] == Wall)
				throw IllegalAction (IA_Step);
			else
				x_pos--;
			break;
	}
}

bool ROBOT :: do_in_front_of_wall ()
{
	switch (current_direction)
	{
		case North:
			return (World [x_pos][y_pos+1] == Wall);
		case East:
			return (World [x_pos+1][y_pos] == Wall);
		case South:
			return (World [x_pos][y_pos-1] == Wall);
		case West:
			return (World [x_pos-1][y_pos] == Wall);
		default :				// superfluous, but present to suppress compiler warning
			return false ;
	}
}

bool ROBOT :: do_north ()
{
    return current_direction == North;
}

void ROBOT :: turn_left ()
{
	add_charles_action (TURN_LEFT);
	do_turn_left () ;
}

void ROBOT :: turn_right ()
{
	add_charles_action (TURN_RIGHT);
	do_turn_right () ;
}

bool ROBOT :: on_ball ()
{
	add_charles_action (ON_BALL);
	return do_on_ball ();
}

void ROBOT :: get_ball ()
{
	add_charles_action (GET_BALL);
	do_get_ball () ;
}

void ROBOT :: put_ball ()
{
	add_charles_action (PUT_BALL);
	do_put_ball () ;
}

void ROBOT :: step ()
{
	add_charles_action (STEP);
    do_step () ;
}

bool ROBOT :: in_front_of_wall ()
{
	add_charles_action (IN_FRONT_OF_WALL);
	return do_in_front_of_wall () ;
}

bool ROBOT :: north ()
{
	add_charles_action (NORTH);
	return do_north ();
}

void ROBOT :: render_charles_action (ACTION action)
{
    switch (action)
    {
    case STEP:
        {
            Canvas canvas (charles_world());
            clear_world_section (canvas, x_pos - 1, y_pos - 1, x_pos + 1, y_pos + 1);
            draw_world_section  (canvas, x_pos - 1, y_pos - 1, x_pos + 1, y_pos + 1);
            do_step () ;
            draw_robot (canvas);
            break ;
        }
    case TURN_LEFT:
        {
            do_turn_left ();
            redraw ();
            break ;
        }
    case TURN_RIGHT:
        {
            do_turn_right ();
            redraw ();
            break ;
        }
    case GET_BALL:
        {
           	do_get_ball () ;
            redraw ();
            break ;
        }
    case PUT_BALL:
        {
           	do_put_ball () ;
            redraw ();
            break ;
        }
    default:  // ON_BALL, IN_FRONT_OF_WALL, NORTH have no rendering
        break ;
    }
}

void ROBOT :: rest (int r)
{
	time_step = r;
}

void ROBOT :: max_no_of_actions (int max_no)
{
    if (max_no > 0)
        max_number_of_actions = max_no ;
}

GPOINT toGPOINT (int x, int y)
{
	const int X_OFFSET = 12, Y_OFFSET = 12;
	return GPOINT (x + X_OFFSET, SCREENHEIGHT - y - Y_OFFSET);
}

GPOINT ROBOT :: screenpos (int sin_dir, int cos_dir, int shape_edge)
{
	double x = x_pos * 2 * SIZE_OF_STONE + cos_dir * charles_shape[shape_edge][0] - sin_dir * charles_shape[shape_edge][1];
	double y = y_pos * 2 * SIZE_OF_STONE + sin_dir * charles_shape[shape_edge][0] + cos_dir * charles_shape[shape_edge][1];
	return toGPOINT (int (x), int (y));
}

void ROBOT :: redraw (Canvas &canvas)
{
	clear_world_section (canvas, x_pos - 1, y_pos - 1, x_pos + 1, y_pos + 1);
	draw_world_section (canvas, x_pos - 1, y_pos - 1, x_pos + 1, y_pos + 1);
	draw_robot (canvas);
}

void ROBOT :: redraw ()
{
	Canvas canvas (charles_world());
	redraw (canvas);
}

void ROBOT :: draw_robot (Canvas &canvas)
{
	int sin_dir = -1, cos_dir = 0 ;

	switch (current_direction)
	{
		case East:
			sin_dir = -1;
			cos_dir = 0;
			break;
		case South:
			sin_dir = 0;
			cos_dir = -1;
			break;
		case West:
			sin_dir = 1;
			cos_dir = 0;
			break;
		case North:
			sin_dir = 0;
			cos_dir = 1;
			break;
	}

	GPOINT edges [4];

	for (int i = 0; i < 4; i++)
		edges [i] = screenpos (sin_dir, cos_dir, i);

	canvas.setPenColour (RedRGB);
	canvas.fillPolygon (edges, 4);
}

void  ROBOT :: draw_robot ()
{
	Canvas canvas (charles_world());
	draw_robot (canvas);
}

void  ROBOT :: draw_stone (Canvas &canvas, int x, int y)
{
	const GPOINT lo = toGPOINT (x * SIZE_OF_STONE - SIZE_OF_STONE / 2, y * SIZE_OF_STONE - SIZE_OF_STONE / 2);
	canvas.fillRectangle (lo, GPOINT (lo.x + SIZE_OF_STONE,lo.y - SIZE_OF_STONE));
}

void  ROBOT :: draw_wall (Canvas &canvas, int x, int y)
{
	canvas.setPenColour (BlueRGB);

	draw_stone (canvas, x*2, y*2);
	if (x < WORLDWIDTH - 1 && World [x+1][y] == Wall)
		draw_stone (canvas, x*2+1, y*2);
	if (y < WORLDHEIGHT - 1 && World [x][y+1] == Wall)
		draw_stone (canvas, x*2, y*2+1);
}

void  ROBOT :: draw_empty_cell (Canvas &canvas, int x, int y)
{
	canvas.setPenColour (BlackRGB);
	canvas.setPenPos (toGPOINT (x*2*SIZE_OF_STONE, y*2*SIZE_OF_STONE));
	canvas.drawPOINT ();
}

void  ROBOT :: draw_ball (Canvas &canvas, int x, int y)
{
	canvas.setPenColour (GreenRGB);

	const GPOINT lo = toGPOINT (x * 2 * SIZE_OF_STONE - SIZE_OF_BALL / 2, y * 2 * SIZE_OF_STONE - SIZE_OF_BALL / 2);
	canvas.fillOval (lo, GPOINT (lo.x + SIZE_OF_BALL,lo.y - SIZE_OF_BALL));
}

void ROBOT :: draw_world (Canvas& canvas)
{
	draw_world_section (canvas, 0, 0, WORLDWIDTH-1, WORLDHEIGHT-1);
}

void ROBOT :: clear_world_section (Canvas &canvas, int from_x, int from_y, int to_x, int to_y)
{
	const GPOINT lo = toGPOINT (from_x * 2 * SIZE_OF_STONE, from_y * 2 * SIZE_OF_STONE - SIZE_OF_STONE / 2);
	const GPOINT rb = toGPOINT (to_x * 2 * SIZE_OF_STONE, to_y * 2 * SIZE_OF_STONE + SIZE_OF_STONE / 2);

	canvas.setPenColour	(WhiteRGB);
	canvas.fillRectangle (lo, rb);
}

void ROBOT :: draw_world_section (Canvas &canvas, int from_x, int from_y, int to_x, int to_y)
{
	for (int b = from_x; b <= to_x; b++)
	{	for (int h = from_y; h <= to_y; h++)
		{	switch (World [b][h])
			{
				case Wall:
					draw_wall		(canvas, b, h);
					break;
				case Empty:
					draw_empty_cell	(canvas, b, h);
					break;
				default:
					draw_ball		(canvas, b, h);
					break;
			}
		}
	}
}

void ROBOT :: new_world (const char world [])
{
	ifstream labyrinth;

	labyrinth.open (world);

	if (labyrinth.fail ())
		throw IllegalAction (IA_Open);
	else
	{
		int dir, x_ball, y_ball, horizontal_walls, vertical_walls;

		labyrinth >> x_pos >> y_pos >> dir >> x_ball >> y_ball >> horizontal_walls >> vertical_walls;

		current_direction = Direction (dir);

		clear_world ();
		rectangle (0,0,WORLDWIDTH,WORLDHEIGHT,Wall,World);

		World [x_ball][y_ball] = Ball;

		for (int i = 0; i < horizontal_walls; i++)
		{	int x, y, d;
			labyrinth >> x >> y >> d;
			for (int j = 0; j < d; j++)
				World [x+j][y] = Wall;
		}
		for (int i = 0; i < vertical_walls; i++)
		{	int x, y, d;
			labyrinth >> x >> y >> d;
			for (int j = 0; j < d; j++)
				World [x][y+j] = Wall;
		}

		labyrinth.close ();

		Canvas canvas (charles_world());
		clear_world_section	(canvas, 0, 0, WORLDWIDTH-1, WORLDHEIGHT-1);

		draw_world	(canvas);
		draw_robot	(canvas);
	}
}

void ROBOT :: clear_actions ()
{
    while (size(actions) > 0)
    actions.pop_back();
    no_of_actions_replayed = 0 ;
}

void ROBOT :: reset()
{
	current_direction = INITIAL_DIRECTION;
	x_pos             = INITIAL_X_POS;
	y_pos             = INITIAL_Y_POS;
	clear_actions () ;

	clear_world ();
	rectangle (0,0,WORLDWIDTH,WORLDHEIGHT,Wall,World);
	copy_world (World,World2) ;

	Canvas canvas (charles_world());
	clear_world_section	(canvas, 0, 0, WORLDWIDTH-1, WORLDHEIGHT-1);

	draw_world (canvas);
	draw_robot (canvas);
}

bool ROBOT :: actions_available ()
{
    return no_of_actions_replayed < size (actions) ;
}

void ROBOT :: render_next_action ()
{
    if (actions_available())
    {
        render_charles_action (actions[no_of_actions_replayed]) ;
        no_of_actions_replayed++ ;
    }
}

Robot& Charles ()			// declaration of Charles the robot
{
	static Robot charles;
	return charles;
}

void RobotGUI :: Window (const RECT& area)
{
	Canvas canvas (charles_world ());
	Charles().draw_world (canvas);
	Charles().draw_robot (canvas);
}

void RobotGUI :: Timer (const int dt)
{
    Charles().render_next_action() ;
    stopTimer () ;
    if (Charles().actions_available())            // there are still Charles actions
        startTimer (Charles().get_time_step()) ;  // use the timer with the most recent delay (perhaps altered by user)
    else
        clear_actions () ;                        // clean up all Charles actions for a next experiment
}

RobotGUI :: RobotGUI()
	: GUI (GSIZE (SCREENWIDTH, SCREENHEIGHT), (char*)"Charles does its first steps")
{
	Charles();
}

RobotGUI& charles_world()
{
	static RobotGUI gui;
	return gui;
}

void ROBOT :: place_rectangle ( int left, int bottom, int width, int height )
{
	for (int hor = left; hor<=left+width; hor++)
	{
		World [hor][bottom]        = Wall ;
		World [hor][bottom+height] = Wall ;
	};

	for (int vert = bottom; vert<=bottom+height; vert++)
	{
		World [left][vert]       = Wall ;
		World [left+width][vert] = Wall ;
	};

	Canvas canvas (charles_world());
	draw_world (canvas);
}

void ROBOT :: place_walls ( int left, int bottom, int no_of_walls, bool horizontal )
{
    if (horizontal)
    {
        for (int x = left ; x <= left + no_of_walls ; x++)
            World [x][bottom] = Wall ;
    }
    else
    {
        for (int y = bottom; y <= bottom+no_of_walls ; y++)
            World[left][y] = Wall ;
    }
    Canvas canvas (charles_world()) ;
    draw_world (canvas) ;
    draw_robot (canvas) ;
}

void ROBOT :: place_balls ( int left, int bottom, int no_of_balls, bool horizontal )
{
    if (horizontal)
    {
        for (int x = left ; x <= left + no_of_balls ; x++)
            World [x][bottom] = Ball ;
    }
    else
    {
        for (int y = bottom; y <= bottom+no_of_balls ; y++)
            World[left][y] = Ball ;
    }
    Canvas canvas (charles_world()) ;
    draw_world (canvas) ;
    draw_robot (canvas) ;
}

void ROBOT :: create_ball (int x, int y)
{
    if (x >= 0 && x < WORLDWIDTH && y >= 0 && y < WORLDHEIGHT)
    {
        World[x][y] = Ball;
        Canvas canvas (charles_world());
        draw_world (canvas);
        draw_robot (canvas);
    }
}

// turning method calls into function calls for student assignments:

void step			() { Charles().step(); }
void turn_left 		() { Charles().turn_left(); }
void turn_right		() { Charles().turn_right(); }

bool on_ball		() { return Charles().on_ball(); }

void get_ball		() { Charles().get_ball(); }
void put_ball		() { Charles().put_ball(); }

bool in_front_of_wall () { return Charles().in_front_of_wall(); }
bool north			() { return Charles().north(); }

void rest       	(int n) { Charles().rest(n); }

void clear_world () { Charles().clear_world(); }
void new_world (const char world [])  { Charles().new_world(world); }
void reset			() { Charles().reset(); }
void stop 			() { charles_world().Stop();}

void place_rectangle        (int left, int bottom, int width, int height)  { Charles().place_rectangle(left,bottom,width,height); }
void place_walls            (int left, int bottom, int no_of_walls, bool horizontal) { Charles().place_walls (left,bottom,no_of_walls,horizontal); }
void place_balls            (int left, int bottom, int no_of_balls, bool horizontal) { Charles().place_balls (left,bottom,no_of_balls,horizontal); }
void make_labyrinth         () { Charles().new_world ( "labyrinth" ) ; }
void create_ball            (int x, int y) { Charles().create_ball(x,y); }
void start_replay_for_timer () { Charles().start_replay_for_timer(); }
void store_world            () { Charles().store_world(); }
void retrieve_world         () { Charles().retrieve_world(); }
void clear_actions          () { Charles().clear_actions(); }
