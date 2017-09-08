#pragma once

namespace RE {
	float FastSqrt(float x) {
		float xhalf = 0.5f*x;
		int i = *(int*)&x;
		i = 0x5f3759df - (i >> 1); // 计算第一个近似根
		x = *(float*)&i;
		x = x*(1.5f - xhalf*x*x); // 牛顿迭代法
		return x;
	}

	float InvSqrt(float number) 	{
		int i;
		float x2, y;
		const float threehalfs = 1.5F;

		x2 = number * 0.5F;
		y = number;
		i = *(int *)&y;
		i = 0x5f375a86 - (i >> 1);
		y = *(float *)&i;
		y = y * (threehalfs - (x2 * y * y));
		y = y * (threehalfs - (x2 * y * y));
		y = y * (threehalfs - (x2 * y * y));
		return number*y;
	}
}