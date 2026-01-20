//グラフィック／テキスト描画ライブラリ整備
//『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2025/10

/*	ピンアサイン
			ESP32-C3
SPI-SS		GPIO_NUM_10
SPI-MOSI	GPIO_NUM_7
SPI-MISO	GPIO_NUM_2
SPI-SCK		GPIO_NUM_6
LCD-DC		GPIO_NUM_4
LCD-RESET	GPIO_NUM_NC
*/

#include <Arduino.h>
#include "LcdST77xx.hpp"

//オブジェクト
LcdST77xx	lcd;

//色
static	Color	ColorWhite = Color(0xFF, 0xFF, 0xFF);
static	Color	ColorBlack = Color(0x00, 0x00, 0x00);
static	Color	ColorYellow = Color(0xFF, 0xFF, 0x00);
static	Color&	foreColor = ColorWhite;
static	Color&	bgColor = ColorBlack;

void	setup(void)
{
	Serial.begin(115200);
	delay(1000);

	lcd.Initialize();
	lcd.RotateFlip(ERotFlip::Rot90 | ERotFlip::XFlip);
	lcd.ClearScreen(ColorYellow);

	lcd.SetScale(3, 4);
	lcd.DrawInt(0, 0, INT32_MIN);
}

void	loop(void)
{
	//	
}
