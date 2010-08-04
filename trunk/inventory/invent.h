/*
 *******************************************************************************
 * invent.h
 * Creation date: 02.05.2009
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
 * Definition script for Inventory
 *
 * Comments
 * 
 * for short descriptions see comments in this file
 *
 *******************************************************************************
 */

#ifndef INVENT_H
#define INVENT_H
/*! \file 
 *  Include file for inventory functions.
 */

/* ----- INCLUDES ----- */


/* ----- DEFINITIONS ----- */

#define INVENT_ACTIVE	/*!< This define can be evaluated to check if this module is included and active */

/*! \name Configuration Data 
 *  The values of these parameters can be overwritten by defining them before including this file.
 * \{ */
#ifndef INV_PANELDIST
#define INV_PANELDIST 5 	/*!< Distance between inventory panel and screen border */
#endif

#ifndef INV_FADESPEED
#define INV_FADESPEED 25  	/*!< Fading speed for show/hide of inventory (= INV_ALPHA/(fadetime*16) */
#endif

#ifndef INV_ALPHA
#define INV_ALPHA 90			/*!< Maximum alpha value of inventory */
#endif

#ifndef INV_SCALE
#define INV_SCALE 1.0		/*!< Maximum scale value of inventory */
#endif
/* \} */


/*! \name Constant Data 
 *  These constants are used as parameters for certain functions.
 * \{ */
#define INV_UNLIMITEDITEMS  0
#define INV_UNLIMITEDWEIGHT 0
#define INV_UNLIMITEDAMOUNT 0

#define INV_ALIGN_LEFT   1
#define INV_ALIGN_RIGHT  2
#define INV_ALIGN_TOP    4
#define INV_ALIGN_BOTTOM 8
/* \} */

/* ----- STRUCTURES ----- */


/*! Definition of an inventory item. 
 *  Each object which is supposed to be added to the INVENTORY has to make use of this structure.
 *  Use the INVITEM_create function for creation of a new item.
 */
typedef struct
{
	var vWeight;	 /*!< Weight of item. May not exceed maximum weight of INVENTORY */
	var vAmount;	 /*!< Amount of item. May not exceed maximum amount of INVENTORY */
	var vStackable; /*!< Allow item to be stacked. Some items should not be stackable (e.g. items with random stats like magic weapons) */
	long lItemId;	 /*!< Id of item. The item Id should be unique */

	BMAP* pbmapPicture; /*!< Bitmap picture of item */
	void* psExt;        /*!< Pointer to user defined struct or variable. 
	                         Attach game specific item properties here (e.g. stats of magic items) */
	void* pfExtRemove;  /*!< Pointer to specific item removal function.
	                        Attach game specific item property removal function here if e.g. deallocation
	                        for the attached object is required. 
	                        Function parameter: type void* - Pointer to external properties */
	void* psNext;	/*!< Pointer to next INVITEM in INVENTORY. Items are arranged in a ring buffer */
	void* psPrev;	/*!< Pointer to previous INVITEM in INVENTORY. Items are arranged in a ring buffer */
}INVITEM;

/*! Definition of an inventory.
 *  This structure contains the basic setup of the inventory. Items are added as a
 *  linked list of type INVITEM.
 *  Several differently configured inventories can be active at the same time.
 */
typedef struct
{
	var vMaxItems;		/*!< Maximum number of different items fitting into inventory */
	var vMaxWeight;	/*!< Maximum weight of items fitting into inventory */
	var vMaxAmount;	/*!< Maximum amount per item fitting into inventory */

	var vCurItems;		/*!< Number of items currently in inventory */
	var vCurWeight;	/*!< Weight of items currently in inventory */

	var vAmountCur;	/*!< Used for display of amount of currently selected item */
	var vWeightCur;	/*!< Used for display of weight of currently selected item */
	
	INVITEM* psCurItem;	/*!< Pointer to currently selected item */
	
	var vAlignment;	/*!< Alignment to borders of screen */
	BMAP* pbmapBackdrop;	/*!< Pointer to background bitmap */
	PANEL* ppanCur;	/*!< Pointer to backdrop panel */
	PANEL* ppanItem;	/*!< Pointer to panel of current item */
	var vPosX;			/*!< x position of panel, relative to vAlignment */
	var vPosY;			/*!< y position of panel, relative to vAlignment */
	var vFadeActive;  /*!< Fading (show/hide inventory) currently in process */
}INVENTORY;


/* ----- EXTERNAL FUNCTIONS ----- */


/*! Creates new Inventory.
 *  Initialize a new inventory with the given background bitmap and the given layer.
 *  \param	strBmap file name of background bitmap
 *  \param	vLayer  layer value of inventory panels
 *  \return	Pointer to INVENTORY structure
 */
INVENTORY* INVENTORY_create(STRING* strBmap, var vLayer);


/*! Removes an Inventory.
 *  Frees all memory previously allocated by the given inventory. 
 *  \param	psInv Pointer to affected INVENTORY
 */
void INVENTORY_remove(INVENTORY* psInv);


/*! Show Inventory on screen.
 *  Inventory will smoothly fade in. Fade speed depends on INV_FADESPEED and INV_ALPHA
 *  \param	psInv Pointer to affected INVENTORY
 */
