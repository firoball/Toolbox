/*
 *******************************************************************************
 * control.h
 * Creation date: 13.10.2009
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
 * Definition script for key control
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */

#ifndef CONTROL_H
#define CONTROL_H
/*! \file */

/* ----- INCLUDES ----- */



/* ----- DEFINITIONS ----- */

#define CONTROL_ACTIVE	/*!< This define can be evaluated to check if this module is included and active */

/*! \name Configuration Data 
 *  The values of these parameters can be overwritten by defining them before including this file.
 * \{ */
#ifndef CTRL_LAYER
#define CTRL_LAYER 20 	/*!< layer for text display */
#endif

#ifndef CTRL_TEXTCOL_RED
#define CTRL_TEXTCOL_RED 255 /*!< display text color: red component */
#endif

#ifndef CTRL_TEXTCOL_GREEN
#define CTRL_TEXTCOL_GREEN 255 /*!< display text color: green component */
#endif

#ifndef CTRL_TEXTCOL_BLUE
#define CTRL_TEXTCOL_BLUE 255 /*!< display text color: blue component */
#endif

#ifndef CTRL_TEXTCOL_SEL_RED
#define CTRL_TEXTCOL_SEL_RED 255 /*!< display selected text color: red component */
#endif

#ifndef CTRL_TEXTCOL_SEL_GREEN
#define CTRL_TEXTCOL_SEL_GREEN 255 /*!< display selected text color: green component */
#endif

#ifndef CTRL_TEXTCOL_SEL_BLUE
#define CTRL_TEXTCOL_SEL_BLUE 0 /*!< display selected text color: blue component */
#endif

#ifndef CTRL_SCALEFACTOR
#define CTRL_SCALEFACTOR 1.0	/*!< global scale factor for text display */ 
#endif

#ifndef CTRL_BASE_RES_X
#define CTRL_BASE_RES_X 640 /*!< display reference resolution for x scaling */
#endif

#ifndef CTRL_BASE_RES_Y
#define CTRL_BASE_RES_Y 480 /*!< display reference resolution for y scaling */
#endif

#ifndef CTRL_TEXTOFFS_Y
#define CTRL_TEXTOFFS_Y 20	/*!< y offset of text menu (including title) */
#endif

#ifndef CTRL_FADESPEED
#define CTRL_FADESPEED 1.5 /*!< text fading speed in seconds (only used when text fading is activated) */
#endif
/* \} */


/*! \name Constant Data 
 *  These constants are used as parameters for certain functions.
 * \{ */
#define CTRL_UNDEFINED 0

#define CTRL_UNLOCKED 0
#define CTRL_LOCKED   1

#define CTRL_MOUSE_START 280 /*!< scancode range for mouse buttons - start */
#define CTRL_MOUSE_END   282 /*!< scancode range for mouse buttons - end */
#define CTRL_JOY1_START  256 /*!< scancode range for joystick1 buttons - start */
#define CTRL_JOY1_END    267 /*!< scancode range for joystick1 buttons - end */
#define CTRL_JOY2_START  268 /*!< scancode range for joystick2 buttons - start */
#define CTRL_JOY2_END    279 /*!< scancode range for joystick2 buttons - end */
#define CTRL_KEY_START     1 /*!< scancode range for keyboard buttons - start */
#define CTRL_KEY_END     255 /*!< scancode range for keyboard buttons - end */

#define CTRL_ENABLE_FADE   1 /*!< used for CTRL_setFlags: enables fading for menu visibility toggling */
#define CTRL_DENY_MOUSE    2 /*!< used for CTRL_setFlags: do not allow mouse buttons for key assignment */
#define CTRL_DENY_JOY1     4 /*!< used for CTRL_setFlags: do not allow joystick1 buttons for key assignment */
#define CTRL_DENY_JOY2     8 /*!< used for CTRL_setFlags: do not allow joystick2 buttons for key assignment */
#define CTRL_DENY_KEY     16 /*!< used for CTRL_setFlags: do not allow keyboard keys for key assignment */
/* \} */

/* ----- STRUCTURES ----- */


/*! Definition of a control unit. 
 *  Use the CONTROL_create function for creation of a unit.
 *  Several control units can be defined for different players, but only one should be active at a time.
 */
typedef struct
{
	var vNumKeys;     		/*!< Numbers of assignable keys */
	var* vKeyCodes;   		/*!< Array of assigned key codes */
	var* vKeyLocks;   		/*!< Array of locks for assigned key codes */
	void** pfFunc;     		/*!< Array of assigned key press functions */
	FONT* fntText;    		/*!< Font for interface display */ 
	TEXT* txtDisplay;			/*!< pointer to text display */
	TEXT* txtSelection;		/*!< pointer to text showing selected option */
	TEXT* txtKey;       		/*!< pointer to text showing assigned key */
	TEXT* txtDescription; 	/*!< pointer to text showing key description */
	var vSelected;				/*!< Currently selected key index */
	var vActive;        		/*!< Struct instance currently active (operating) */
	var vVisible; 				/*!< Control unit is currently visible */
	var vResX; 					/*!< internal storage for detection of resolution change */
	var vResY; 					/*!< internal storage for detection of resolution change */
	var vBusy; 					/*!< 0: control object is ready for use	1: control object is temporarily locked */
	var vFlags; 				/*!< internal storage of behaviour flags */
	var vForceUpdate; 		/*!< 0: no effect 1: Force layout update */
}CTRL;


