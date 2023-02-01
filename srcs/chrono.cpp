#include "chrono.hpp"

void	timerDisplay(struct timeval *start, struct timeval *end)
{
	std::cout << "The operation took " << time_diff(start, end) << " nanoseconds to complete." << std::endl;
}

long double	time_diff(struct timeval *start, struct timeval *end)
{
	long double	sec;
	long double	usec;

	sec = end->tv_sec - start->tv_sec;
	usec = end->tv_usec - start->tv_usec;
	return (sec * 1000 + usec / 1000);
}

long double	get_the_time()
{
	struct timeval	now;
	long double		sec;
	long double		usec;

	gettimeofday(&now, NULL);
	sec = now.tv_sec;
	usec = now.tv_usec;
	return (sec * 1000 + usec / 1000);
}

