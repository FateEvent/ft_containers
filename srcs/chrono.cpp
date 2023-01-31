#include "chrono.hpp"

void	displayClock(T_time onceUponATime, std::string header) {
	(void)header;
	std::cout << std::setfill(' ') << std::setw(55) <<"         TIMER         \n";
	std::cout << std::setfill(' ') << std::setw(55) <<" --------------------------\n";
	std::cout << std::setfill(' ') << std::setw(29);

	std::cout << "| " << std::setfill('0') << std::setw(2) << onceUponATime.hours << " hrs | ";
	std::cout << std::setfill('0') << std::setw(2) << onceUponATime.minutes << " min | ";
	std::cout << std::setfill('0') << std::setw(2) << onceUponATime.seconds << " sec |" << std::endl;
	std::cout << std::setfill(' ') << std::setw(55) <<" --------------------------\n";
}

T_time	getTimeStamp()
{
	T_time	onceUponATime;

	onceUponATime.now = time(nullptr);
	if (std::strftime(onceUponATime.hours, sizeof(onceUponATime.hours), "%H", std::localtime(&onceUponATime.now))) {
		std::cout << onceUponATime.hours;
	}
	if (std::strftime(onceUponATime.minutes, sizeof(onceUponATime.minutes), "%M", std::localtime(&onceUponATime.now))) {
		std::cout << onceUponATime.minutes;
	}
	if (std::strftime(onceUponATime.seconds, sizeof(onceUponATime.seconds), "%S", std::localtime(&onceUponATime.now))) {
		std::cout << onceUponATime.seconds;
	}
	return (onceUponATime);
}

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

long	length_diff(long present, long past)
{
	return (present - past);
}
