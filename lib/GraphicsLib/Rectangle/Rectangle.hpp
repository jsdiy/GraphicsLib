//	矩形領域を操作するクラス
//『昼夜逆転』工作室	http://jsdiy.starfree.jp
//	2024/04	@jsdiy

#pragma	once

#include <Arduino.h>

//単純な矩形領域型
class	SimpleRectangle
{
protected:
	int16_t	left = 0, top = 0, right = 0, bottom = 0;
	bool	isAvailable = false;	//矩形領域として成立しているか

public:
	SimpleRectangle(void) {}
	SimpleRectangle(int16_t x, int16_t y, int16_t w, int16_t h) { SetRect(x, y, w, h); }

	void	SetRect(int16_t x, int16_t y, int16_t w, int16_t h)
	{
		isAvailable = false;
		if (w <= 0 || h <= 0) { return; }
		left = x;	top = y;	right = x + w - 1;	bottom = y + h - 1;
		isAvailable = true;
	}

	int16_t	Left(void) { return left; }
	int16_t	Top(void) { return top; }
	int16_t	Right(void) { return right; }
	int16_t	Bottom(void) { return bottom; }
	int16_t	Width(void) { return (right - left + 1); }
	int16_t	Height(void) { return (bottom - top + 1); }
	bool	IsAvailable(void) { return isAvailable; }
};

//変形処理ありの矩形領域型
class	Rectangle	: public SimpleRectangle
{
private:
	//Rectangle	Copy(void);

public:
	using	SimpleRectangle::SimpleRectangle;	//親クラスの全てのコンストラクタが継承されて利用できる

	void	Move(int16_t x, int16_t y);
	Rectangle	Inner(int16_t x, int16_t y, Rectangle& child);
};
