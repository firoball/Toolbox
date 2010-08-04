/*
 *******************************************************************************
 * control.c
 * Creation date: 22.10.2009
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
 * Script for key control functions
 *
 * Comments
 * 
 * for short descriptions see comments in control.h
 *
 *******************************************************************************
 */

/* ----- INCLUDES ----- */


#include <strio.c>


/* ----- GLOBALS ----- */

TEXT* CTRL_txtKeymap = {/*flags = SHOW;*/} 
var CTRL_vInitialized = 0;
STRING* CTRL_strDummy = "";
CTRL* CTRL_psCurrent = NULL;

void CTRL__pfKeyPress(); /* function pointer */

/* ----- EXTERNAL FUNCTIONS ----- */

CTRL* CTRL_create(var vNumKeys, STRING* strTitle)
{
	return CTRL_create(vNumKeys, strTitle, "ackfont.pcx", (var)0);
}

CTRL* CTRL_create(var vNumKeys, STRING* strTitle, STRING* strFont, var vTextFlags)
{
	int i;
	BMAP* bmapTemp;
	var vWidth;
	var vHeight;
	CTRL* psCtrl;
	
	/* perform a first initialization of the keymap data */
	CTRL__initialize();
	
	psCtrl = (CTRL*)malloc(sizeof(CTRL));
	psCtrl->vNumKeys  = vNumKeys;
	psCtrl->vKeyCodes = (var*)malloc(sizeof(var) * vNumKeys);
	psCtrl->vKeyLocks = (var*)malloc(sizeof(var) * vNumKeys);
	psCtrl->pfFunc    = (void**)malloc(sizeof(void*) * vNumKeys);
	psCtrl->vSelected = 0;
	psCtrl->vActive   = 1;
	psCtrl->vVisible  = 0;
	psCtrl->vResX     = CTRL_UNDEFINED;
	psCtrl->vResY     = CTRL_UNDEFINED;
	psCtrl->vBusy     = 0;
	psCtrl->vFlags    = 0;
	psCtrl->vForceUpdate = 0;
	
	for (i = 0; i < vNumKeys; i++)
	{
		(psCtrl->vKeyCodes)[i] = CTRL_UNDEFINED;
		(psCtrl->vKeyLocks)[i] = CTRL_UNLOCKED;
		(psCtrl->pfFunc)[i]    = NULL;
	}
	
	/* drop all non allowed text flags */
	vTextFlags &= ~CENTER_X & ~CENTER_Y & ~ARIGHT & ~SHOW & ~TRANSLUCENT;
	psCtrl->fntText        = font_create(strFont);

	psCtrl->txtDisplay     = txt_create(1, CTRL_LAYER);
	psCtrl->txtDisplay->font = psCtrl->fntText;
	str_cpy((psCtrl->txtDisplay->pstring)[0], strTitle);
	psCtrl->txtDisplay->flags |= CENTER_X | vTextFlags; 

	psCtrl->txtKey         = txt_create(vNumKeys, CTRL_LAYER);
	psCtrl->txtKey->font = psCtrl->fntText;
	psCtrl->txtKey->flags |= CENTER_X | vTextFlags; 

	psCtrl->txtDescription = txt_create(vNumKeys, CTRL_LAYER);
	psCtrl->txtDescription->font = psCtrl->fntText;
	psCtrl->txtDescription->flags |= vTextFlags; 

	psCtrl->txtSelection   = txt_create(vNumKeys, CTRL_LAYER + 1);
	psCtrl->txtSelection->font = psCtrl->fntText;
	psCtrl->txtSelection->flags |= CENTER_X | LIGHT | vTextFlags;
	psCtrl->txtSelection->red   = CTRL_TEXTCOL_SEL_RED;
	psCtrl->txtSelection->green = CTRL_TEXTCOL_SEL_GREEN;
	psCtrl->txtSelection->blue  = CTRL_TEXTCOL_SEL_BLUE;

	/* start cyclic polling function */
	CTRL__loop(psCtrl);

	return psCtrl;
}

