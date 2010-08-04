/* engine includes */
#include <acknex.h>
#include <default.c>


#define PRAGMA_POINTER
#define PRAGMA_PATH ".." //toolbox path

#define TOOLBOX_USE_SYSMSG
#define TOOLBOX_USE_STATE
#include "toolbox.h" 

// States
#define START 0
#define MENU 1
#define PLAY 2
#define END 3

// States and conditions
/* ------------------------------------------------- *

               key_q                  key_e
   START <--------------> MENU ---------------> PLAY
     ^         key_w       |                     |
     |                     | key_s               |
     |                     |                     |
     |   key_a             v                     |
     \------------------> END <------------------/
         key_z                       key_d

 * ------------------------------------------------- */
// State changes will be displayed with 1 second delay to make sure the keypresses are recognized correctly.
// The state change process itself is very fast.

var state = -1;

void printstate()
{
	printf("current state: %d", state/1024);
}

//START state
void init0()
{
	wait (-1);
	printf("Init START");
}

void deinit0()
{
	wait (-1);
	printf("Deinit START");
}

void cyclicfunc0()
{
	// tracking the state HERE of course makes no sense. This is done internally by the state manager.
	// It is just used in this sample to show that the function really is called
	state = START;
}

void cyclicfunc1()
{
	state = MENU;
}

void cyclicfunc2()
{
	state = PLAY;
}

void cyclicfunc3()
{
	state = END;
}


//transition functions
void transitionfunc01()
{
	wait (-1);
	printf("transition function START -> MENU called");	
}

void transitionfunc03()
{
	wait (-1);
	printf("transition function START -> END called");	
}

void transitionfunc10()
{
	wait (-1);
	printf("transition function MENU -> START called");	
}

void transitionfunc12()
{
	wait (-1);
	printf("transition function MENU -> PLAY called");	
}

void transitionfunc13()
{
	wait (-1);
	printf("transition function MENU -> END called");	
}

void transitionfunc23()
{
	wait (-1);
	printf("transition function PLAY -> END called");	
}

void transitionfunc30()
{
	wait (-1);
	printf("transition function END -> START called");	
}

//transition conditions
var conditionfunc01()
{
	if (key_q) return 1;
	else return 0;
}

var conditionfunc03()
{
	if (key_a) return 1;
	else return 0;
}

var conditionfunc10()
{
	if (key_w) return 1;
	else return 0;
}

var conditionfunc12()
{
	if (key_e) return 1;
	else return 0;
}

var conditionfunc13()
{
	if (key_s) return 1;
	else return 0;
}

var conditionfunc23()
{
	if (key_d) return 1;
	else return 0;
}

var conditionfunc30()
{
	if (key_z) return 1;
	else return 0;
}




void main()
{
	STATEMGR* psState;

	screen_color.blue = 150;

	psState = STATEMGR_create(START);
	SYSMSG_create();
	
	// add states and cyclic functions

	STATEMGR_addState(psState, START, cyclicfunc0, init0, deinit0);
	STATEMGR_addState(psState, MENU,  cyclicfunc1, NULL, NULL);
	STATEMGR_addState(psState, PLAY,  cyclicfunc2, NULL, NULL);
	STATEMGR_addState(psState, END,   cyclicfunc3, NULL, NULL);
	//STATEMGR_addState(psState, END,   cyclicfunc3, NULL, NULL);

	// add state transitions

	STATEMGR_addTransition(psState, START, MENU,  conditionfunc01, transitionfunc01);
	STATEMGR_addTransition(psState, START, END,   conditionfunc03, transitionfunc03);
	STATEMGR_addTransition(psState, MENU,  START, conditionfunc10, transitionfunc10);
	STATEMGR_addTransition(psState, MENU,  PLAY,  conditionfunc12, transitionfunc12);
	STATEMGR_addTransition(psState, MENU,  END,   conditionfunc13, transitionfunc13);
	STATEMGR_addTransition(psState, PLAY,  END,   conditionfunc23, transitionfunc23);
	STATEMGR_addTransition(psState, END,   START, conditionfunc30, transitionfunc30);
	//STATEMGR_addTransition(psState, END,   START, conditionfunc30, transitionfunc30);
	
	// check which state is active by pressing space
	on_space = printstate;
	
	while(!key_esc)
	{
		// cyclically execute state manager
		STATEMGR_execute(psState);
		wait (1);
	}		
	STATEMGR_remove(psState);
	SYSMSG_remove();
	sys_exit("");
}