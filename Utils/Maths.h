#pragma once

namespace utils
{

unsigned int Rand();
float RandRange(int maxRange);
float Lerp(float a, float b, float t);
float DistanceBetweenTwoPoints(DirectX::XMVECTORF32 positionA, DirectX::XMVECTORF32 positionB);
} // namespace utils
