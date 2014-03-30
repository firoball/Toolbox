/*
 *******************************************************************************
 * inv_sample.c
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
 * Sample script for INVENTORY module
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */

#include <acknex.h>

#define PRAGMA_POINTER
#define PRAGMA_PATH ".."

#define TOOLBOX_USE_INVENTORY
#define TOOLBOX_USE_SYSMSG
#include "toolbox.h"


INVENTORY* psInv;
var vPos;
var vDir;

void invent_toggle()
{
	if (INVENTORY_isVisible(psInv))
		INVENTORY_hide(psInv);
	else
		INVENTORY_show(psInv);
}

void invent_pos()
{
	vPos++;
	vPos &= 3;
	
	switch(vPos)
	{
		case 0:
			INVENTORY_setPos(psInv, 0, 0, INV_ALIGN_LEFT | INV_ALIGN_TOP);
			break;
			
		case 1:
			INVENTORY_setPos(psInv, 0, 0, INV_ALIGN_RIGHT | INV_ALIGN_TOP);
			break;
			
		case 2:
			INVENTORY_setPos(psInv, 0, 0, INV_ALIGN_RIGHT | INV_ALIGN_BOTTOM);
			break;
			
		case 3:
			INVENTORY_setPos(psInv, 0, 0, INV_ALIGN_LEFT | INV_ALIGN_BOTTOM);
			break;
			
		default:
			break;
	}
}

void add_item_q()
{
	INVITEM* psItem;
	var vResult;
	
	psItem = INVITEM_create("item_q.pcx", 1, 10, 1);
	vResult = INVENTORY_add(psInv, psItem);
	
	if (vResult)
		error ("item added");
	else
		error ("item not added");
}

void add_item_a()
{
	INVITEM* psItem;
	var vResult;
	
	psItem = INVITEM_create("item_a.pcx", 2, 20, 1);
	vResult = INVENTORY_add(psInv, psItem);
	
	if (vResult)
		error ("item added");
	else
		error ("item not added");
}

void add_item_y()
{
	INVITEM* psItem;
	var vResult;
	
	psItem = INVITEM_create("item_y.pcx", 3, 25, 1);
	vResult = INVENTORY_add(psInv, psItem);
	
	if (vResult)
		error ("item added");
	else
		error ("item not added");
}

void drop_item()
{
	INVENTORY_sub(psInv);
}

void next_item()
{
	INVENTORY_next(psInv);
}

void prev_item()
{
	INVENTORY_prev(psInv);
}

void main()
{
	video_mode = 7; 
	screen_color.blue = 150;
	vPos = 0;
	vDir = 0;
	
	wait (3);
	SYSMSG_create();
	SYSMSG_setMode(SYSMSG_SHOW_ERROR);
	psInv = INVENTORY_create("invtemp.pcx", 20);
	INVENTORY_setSize(psInv, 5 /*items*/, 95/*weight*/, 4 /*amount*/);
	INVENTORY_show(psInv);
	
	/* assign keys */
	on_1 = invent_toggle;
	on_2 = invent_pos;
	on_q = add_item_q;
	on_a = add_item_a;
	on_z = add_item_y;
	on_d = drop_item;
	on_cur = next_item;
	on_cul = prev_item;
	
	while (key_esc == 0)
		wait (1);
		
	INVENTORY_remove(psInv);
	SYSMSG_remove();
	sys_exit(NULL);
}