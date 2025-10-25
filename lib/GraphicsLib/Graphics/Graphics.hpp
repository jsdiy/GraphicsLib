//図形描画
//『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2024/4 - 2025/10

#pragma	once

#include <Arduino.h>
#include "Color.hpp"

//図形描画
class	Graphics
{
private:
	void	SwapPoint(int16_t* x1, int16_t* y1, int16_t* x2, int16_t* y2);	//点P(x1,y1)と点Q(x2,y2)の座標を交換する; (P←→Q)
	void	DrawLineH(int16_t x1, int16_t y1, int16_t x2, const Color& color);
	void	DrawLineV(int16_t x1, int16_t y1, int16_t y2, const Color& color);
	void	FillBetweenLine(int16_t x1s, int16_t y1s, int16_t x1e, int16_t y1e,
				int16_t x2s, int16_t y2s, int16_t x2e, int16_t y2e, const Color& color);
	void	DrawCircleWithFillFlag(int16_t cx, int16_t cy, int16_t r, uint8_t isFill, const Color& color);

public:
	virtual	~Graphics(void) {}
	virtual	void	FillRect(int16_t x, int16_t y, int16_t w, int16_t h, const Color& color) = 0;
	void	DrawPixel(int16_t x, int16_t y, const Color& color);
	void	DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, const Color& color);
	void	DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, const Color& color);
	void	DrawTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, const Color& color);
	void	FillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, const Color& color);
	void	DrawCircle(int16_t cx, int16_t cy, int16_t r, const Color& color);
	void	FillCircle(int16_t cx, int16_t cy, int16_t r, const Color& color);
};
