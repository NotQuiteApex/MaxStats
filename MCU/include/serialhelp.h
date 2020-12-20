#ifndef MAX_STATS_SERIAL_HELP_H
#define MAX_STATS_SERIAL_HELP_H

#include "screen.h"

void serial_begin();

void serial_loop();

void serial_receive();

bool serial_matches(char *);

bool receive_once_data();

bool receive_cont_data();

#endif // MAX_STATS_SERIAL_HELP_H
