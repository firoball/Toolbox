/*
 *******************************************************************************
 * camctrl.h
 * Creation date: 04.11.2008
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
 * Definition script for camera control
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */
#ifndef CAMCTRL_H
#define CAMCTRL_H
/*! \file 
 *  Include file for camera/view controller.
 */


/* ----- INCLUDES ----- */


/* ----- DEFINITIONS ----- */

#define CAMCTRL_ACTIVE	/*!< This define can be evaluated to check if this module is included and active */

/*! \name Configuration Data 
 *  The values of these parameters can be overwritten by defining them before including this file.
 *  Depending on the used camera type, more configuration parameters are available.
 *  See following references (check description of camera modes for more information)
 *
 *  - camctrl_1stperson.h
 *  - camctrl_follow.h
 *  - camctrl_chase.h
 *  - camctrl_lookat.h
 *  
 *  \note
 *  These files are automatically included when required. Never include them manually in any project.
 *
 * Additionally following generic configuration parameters are available independent from used camera type:
 * \{ */
#ifndef CAMCTRL_DEFLAYER
#define CAMCTRL_DEFLAYER     20  /*!< default layer of view if default CAMERA view is not used */
#endif

#ifndef CAMCTRL_DEFDIST
#define CAMCTRL_DEFDIST      -300/*!< default x distance of camera */
#endif

#ifndef CAMCTRL_DEFSPEED
#define CAMCTRL_DEFSPEED     5   /*!< default movement speed of camera */
#endif

#ifndef CAMCTRL_DEFSMOOTHFAC
#define CAMCTRL_DEFSMOOTHFAC 1.0 /*!< default smoothing factor for camera movement */
#endif

/* \} */


/*! \name Constant Data 
 *  These constants are used as parameters for certain functions.
 *  List of available camera modes. Each mode needs some settings to be
 *  initialized before the corresponding camera mode can be set active.
 *  
 * \{ */
#define CAMCTRL_1STPERSON    0   
/*!< 1st person perspective for given entity. 
                                      Required initialization calls:
                                      - CAMCTRL_setEntity()\n
                                      
                                      Optional initialization calls:
                                      - CAMCTRL_setAngle()
                                  */
#define CAMCTRL_CHASE        1   
/*!< follow entity with dist, auto adjust angle to entity angle.
                                      Required initialization calls:
                                      - CAMCTRL_setEntity()\n
                                      
                                      Optional initialization calls:
                                      - CAMCTRL_setDist()
                                      - CAMCTRL_setAngle()
                                  */
#define CAMCTRL_FIXED        2   /* fixed view position, no automatic movement */
#define CAMCTRL_LOOKAT       3   
/*!< fixed view position, auto adjust view angle to follow given position. 
                                      Required initialization calls:
                                      - CAMCTRL_setEntity()\n
*/
#define CAMCTRL_FOLLOW       4   
/*!< follow entity with dist, no auto adjust of view angle. 
                                      Required initialization calls:
                                      - CAMCTRL_setEntity()\n

                                      Optional initialization calls:
                                      - CAMCTRL_setDist()
                                      - CAMCTRL_setAngle()
                                  */
#define CAMCTRL_INACTIVE     5   /*!< camera control is inactive */
/* \} */


/* ----- STRUCTURES ----- */

/*! Definition of a camera control. 
 *  This structure carries all information required for certain camera modes.
 *  In order to assign custom values, use the CAMTRL_setxxx functions.
 *  The interpretation of the given values depends on the selected camera mode.
 */
typedef struct
{
	VIEW*   pviewCam;			/*!< pointer to used VIEW */
	VECTOR* pvecTargetPos; 	/*!< view position */
	VECTOR* pvecLookAtPos;	/*!< position view looks at */
	VECTOR* pvecViewDist;	/*!< distance to target object */
	ENTITY* pentGenius;		/*!< pointer to parent entity */
	ENTITY* pentViewMover;	/*!< entity, view is attached to, for movement calculations */ 
	ANGLE*  pangOffset;		/*!< viewing angle offset */
	ANGLE*  pangTarget;		/*!< target viewing angle */
	var     vMode;				/*!< activated view mode */
	var     vLastMode;		/*!< last activated view mode */
	var     vSmoothing;		/*!< smoothing factor for view angle adjustments */
	var     vSpeed;			/*!< movement speed */
	var     vViewCreated;	/*!< flag: VIEW created by CAMCTRL or not? */
	var     vCameraLocked;  /*!< flag: Camera mode locked due to faulty initialization? */
	
	VECTOR  vecDefaultDist; /*!< default distance if no distance was specified by user */
	VECTOR  vecTemp;        /*!< multi purpose vector used by certain camera modes */
	var     vTemp;          /*!< multi purpose variable used by certain camera modes */
}CAMCTRL;


