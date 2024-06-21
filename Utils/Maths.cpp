#include "GameProgExercise01.h"
#include "Utils\Maths.h"

namespace utils
{

static unsigned int randSeed = 0;
unsigned int Rand()
{
	SYSTEMTIME time{};
	GetSystemTime( &time );
	return( ( randSeed++ * 1103515245u ) + 12345 + time.wMinute * 60 + time.wSecond );
}

float RandRange( int maxRange ) 
{
	maxRange *= 100;
	float tempFloat = Rand() % maxRange;
	tempFloat /= 100;
	
	return tempFloat;
}

float Lerp( float a, float b, float t ) 
{
	return a * ( 1.0f - t ) + ( b * t );
}

} // namespace utils
