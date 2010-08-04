/*
 *******************************************************************************
 * state.h
 * Creation date: 29.12.2009
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
 * Definition script for State Manager
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */

#ifndef STATE_H
#define STATE_H
/*! \file 
 *  Include file for state manager.
 */

/* ----- INCLUDES ----- */


#include "..\list\list.h"


/* ----- DEFINITIONS ----- */


#define STATE_ACTIVE	/*!< This define can be evaluated to check if this module is included and active */


/* ----- STRUCTURES ----- */

/*! Definition of a state transition.
 *  All added state transitions are checked cyclically. Use STATEMGR_addTransition to add transitions.
 */
typedef struct
{
	var vCurState;  /*!< Current state */
	var vNewState;  /*!< Next state */
	void* pfCheck;  /*!< Pointer to transition check function */
	void* pfSwitch; /*!< Pointer to transition action function */
}STATE_TRAN;


/*! Definition of a state action.
 *  All added state actions are called cyclically as long as the corresponding state is active. 
 *  Use STATEMGR_addAction to add actions.
 */
typedef struct
{
	var vState;     /*!< State */
	void* pfAction; /*!< Pointer to correnspoing cyclic function */	
	void* pfInit;   /*!< Pointer to initialization function */
	void* pfDeinit; /*!< Pointer to deinitialization function */
}STATE_ACTION;


/*! Definition of a state machine.
 *  This structure contains the basic setup of the state machine. State transitions are defined as a
 *  struct of type STATE_TRAN. State actions (cyclically called) are defined as a struct of
 *  type STATE_ACTION.
 *  Several differently configured state machines can be active at the same time.
 */
typedef struct
{
	var vState;          /*!< Current state */
	void* pfAction;      /*!< Pointer to correnspoing cyclic function */
	void* pfInit;        /*!< Pointer to initialization function */
	void* pfDeinit;      /*!< Pointer to deinitialization function */
	LIST* psTransitions; /*!< LIST containing  all possible transitions between states */
	LIST* psActions;     /*!< LIST containing all cyclic functions for all states */
}STATEMGR;


/* ----- EXTERNAL FUNCTIONS ----- */


/*! Creates new State Manager.
 *  Use STATEMGR_addTransition and STATEMGR_addAction to configure state manager.
 *  \param	vState initial state
 *  \return	Pointer to STATEMGR structure
 */
STATEMGR* STATEMGR_create(var vState);


/*! Removes a State Machine.
 *  Frees all memory previously allocated by the given state machine. 
 *  \param	psState Pointer to affected STATEMGR
 */
void STATEMGR_remove(STATEMGR* psState);


/*! Add new transition to State Machine.
 *  A new state transition with check condition callback and tranistion action callback is added.
 *  Transitions can only be added, but not removed. 
 *  \param	psState Pointer to affected STATEMGR
 *  \param	vCurState Current state
 *  \param	vNewState Next state
 *  \param	pfCheck Check condition callback function (type: var func(void) - return value 1 = true, 0 = false)
 *  \param	pfSwitch Pointer to transition action callback function (type void func(void)) [optional: may be set to NULL]
 */
void STATEMGR_addTransition(STATEMGR* psState, var vCurState, var vNewState, void* pfCheck, void* pfSwitch);
 

/*! Add new state to State Machine.
 *  A new state with init, deinit and cyclical callback functions is added.
 *  States can only be added, but not removed.
 *  Each state may only be added once. If a state is added multiple times, only the first definition will be recognized. 
 *  \param	psState Pointer to affected STATEMGR
 *  \param	vCurState Current state
 *  \param	pfAction Pointer to cyclical action callback function (type void func(void)) [optional: may be set to NULL]
 *  \param	pfInit Pointer to initialization function (called on state entry - type void func(void)) [optional: may be set to NULL]
 *  \param	pfDeinit Pointer to deinitialization function (called on state exit - type void func(void)) [optional: may be set to NULL]
 */
void STATEMGR_addState(STATEMGR* psState, var vCurState, void* pfAction, void* pfInit, void* pfDeinit);


/*! Execute cyclical actions and transition checks for State Machine.
 *  Must be called cyclically.
 *  \param	psState Pointer to affected STATEMGR
 */
void STATEMGR_execute(STATEMGR* psState);


/*! Get currently active state.
 *  Should only be used for stopping loop functions and never for if checks.
 *  \param	psState Pointer to affected STATEMGR
 *  \return	Current state
 */
var STATEMGR_getState(STATEMGR* psState);

 
/* ----- INTERNAL FUNCTIONS - DO NOT USE ----- */


/*! \internal - Do not use! */
void STATEMGR__checkTransitions(STATEMGR* psState);
/*! \internal - Do not use! */
void STATEMGR__doTransition(STATEMGR* psState, STATE_TRAN* psTran);
/*! \internal - Do not use! */
void STATEMGR__getActions(STATEMGR* psState);
/*! \internal - Do not use! */
void STATEMGR__updateActions(STATEMGR* psState, STATE_ACTION* psAction);


#include "state.c"


#endif 