/* ----- EXTERNAL FUNCTIONS ----- */

/*! Creates new key control manager.
 *  Use CTRL_setDescription and CTRL_setFunction to configure state manager.
 *  Configuration must be applied during project initialization before usage of control
 *  manager.
 *  \param	vNumKeys number of reconfigurable keys
 *  \param  strTitle title of key control setup menu
 *  \param  strFont filename of bitmap font or name of Truetype font 
 *  \param  vTextFlags any bit combination of key control menu flags (CTRL_ENABLE_..., CTRL_DENY_...)
 *  \return	Pointer to CTRL structure
 */
CTRL* CTRL_create(var vNumKeys, STRING* strTitle, STRING* strFont, var vTextFlags);

/*! Creates new Control Manager.
 *  Use CTRL_setDescription and CTRL_setFunction to configure state manager.
 *  Configuration must be applied during project initialization before usage of control
 *  manager.
 *  Uses default font and default behaviour flags.
 *  \param	vNumKeys number of reconfigurable keys
 *  \param  strTitle title of key control setup menu
 *  \return	Pointer to CTRL structure
 */
CTRL* CTRL_create(var vNumKeys, STRING* strTitle);

/*! Remove key control manager.
 *  \return	Pointer to CTRL structure
 */
void CTRL_remove(CTRL* psCtrl);

/*! Apply description 
 *  \param	psCtrl pointer to CTRL structure
 *  \param	vIndex index of reconfigurable key
 *  \param	strDesc description/name of reconfigurable key displayed in menu
 */
void CTRL_setDescription(CTRL* psCtrl, var vIndex, STRING* strDesc);

STRING* CTRL_getDescription(CTRL* psCtrl, var vIndex);

void CTRL_setFunction(CTRL* psCtrl, var vIndex, void* pfFunc);

var CTRL_isPressed(CTRL* psCtrl, var vIndex);

/*! Show key control layout.
 *  Shows the key control layout of the given pointer. Use CTRL_setFlags() to setup additional 
 *  display features like fading.
 *  \param	psCtrl pointer to CTRL structure
 */
void CTRL_show(CTRL* psCtrl);

/*! Hide key control layout.
 *  Hide the key control layout of the given pointer. Use CTRL_setFlags() to setup additional 
 *  display features like fading.
 *  \param	psCtrl pointer to CTRL structure
 */
void CTRL_hide(CTRL* psCtrl);

/*! Check visibility key control layout.
 *  \param	psCtrl pointer to CTRL structure
 */
void CTRL_isActive(CTRL* psCtrl);

/*! Set text flags.
 *  Set any flag CTRL_ENABLE_..., CTRL_DENY_... to influence behaviour of the control setup.
 *  \param	psCtrl pointer to CTRL structure
 *  \param	vFlags any valid TEXT flag
 */
void CTRL_setFlags(CTRL* psCtrl, var vFlags);

/*! Loads key coniguration from disk.
 *  Load key configuration with given filename.
 *  \param	psCtrl pointer to CTRL structure
 *  \param	strFile Filename of config file
 *  \return	1: successsful - 0: failure
 */
var CTRL_load(CTRL* psCtrl, STRING* strFile);

/*! Saves current key coniguration.
 *  Store key configuration with given filename.
 *  \param	psCtrl pointer to CTRL structure
 *  \param	strFile Filename of config file
 *  \return	1: successsful - 0: failure
 */
var CTRL_save(CTRL* psCtrl, STRING* strFile);

/* ----- INTERNAL FUNCTIONS ----- */

/*! \internal - Do not use! */
var CTRL__getLength(CTRL* psCtrl, TEXT* txtCtrl); 
/*! \internal - Do not use! */
void CTRL__setKeys(CTRL* psCtrl);
/*! \internal - Do not use! */
void CTRL__resetKeys(CTRL* psCtrl);
/*! \internal - Do not use! */
void CTRL__resetKeys();
/*! \internal - Do not use! */
void CTRL__remove();
/*! \internal - Do not use! */
void CTRL__down();
/*! \internal - Do not use! */
void CTRL__up();
/*! \internal - Do not use! */
void CTRL__set();
/*! \internal - Do not use! */
void CTRL__update(CTRL* psCtrl);
/*! \internal - Do not use! */
void CTRL__initialize();
/*! \internal - Do not use! */
void CTRL__loop(CTRL* psCtrl);
/*! \internal - Do not use! */
void CTRL__checkKeyPresses(CTRL* psCtrl);
/*! \internal - Do not use! */
void CTRL__checkLayout(CTRL* psCtrl);

#include "control.c"


#endif