void CTRL_remove(CTRL* psCtrl)
{
	int i;
	
	/* deactivate all loop actions before removal */
	psCtrl->vActive = 0;
	/* hide control menu before removal if still visible */
	if (psCtrl->vVisible == 1)
	{
		CTRL_hide(psCtrl);
	}
	
	wait (1);
	
	free(psCtrl->vKeyCodes);
	free(psCtrl->vKeyLocks);
	free(psCtrl->pfFunc);
	ptr_remove(psCtrl->fntText);
	ptr_remove(psCtrl->txtDisplay);
	ptr_remove(psCtrl->txtKey);
	ptr_remove(psCtrl->txtDescription);
	ptr_remove(psCtrl->txtSelection);
	
	free(psCtrl);
}

void CTRL_setDescription(CTRL* psCtrl, var vIndex, STRING* strDesc)
{
	if ((vIndex < psCtrl->vNumKeys) && (strDesc != NULL))
	{
		str_cpy((psCtrl->txtDescription->pstring)[vIndex], strDesc); 
	}
	else
	{
		#ifdef SYSMSG_ACTIVE
		SYSMSG_print(SYSMSG_SHOW_ERROR, "CTRL_setDescription: given index out of range or given string empty");
		#endif
	}
}

STRING* CTRL_getDescription(CTRL* psCtrl, var vIndex)
{
	STRING* pstrTemp;
	
	if (vIndex < psCtrl->vNumKeys)
	{
		pstrTemp = (psCtrl->txtDescription->pstring)[vIndex];
	}
	else
	{
		#ifdef SYSMSG_ACTIVE
		SYSMSG_print(SYSMSG_SHOW_ERROR, "CTRL_getDescription: given index out of range");
		#endif
		pstrTemp = CTRL_strDummy;
	}

	return pstrTemp;
}

void CTRL_setFunction(CTRL* psCtrl, var vIndex, void* pfFunc)
{
	if ((vIndex < psCtrl->vNumKeys) && (pfFunc != NULL))
	{
		(psCtrl->pfFunc)[vIndex] = pfFunc;
	}
	
	if (vIndex >= psCtrl->vNumKeys)
	{
		#ifdef SYSMSG_ACTIVE
		SYSMSG_print(SYSMSG_SHOW_ERROR, "CTRL_setFunction: given index out of range");
		#endif
	}	
}

var CTRL_isPressed(CTRL* psCtrl, var vIndex)
{
	var vRetval;
	
	if (vIndex < psCtrl->vNumKeys)
	{
		/* evaluate key presses only if setup menu is not visible */
		if (psCtrl->vVisible == 0)
		{
			vRetval = key_pressed((psCtrl->vKeyCodes)[vIndex]);
		}
		else
		{
			vRetval = 0;
		}
	}
	else
	{
		#ifdef SYSMSG_ACTIVE
		SYSMSG_print(SYSMSG_SHOW_ERROR, "CTRL_isPressed: given index out of range");
		#endif
		vRetval = 0;
	}
	
	return vRetval;
}

void CTRL_show(CTRL* psCtrl)
{
	/* do not show when busy */
	if ((psCtrl->vBusy != 0) || (psCtrl->vVisible != 0)) return;
	
	psCtrl->txtDisplay->flags     |= SHOW;
	psCtrl->txtSelection->flags   |= SHOW;
	psCtrl->txtKey->flags         |= SHOW;
	psCtrl->txtDescription->flags |= SHOW;

	/* select first key binding */
	psCtrl->vSelected = 0;
	CTRL__update(psCtrl);

	psCtrl->vVisible = 1;	

	if ((psCtrl->vFlags & CTRL_ENABLE_FADE) != 0)
	{
		psCtrl->txtDisplay->alpha     = 0;
		psCtrl->txtSelection->alpha   = 0;
		psCtrl->txtKey->alpha         = 0;
		psCtrl->txtDescription->alpha = 0;
		psCtrl->vBusy = 1;
		
		while (psCtrl->txtDisplay->alpha < 100)
		{
			wait (1);
			psCtrl->txtDisplay->alpha    += CTRL_FADESPEED * time_step * 16;
			psCtrl->txtSelection->alpha   = psCtrl->txtDisplay->alpha;
			psCtrl->txtKey->alpha         = psCtrl->txtDisplay->alpha;
			psCtrl->txtDescription->alpha = psCtrl->txtDisplay->alpha;
		}		
		
		psCtrl->txtDisplay->alpha     = 100;
		psCtrl->txtSelection->alpha   = 100;
		psCtrl->txtKey->alpha         = 100;
		psCtrl->txtDescription->alpha = 100;
		psCtrl->vBusy = 0;
	}
	
	/* activate key setup */
	CTRL__setKeys(psCtrl);
	
}

