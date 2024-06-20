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

float RandRange(int maxRange) 
{
	maxRange *= 100;
	float tempFloat = Rand() % maxRange;
	tempFloat /= 100;
	
	return tempFloat;
}

float Lerp(float a, float b, float t) 
{
	return a * (1.0 - t) + (b * t);
}

float DistanceBetweenTwoPoints(DirectX::XMVECTORF32 positionA, DirectX::XMVECTORF32 positionB)
{
	//return sqrt(((positionB[0] - positionA[0]) * (positionB[0] - positionA[0]) + 
		//(positionB[1] - positionA[1]) * (positionB[1] - positionA[1]) +
		//(positionB[2] - positionA[2]) * (positionB[2] - positionA[2])));
	return (10.0f);
}

} // namespace utils
