#include <acknex.h>

#define PRAGMA_PATH ".."
#include "sysmsg\sysmsg.h"

var sample_cnt = 0;

void sample_msg()
{
	STRING* msg = "#30";
	STRING* s = "#9";

	str_cpy(msg, "New message arrived! Number ");
	str_for_num(s,sample_cnt); 
	str_cat(msg, s);

	SYSMSG_print(msg->chars);
	sample_cnt++;
}

void main()
{
	video_mode = 7; 
	screen_color.blue = 150;
	wait (3);
	on_a = sample_msg;
	SYSMSG_create();
	SYSMSG_logToFile(ON);
	while (key_esc == 0)
		wait (1);
		
	SYSMSG_remove();
	sys_exit(NULL);
}