void CTRL_hide(CTRL* psCtrl)
{
	/* do not hide when busy */
	if ((psCtrl->vBusy != 0) || (psCtrl->vVisible == 0)) return;
	
	/* deactivate key setup */
	CTRL__resetKeys(psCtrl);

	if ((psCtrl->vFlags & CTRL_ENABLE_FADE) != 0)
	{
		psCtrl->txtDisplay->alpha     = 100;
		psCtrl->txtSelection->alpha   = 100;
		psCtrl->txtKey->alpha         = 100;
		psCtrl->txtDescription->alpha = 100;
		psCtrl->vBusy = 1;
		
		while (psCtrl->txtDisplay->alpha > 0)
		{
			wait (1);
			psCtrl->txtDisplay->alpha    -= CTRL_FADESPEED * time_step * 16;
			psCtrl->txtSelection->alpha   = psCtrl->txtDisplay->alpha;
			psCtrl->txtKey->alpha         = psCtrl->txtDisplay->alpha;
			psCtrl->txtDescription->alpha = psCtrl->txtDisplay->alpha;
		}		
		
		psCtrl->txtDisplay->alpha     = 0;
		psCtrl->txtSelection->alpha   = 0;
		psCtrl->txtKey->alpha         = 0;
		psCtrl->txtDescription->alpha = 0;
		psCtrl->vBusy = 0;
	}
	
	psCtrl->txtDisplay->flags     &= ~SHOW;
	psCtrl->txtSelection->flags   &= ~SHOW;
	psCtrl->txtKey->flags         &= ~SHOW;
	psCtrl->txtDescription->flags &= ~SHOW;

	psCtrl->vVisible = 0;
}

var CTRL_isActive(CTRL* psCtrl)
{
	/* the CTRL unit is active as long as it is visible on screen.
	 * Not to be confused with the internal flag vActive, which is
	 * used for stopping itnernal loops on object removal only!
	 */
	return psCtrl->vVisible;	
}

void CTRL_setFlags(CTRL* psCtrl, var vFlags)
{
	psCtrl->vFlags = vFlags;
	
	/* text fading */
	if ((vFlags & CTRL_ENABLE_FADE) != 0)
	{
		psCtrl->txtDisplay->flags     |= TRANSLUCENT;
		psCtrl->txtSelection->flags   |= TRANSLUCENT;
		psCtrl->txtKey->flags         |= TRANSLUCENT;
		psCtrl->txtDescription->flags |= TRANSLUCENT;
	}
	else
	{
		psCtrl->txtDisplay->flags     &= ~TRANSLUCENT;
		psCtrl->txtSelection->flags   &= ~TRANSLUCENT;
		psCtrl->txtKey->flags         &= ~TRANSLUCENT;
		psCtrl->txtDescription->flags &= ~TRANSLUCENT;
	}


}

void CTRL_load(CTRL* psCtrl, STRING* strFile)
{
	var vHandle;
	var i;
	var vNumKeys;
	var vRetval;

	vHandle = file_open_read(strFile);

	if (vHandle != 0)
	{
		file_str_read(vHandle, NULL);
		
		/* number of keys - compatibility fallback*/
		vNumKeys = file_var_read(vHandle);
		#ifdef SYSMSG_ACTIVE
		if (vNumKeys != psCtrl->vNumKeys)
		{
			SYSMSG_print(SYSMSG_SHOW_ERROR, "CTRL_load: key config file appears to be old");
		}
		#endif
		vNumKeys = minv(vNumKeys, psCtrl->vNumKeys);
		
		/* key config */
		for (i = 0; i < vNumKeys; i++)
		{
			(psCtrl->vKeyCodes)[i] = file_var_read(vHandle);
		}
	
		file_close(vHandle);
		CTRL__update(psCtrl);
		vRetval = 1;
	}
	else
	{
		#ifdef SYSMSG_ACTIVE
		SYSMSG_print(SYSMSG_SHOW_ERROR, "CTRL_load: cannot open key config file");
		#endif
		vRetval = 0;
	}
	return vRetval;
}

