#ifndef COMMON_ACTORS_H
#define COMMON_ACTORS_H

#include "Console_actor.hh"

Console_actor process_prompt("Process", c_green);
Console_actor timer_prompt(  "Timer",   c_blue );
Console_actor debug_prompt(  "Debug",   c_lgray);
Console_actor error_prompt(  "Error",   c_red);

#endif