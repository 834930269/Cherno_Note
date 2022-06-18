#pragma once

static class Helper {
public:
	static float lerp(float x,float y,float weight) {
		return x + (y - x) * weight;
	}
};