var CTRL_save(CTRL* psCtrl, STRING* strFile)
{
	var vHandle;
	var i;
	STRING* strTemp;
	var vRetval;
		
	vHandle = file_open_write(strFile);

	if (vHandle != 0)
	{
		/* config file comment */
		file_str_write(vHandle, "//Key Config: ");
		strTemp = str_create((psCtrl->txtDisplay->pstring)[0]);
		str_replace(strTemp, ",", "_"); 
		file_str_write(vHandle, strTemp);
		file_str_write(vHandle, "\r\n");
		ptr_remove(strTemp);
		
		/* number of keys */
		file_var_write(vHandle, psCtrl->vNumKeys);
	
		/* key config */
		for (i = 0; i < psCtrl->vNumKeys; i++)
		{
			file_var_write(vHandle, (psCtrl->vKeyCodes)[i]);
		}
	
		file_close(vHandle);
		vRetval = 1;
	}
	else
	{
		#ifdef SYSMSG_ACTIVE
		SYSMSG_print(SYSMSG_SHOW_ERROR, "CTRL_save: cannot open key config file");
		#endif
		vRetval = 0;
	}
	return vRetval;
}


/* ----- INTERNAL FUNCTIONS ----- */

var CTRL__getLength(CTRL* psCtrl, TEXT* txtCtrl) 
{
	int i;
	var vWidth;
	var vMaxWidth;
	
	vMaxWidth = 0;
	vWidth = 0;
	
	/* search for longest string */
	for (i = 0; i < txtCtrl->strings; i++)
	{
		vWidth = str_width((txtCtrl->pstring)[i], psCtrl->fntText);
		if (vWidth > vMaxWidth)
		{
			vMaxWidth = vWidth;	
		}
	}
	
	return vMaxWidth;
}

void CTRL__setKeys(CTRL* psCtrl)
{
	/* store pointer to active control globally as it cannot
	   be passed by functions assigned to keys
	 */
	CTRL_psCurrent = psCtrl;

	/* enable keys */
	on_cuu   = CTRL__up;
	on_cud   = CTRL__down;
	on_enter = CTRL__set;
//	on_esc   = CTRL__resetKeys;
	on_del   = CTRL__remove;
}

void CTRL__resetKeys(CTRL* psCtrl)
{
	/* simple wrapper function for consistent function interface */
	CTRL__resetKeys();
}

void CTRL__resetKeys()
{
	/* reset global control pointer */
	CTRL_psCurrent = NULL;

	/* disable keys */
	on_cuu   = NULL;
	on_cud   = NULL;
	on_enter = NULL;
//	on_esc   = NULL;
	on_del   = NULL;
}

void CTRL__remove()
{
	CTRL* psCtrl;
	
	if (CTRL_psCurrent != NULL)
	{
		psCtrl = CTRL_psCurrent;
		/* remove key code */
		(psCtrl->vKeyCodes)[psCtrl->vSelected] = CTRL_UNDEFINED;
		/* move to next key */
		CTRL__down();
	}
}

void CTRL__down()
{
	if (CTRL_psCurrent != NULL)
	{
		/* select next key */
		CTRL_psCurrent->vSelected++;
		if (CTRL_psCurrent->vSelected >= CTRL_psCurrent->vNumKeys)
		{
			CTRL_psCurrent->vSelected = 0;	
		}
		/* update display */
		CTRL__update(CTRL_psCurrent);
	}
}

void CTRL__up()
{
	if (CTRL_psCurrent != NULL)
	{
		/* select previous key */
		CTRL_psCurrent->vSelected--;
		if (CTRL_psCurrent->vSelected < 0)
		{
			CTRL_psCurrent->vSelected = CTRL_psCurrent->vNumKeys - 1;	
		}
		/* update display */
		CTRL__update(CTRL_psCurrent);
	}
}