void INVENTORY_show(INVENTORY* psInv);


/*! Hide Inventory from screen. 
 *  Inventory will smoothly fade out. Fade speed depends on INV_FADESPEED and INV_ALPHA
 *  \param	psInv Pointer to affected INVENTORY
 */
void INVENTORY_hide(INVENTORY* psInv);


/*! Check whether inventory is visible. 
 *  \param	psInv Pointer to affected INVENTORY
 *  \return 0: invisible		1: visible
 */
var INVENTORY_isVisible(INVENTORY* psInv);

/*! set position of inventory window. 
 *  \param	psInv Pointer to affected INVENTORY
 *  \param	vX x Position on screen
 *  \param	vY y Position on screen
 *  \param	vAlign Bit mask of INV_ALIGN_xx
 */
void INVENTORY_setPos(INVENTORY* psInv, var vX, var vY, var vAlign);


/*! Set max number of items and max weight per inventory and max amount of single item.
 *  \param	psInv Pointer to affected INVENTORY
 *  \param	vMaxItems  Max number of items (0 -> unlimited)
 *  \param	vMaxWeight Max weight of all items (0 -> unlimited)
 *  \param	vMaxAmount Max amount of single item (0 -> unlimited)
 */
void INVENTORY_setSize(INVENTORY* psInv, var vMaxItems, var vMaxWeight, var vMaxAmount);


/*! Recalculate inventory position. (call on resolution change) 
 *  \param	psInv Pointer to affected INVENTORY
 */
void INVENTORY_updatePos(INVENTORY* psInv);


/*! Get pointer to currently selected inventory item.
 *  Delivers the selected item as INVITEM object.
 *  \param	psInv Pointer to affected INVENTORY
 *  \return Pointer to INVITEM object structure
 */
INVITEM* INVENTORY_getItem(INVENTORY* psInv);


/*! Get pointer to first item with given item id.
 *  Delivers found item as INVITEM object.
 *  \param	psInv Pointer to affected INVENTORY
 *  \param	lItemId item id
 *  \return Pointer to INVITEM object structure
 */
INVITEM* INVENTORY_searchItem(INVENTORY* psInv, long lItemId);


/*! Add new item to inventory.
 *  Item needs to be created with INVITEM_create first.
 *  \param	psInv Pointer to affected INVENTORY
 *  \param	psItem Pointer to INVITEM to be added
 *  \return 1: item added	0: item not added
 */
var INVENTORY_add(INVENTORY* psInv, INVITEM* psItem);


/*! Use/drop currently selected inventory item.
 *  \param	psInv Pointer to affected INVENTORY
 */
void INVENTORY_sub(INVENTORY* psInv);


/*! Switch to next item in inventory.
 *  \param	psInv Pointer to affected INVENTORY
 */
void INVENTORY_next(INVENTORY* psInv);


/*! Switch to previous item in inventory.
 *  \param	psInv Pointer to affected INVENTORY
 */
void INVENTORY_prev(INVENTORY* psInv);



/*! Creates new inventory item and sets Item Bitamp, Item id and item weight.
 *  \param	strBmap File name of item bitmap
 *  \param	lItemId Item Identifier
 *  \param	vWeight Item weight
 *  \param	vAmount Item amount
 *  \return Pointer to INVITEM structure
 */
INVITEM* INVITEM_create(STRING* strBmap, long lItemId, var vWeight, var vAmount);


/*! Update amount, stackable and custom struct of given inventory item.
 *  \param	psItem Pointer to INVITEM
 *  \param	psExt Pointer to item extension (custom struct for storing additional information)
 *  \param	pfExtRemove Pointer to item extension removal function (function parameter: pointer to extension object, type void*)
 */
void INVITEM_setExtension(INVITEM* psItem, void* psExt, void* pfExtRemove);


/*! Update stackable parameter of given inventory item.
 *  \param	psItem pointer to INVITEM
 *  \param	vStackable 1: item is stackable		0: item is not stackable
 */
void INVITEM_setStackable(INVITEM* psItem, var vStackable);


/*! Get amount of given inventory item.
 *  \param	psItem Pointer to INVITEM
 *  \return Item amount
 */
var INVITEM_getAmount(INVITEM* psItem);


/*! Get weight of given INVITEM.
 *  \param	psItem Pointer to INVITEM
 *  \return Item weight
 */
var INVITEM_getWeight(INVITEM* psItem);


/* ----- INTERNAL FUNCTIONS - DO NOT USE ----- */

/*! \internal - Do not use! */
var INVENTORY__checkAmount(INVENTORY* psInv, var vAmount);
/*! \internal - Do not use! */
var INVENTORY__checkItems(INVENTORY* psInv);
/*! \internal - Do not use! */
var INVENTORY__checkWeight(INVENTORY* psInv, INVITEM* psItem);
/*! \internal - Do not use! */
void INVENTORY__update(INVENTORY* psInv);
/*! \internal - Do not use! */
void INVENTORY__updateWeight(INVENTORY* psInv);

/*! \internal - Do not use! */
void INVITEM__remove(INVITEM* psItem);

#include "invent.c"


#endif
