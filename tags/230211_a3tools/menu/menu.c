#include <acknex.h>

/* include from LIST project */
#define PRAGMA_PATH "..\\list"
#include "list.h"
#include "list.c"

/* include from XMLREADER project */
#define PRAGMA_PATH "..\\xmlreader"
#include "xmlreader.c"
#include "xmlreader.h"

#include "menu.h"
/*
MENU* MENU_create(char* file)
{
	XMLFILE* host;
	host = (XMLFILE*)malloc(sizeof(XMLFILE));
	host->fName = str_create(file);
	host->fHndl = NULL;
	host->anchor = NULL;
		
	return (host);
}

*/

void* fpMenuFunc_exec[MENU_ID_LIMIT];
void* fpMenuFunc_show[MENU_ID_LIMIT];
void* fpMenuFunc_hide[MENU_ID_LIMIT];
void* fpMenuFunc_change[MENU_ID_LIMIT];

void MENU_setExecute(var id, void* func)
{
	id = clamp(id, 0, MENU_ID_LIMIT - 1);
	fpMenuFunc_exec[id] = func;
}

void MENU_setShow(var id, void* func)
{
	id = clamp(id, 0, MENU_ID_LIMIT - 1);
	fpMenuFunc_show[id] = func;
}

void MENU_setHide(var id, void* func)
{
	id = clamp(id, 0, MENU_ID_LIMIT - 1);
	fpMenuFunc_hide[id] = func;
}

void MENU_setChange(var id, void* func)
{
	id = clamp(id, 0, MENU_ID_LIMIT - 1);
	fpMenuFunc_changey[id] = func;
}


/* testkram */
void* func_array[3];
void testprint(int val)
{
	printf("structpointer test: %d", val);
}

void MENU_assignFunc(var id, void* func)
{
	func_array[id] = func;		
}

void func(int val);
void fpExecute(var id)
{
	if (func_array[id] != NULL)
	{
		func = func_array[id];
		func(3);
	}
}

void main()
{
	MENU_assignFunc(0, testprint);
	fpExecute(0);	
	while(1) wait(1);	
}