void CTRL__set()
{
	CTRL* psCtrl;
	var vLock;
		
	if (CTRL_psCurrent != NULL)
	{
		psCtrl = CTRL_psCurrent;
		/* lock controls */
		CTRL__resetKeys(psCtrl);

		/* wait for all keys being released */
		while(key_any != 0)
		{
			wait (1);
		}

		/* wait for input */
		str_cpy((psCtrl->txtKey->pstring)[psCtrl->vSelected], "???"); 
		str_cpy((psCtrl->txtSelection->pstring)[psCtrl->vSelected], (psCtrl->txtKey->pstring)[psCtrl->vSelected]); 

		do
		{
			vLock = CTRL_UNLOCKED;
			while(key_any == 0)
			{
				wait (1);
			}
			/* check whether pressed key is not allowed */
			if (	(
						   ((psCtrl->vFlags & CTRL_DENY_MOUSE) != 0) /* mouse */
						&& (key_lastpressed >= CTRL_MOUSE_START) 
						&& (key_lastpressed <= CTRL_MOUSE_END) 
					) 
					|| (
						   ((psCtrl->vFlags & CTRL_DENY_JOY1) != 0) /* joy 1 */
						&& (key_lastpressed >= CTRL_JOY1_START)
						&& (key_lastpressed <= CTRL_JOY1_END) 
					)
					|| (
						   ((psCtrl->vFlags & CTRL_DENY_JOY2) != 0) /* joy 2 */
						&& (key_lastpressed >= CTRL_JOY2_START)
						&& (key_lastpressed <= CTRL_JOY2_END) 
					)
					|| (
						   ((psCtrl->vFlags & CTRL_DENY_KEY) != 0) /* keyboard */
						&& (key_lastpressed >= CTRL_KEY_START)
						&& (key_lastpressed <= CTRL_KEY_END) 
					)
			   )
			{
				wait (1);
				vLock = CTRL_LOCKED;
			}
		} while (vLock == CTRL_LOCKED);

		if (key_lastpressed != 1) /* ESC pressed? - ignore */
		{
			/* assign key code */
			(psCtrl->vKeyCodes)[psCtrl->vSelected] = key_lastpressed;
		}

		/* wait for all keys being released */
		while(key_any != 0)
		{
			wait (1);
		}

		/* unlock controls */
		CTRL__setKeys(psCtrl);
		/* move to next key */
		CTRL__down();
	}
}

void CTRL__update(CTRL* psCtrl)
{
	int i;
	var vScancode;
	
	/* update key description */
	for (i = 0; i < psCtrl->vNumKeys; i++)
	{
		vScancode = (psCtrl->vKeyCodes)[i];
		/* copy scancode description */
		str_cpy((psCtrl->txtKey->pstring)[i], (CTRL_txtKeymap->pstring)[vScancode]); 

		/* only copy selected key, hide all others
		 * could be done in a faster way by just having a single line text
		 * and moving it to the right position, but this leads to some
		 * calculations based on letter size - which might go wrong.
		 * speed is not the main issue for a key setup menu
		 */
		if (i == psCtrl->vSelected)
		{
			str_cpy((psCtrl->txtSelection->pstring)[i], (psCtrl->txtKey->pstring)[i]); 
		}
		else
		{
			str_cpy((psCtrl->txtSelection->pstring)[i], " "); 
		}
	}
	psCtrl->vForceUpdate = 1;
}

void CTRL__initialize()
{
	var i;
	STRING* strTemp = "";
	var vHandle;
	
	/* initialization is only done one time */
	if (CTRL_vInitialized == 0)
	{
		/* add dummy string for index alignment with key scancode (this is also init value) */
		txt_addstring(CTRL_txtKeymap, "UNDEFINED");

		vHandle = file_open_read ("keymap.dat"); 
		if (vHandle != NULL)
		{
			i = 1;
			while(file_str_read (vHandle, strTemp) != -1)
			{
				/* use key id if string is empty or '.' */
				if (str_cmpi (strTemp, ".") || str_cmpi (strTemp, ""))
				{
					str_for_num(strTemp, i);
					txt_addstring(CTRL_txtKeymap, strTemp);
					i++; 
				}
				/* ignore lines commented with '//' */
				else if (str_stri(strTemp, "//") == 0)
				{
					txt_addstring(CTRL_txtKeymap, strTemp);
					i++;
				}
			}
			file_close (vHandle);
		}
		else
		/* no keymap file found, just fill text with default labels */
		{
			#ifdef SYSMSG_ACTIVE
			SYSMSG_print(SYSMSG_SHOW_DEBUG, "CTRL__initialize: keymap file not found");
			#endif
			
			/* 400 strings should be enough... */
			for (i = 1; i < 400; i++)
			{
				str_for_key(strTemp, i);
				txt_addstring(CTRL_txtKeymap, strTemp);
			}
		}
		/* lock init routine against multiple execution */
		CTRL_vInitialized = 1;
	}
}

