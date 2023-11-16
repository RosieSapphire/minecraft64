#ifndef _ENGINE_IPARMS_H_
#define _ENGINE_IPARMS_H_

#include <libdragon.h>

struct input_parms
{
	joypad_buttons_t press, held;
	joypad_inputs_t stick;
};

#endif /* _ENGINE_IPARMS_H_ */
