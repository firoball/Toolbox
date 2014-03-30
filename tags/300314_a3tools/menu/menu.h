#ifndef MENU_H
#define MENU_H

#ifndef MENU_ID_LIMIT
#define MENU_ID_LIMIT 20
#endif 
 
typedef struct menuopt
{
	STRING* pCaption;
	var value;
}MENUOPT;

typedef struct menu
{
	STRING* pCaption;
	struct menu* pParent;
	var id;

	/* menuitem stuff */
	LIST* pOptions;

	/* menu stuff */
	LIST* pItems;
	void* fpExecute;
	void* fpShow;
	void* fpHide;
	void* fpChange;
}MENU;


MENU* MENU_create(char* file);
void MENU_remove(MENU* host);

void MENU_select(MENU* host);
void MENU_cancel(MENU* host);
void MENU_up(MENU* host);
void MENU_down(MENU* host);
void MENU_left(MENU* host);
void MENU_right(MENU* host);

/* function pointers */
void fpMENU_execute(var id, STRING* caption, var value, STRING* option);
void fpMENU_show(var id, STRING* caption, var value, STRING* option);
void fpMENU_hide(var id, STRING* caption, var value, STRING* option);
void fpMENU_change(var id, STRING* caption, var value, STRING* option);


#endif