/* ----- EXTERNAL FUNCTIONS ----- */

/*! Create camera control.
 *  \return Pointer to CAMCTRL structure
 */
CAMCTRL* CAMCTRL_create(void);

/*! Create camera control using predefined view.
 *  \param	pviewCam Pointer to VIEW
 *  \return Pointer to CAMCTRL structure
 */
CAMCTRL* CAMCTRL_create(VIEW* pviewCam);

/*! Remove camera control.
 *  \param	psHandle Pointer to camera control
 */
void CAMCTRL_remove(CAMCTRL* psHandle);

/*! Show view of camera control .
 *  \param	psHandle Pointer to camera control
 */
void CAMCTRL_show(CAMCTRL* psHandle);

/*! Hide view of camera control.
 *  \param	psHandle Pointer to camera control
 */
void CAMCTRL_hide(CAMCTRL* psHandle);

/*! Set camera mode of camera control.
 *  \param	psHandle Pointer to camera control
 *  \param	vMode Camera mode (use predefined CAMCTRL_xxx camera modes)
 */
void CAMCTRL_setMode(CAMCTRL* psHandle, var vMode);

/*! Set camera distance to target position.
 *  Caution: When the given vector is changed somewhere else in the code,
 *  this will affect the camera. Can be used for additional effects. 
 *  \param	psHandle Pointer to camera control
 *  \param	pvecDist Pointer to distance vector
 */
void CAMCTRL_setDist(CAMCTRL* psHandle, VECTOR* pvecDist);

/*! Set camera viewing angle offset.
 *  Caution: When the given angle is changed somewhere else in the code,
 *  this will affect the camera. Can be used for additional effects. 
 *  \param	psHandle Pointer to camera control
 *  \param	pangDir Pointer to offset angle
 */
void CAMCTRL_setAngle(CAMCTRL* psHandle, ANGLE* pangDir);

/*! Set camera position to position given by vector.
 *  Caution: When the given vector is changed somewhere else in the code,
 *  this will affect the camera. Can be used for additional effects. 
 *  \param	psHandle Pointer to camera control
 *  \param	pvecPos Pointer to position vector
 */
void CAMCTRL_setPos(CAMCTRL* psHandle, VECTOR* pvecPos);

/*! Attach Entity to camera control.
 *  \param	psHandle Pointer to camera control
 *  \param	pentFocus Pointer to entity focused by view
 */
void CAMCTRL_setEntity(CAMCTRL* psHandle, ENTITY* pentFocus);

/*! Set smoothing factor for camera movement.
 *  \param	psHandle Pointer to camera control
 *  \param	vFac Factor (0 slow change .. 1 fast change)
 */
void CAMCTRL_setSmoothFac(CAMCTRL* psHandle, var vFac);

/*! Set speed for camera movement.
 *  \param	psHandle Pointer to camera control
 *  \param	vSpeed Speed per tick
 */
void CAMCTRL_setSpeed(CAMCTRL* psHandle, var vSpeed);

/*! Get view entity of camera control.
 *  \param	psHandle Pointer to camera control
 *  \return Pointer to VIEW of camera control
 */
VIEW* CAMCTRL_getView(CAMCTRL* psHandle);


/* ----- INTERNAL FUNCTIONS ----- */

/*! \internal - Do not use! */
void CAMCTRL__move_loop(CAMCTRL* psHandle);
/*! \internal - Do not use! */
void CAMCTRL__mode_init(CAMCTRL* psHandle);
/*! \internal - Do not use! */
void CAMCTRL__mode_deinit(CAMCTRL* psHandle);

#include "camctrl.c"


#endif