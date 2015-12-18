/*
 *******************************************************************************
 * control_sample.c
 * Creation date: 05.10.2010
 * Author:        Firoball
 *
 *******************************************************************************
 * $Date$
 * $Revision$
 * $Author$
 *
 *******************************************************************************
 * Description
 *
 * Sample script for CONTROL module
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */

#include <acknex.h>
#include <default.c>

#define PRAGMA_POINTER
#define PRAGMA_PATH ".."

#define TOOLBOX_USE_SYSMSG
#define TOOLBOX_USE_CONTROL
#include "toolbox.h"

#define C_UP 0
#define C_DOWN 1
#define C_LEFT 2
#define C_RIGHT 3
#define C_FIRE 4

CTRL* psCtrl;
TEXT* txtDisplay = {pos_y = 200; flags |= SHOW; strings = 2;}
void ctrl_show();
void ctrl_hide();

void ctrl_show()
{
	CTRL_show(psCtrl);
	on_space = ctrl_hide;
}

void ctrl_hide()
{
	CTRL_hide(psCtrl);
	on_space = ctrl_show;
}

void ctrl_save()
{
	/* save control layout to disk */
	CTRL_save(psCtrl, "control.txt");
}


void ctrl_load()
{
	/* load control layout from disk */
	CTRL_load(psCtrl, "control.txt");
}

/* helper functions for display message */
void func_up()
{
	str_cpy((txtDisplay->pstring)[0], "UP key pressed");
	wait (-0.5);
	str_cpy((txtDisplay->pstring)[0], "");
}

void func_down()
{
	str_cpy((txtDisplay->pstring)[0], "DOWN key pressed");
	wait (-0.5);
	str_cpy((txtDisplay->pstring)[0], "");
}

void func_left()
{
	str_cpy((txtDisplay->pstring)[0], "LEFT key pressed");
	wait (-0.5);
	str_cpy((txtDisplay->pstring)[0], "");
}

void func_right()
{
	str_cpy((txtDisplay->pstring)[0], "RIGHT key pressed");
	wait (-0.5);
	str_cpy((txtDisplay->pstring)[0], "");
}

void func_fire()
{
	str_cpy((txtDisplay->pstring)[0], "FIRE key pressed");
	wait (-0.5);
	str_cpy((txtDisplay->pstring)[0], "");
}

void main()
{
	video_mode = 7; 
	screen_color.blue = 150;

	SYSMSG_create();

	/* control unit setup */
	psCtrl = CTRL_create(5, "Great Shooter", /*"Ænigma Scrawl 4 BRK#35b"*/"ackfont.pcx", FILTER | OUTLINE);
	CTRL_setDescription(psCtrl, C_UP, "Move Up");	
	CTRL_setDescription(psCtrl, C_DOWN, "Move Down");	
	CTRL_setDescription(psCtrl, C_LEFT, "Move Left");	
	CTRL_setDescription(psCtrl, C_RIGHT, "Move Right");	
	CTRL_setDescription(psCtrl, C_FIRE, "Fire");	

	/* assign functions to keys */
	CTRL_setFunction(psCtrl, C_UP, func_up);
	CTRL_setFunction(psCtrl, C_DOWN, func_down);
	CTRL_setFunction(psCtrl, C_LEFT, func_left);
	CTRL_setFunction(psCtrl, C_RIGHT, func_right);
	CTRL_setFunction(psCtrl, C_FIRE, func_fire);

	/* some special behaviour flags */
	CTRL_setFlags(psCtrl, CTRL_ENABLE_FADE /*| CTRL_DENY_MOUSE*/);

	/* load last layout from disk - if any */
	CTRL_load(psCtrl, "default_control.txt");

	on_space = ctrl_show;
	on_s     = ctrl_save;
	on_l     = ctrl_load;
	
	/* helper functionylity for display message */
	while(key_esc != 1)
	{
		if (CTRL_isPressed(psCtrl, C_FIRE))
		{
			str_cpy((txtDisplay->pstring)[1], "FIRE key down");
		}
		else
		{
			str_cpy((txtDisplay->pstring)[1], "");
		}
		
		wait (1);
	}
	CTRL_hide(psCtrl);

	/* now remove it from memory */
	CTRL_remove(psCtrl);
	SYSMSG_remove();
	sys_exit(NULL);
}