void CTRL__loop(CTRL* psCtrl)
{
	while(psCtrl->vActive == 1)
	{
		/* only perform actions if control setup is visible */
		if (psCtrl->vVisible != 0)
		{
			/* update layout if necessary */
			CTRL__checkLayout(psCtrl);
		}
		/* only perform actions if control setup is NOT visible */
		else
		{
			/* poll pressed keys */
			CTRL__checkKeyPresses(psCtrl);
		}
		
		wait (1);	
	}
}

void CTRL__checkKeyPresses(CTRL* psCtrl)
{
	var i;
	
	for (i = 0; i < psCtrl->vNumKeys; i++)
	{
		/* function assigned? */
		if ((psCtrl->pfFunc)[i] != NULL)
		{
			/* key was pressed last time? */
			if (key_pressed((psCtrl->vKeyCodes)[i]) == 1)
			{
				/* assigned key function was already executed? */
				if ((psCtrl->vKeyLocks)[i] == CTRL_UNLOCKED)
				{
					CTRL__pfKeyPress = (psCtrl->pfFunc)[i];
					/* any function assigned to key? */
					if (CTRL__pfKeyPress != NULL)
					{
						/* execute and lock */
						CTRL__pfKeyPress();
						(psCtrl->vKeyLocks)[i] = CTRL_LOCKED;
					}
				}
			}
			/* remove lock if different key or no key at all was pressed */
			else
			{
				/* different key was pressed - unlock this function again */
				(psCtrl->vKeyLocks)[i] = CTRL_UNLOCKED;
			}
		}
	}
}

void CTRL__checkLayout(CTRL* psCtrl)
{
	var vWidth1;
	var vWidth2;
	var vHeight;
	var vScaleX;
	var vScaleY;
	var vPosX;
	var vPosY;
	
	/* only update in case of resolution change or if explicitly requested after text content update */
	if ((psCtrl->vResX != screen_size.x) || (psCtrl->vResY != screen_size.y) || (psCtrl->vForceUpdate != 0))
	{
		psCtrl->vForceUpdate = 0;
		
		/* update internal resolutions */
		psCtrl->vResX = screen_size.x;
		psCtrl->vResY = screen_size.y;

		/* make sure text size is same for all resolutions */
		vScaleX = screen_size.x / (CTRL_SCALEFACTOR * CTRL_BASE_RES_X);
		vScaleY = screen_size.y / (CTRL_SCALEFACTOR * CTRL_BASE_RES_Y);
		
		/* title text is centered */
		psCtrl->txtDisplay->pos_x = screen_size.x / 2;
		psCtrl->txtDisplay->scale_x = vScaleX;
		psCtrl->txtDisplay->scale_y = vScaleY;
			
		psCtrl->txtKey->scale_x = vScaleX;
		psCtrl->txtKey->scale_y = vScaleY;
	
		psCtrl->txtDescription->scale_x = vScaleX;
		psCtrl->txtDescription->scale_y = vScaleY;
	
		psCtrl->txtSelection->scale_x = vScaleX;
		psCtrl->txtSelection->scale_y = vScaleY;

		/* now put everything in place */
		vHeight = psCtrl->fntText->dy * vScaleY;
	
		psCtrl->txtDisplay->pos_y = CTRL_TEXTOFFS_Y * vScaleY;
		psCtrl->txtKey->pos_y = psCtrl->txtDisplay->pos_y + 2 * vHeight;	
		psCtrl->txtDescription->pos_y = psCtrl->txtKey->pos_y;	
		psCtrl->txtSelection->pos_y = psCtrl->txtKey->pos_y;	
	
		vWidth1 = CTRL__getLength(psCtrl, psCtrl->txtDescription) * vScaleX;
		vWidth2 = CTRL__getLength(psCtrl, CTRL_txtKeymap) * vScaleX;
		psCtrl->txtDescription->pos_x = (screen_size.x - (vWidth1 + vWidth2 + 15 * vScaleX)) / 2;
		psCtrl->txtKey->pos_x = psCtrl->txtDescription->pos_x + vWidth1 + vWidth2 / 2 + 15 * vScaleX;
		psCtrl->txtSelection->pos_x = psCtrl->txtKey->pos_x;
	}
}
