#ifndef CHRONO_H
# define CHRONO_H

# include <iostream>
# include <sstream>
# include <iomanip>
# include <ctime>
# include <time.h>
# include <sys/time.h>

typedef struct s_time
{
	std::time_t now;
	char		hours[2];
	char		minutes[2];
	char		seconds[2];
}				T_time;

void		timerDisplay(struct timeval *start, struct timeval *end);
long double	time_diff(struct timeval *start, struct timeval *end);
long double	get_the_time(void);

#endif
