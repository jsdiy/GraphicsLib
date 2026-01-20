//	ILI9225 グラフィックLCDドライバー
//『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2026/1	初版

/*	ピンアサイン
			ESP32-C3	ILI9225
SPI-SS		GPIO_NUM_10	CS
SPI-MOSI	GPIO_NUM_7	SDI
SPI-MISO	GPIO_NUM_2	なし（不要）
SPI-SCK		GPIO_NUM_6	CLK
LCD-DC		GPIO_NUM_4	RS（0:インデックスレジスタを設定する 1:レジスタ値/GRAMデータを書き込む）
LCD-RESET	GPIO_NUM_NC	RST
*/

#include <Arduino.h>
#include "LcdILI9225.hpp"
#include "Img20240210V.hpp"	//縦_551HORAI
#include "Img20250330H.hpp"	//横_池のある公園
#include "Img20251124V.hpp"	//縦_紅葉した樹木
#include "ImgFBlue.hpp"

//オブジェクト
LcdILI9225	lcd;

//色
static	Color	ColorWhite	= Color(0xFF, 0xFF, 0xFF);
static	Color	ColorBlack	= Color(0x00, 0x00, 0x00);
static	Color	ColorRed	= Color(0xFF, 0x00, 0x00);
static	Color	ColorGreen	= Color(0x00, 0xFF, 0x00);
static	Color	ColorBlue	= Color(0x00, 0x00, 0xFF);
static	Color	ColorYellow	= Color(0xFF, 0xFF, 0x00);

void	setup()
{
	Serial.begin(115200);
	delay(1000);

	//初期化
	lcd.Initialize();
	lcd.ClearScreen(ColorBlue);	//表示テストを兼ねて白・黒以外で塗る
}

void	setup2()
{
	Serial.begin(115200);
	delay(1000);

	//初期化
	lcd.Initialize();
	lcd.ClearScreen(ColorYellow);	//表示テストを兼ねて白・黒以外で塗る

	//LCD設定
	//lcd.RotateFlip(ERotFlip::Normal | ERotFlip::FlipVertical);
	//lcd.DrawImage(10, 10, ImgFBlue::Width, ImgFBlue::Height, ImgFBlue::Datas, ImgFBlue::Length);
	//lcd.ClearScreen(ColorYellow);
	lcd.RotateFlip(ERotFlip::Rot180 | ERotFlip::FlipHorizontal);
	lcd.DrawImage(10, 10, ImgFBlue::Width, ImgFBlue::Height, ImgFBlue::Datas, ImgFBlue::Length);
	//lcd.ClearScreen(ColorYellow);
	//lcd.RotateFlip(ERotFlip::Normal | ERotFlip::FlipHorizontal);
	//lcd.DrawImage(10, 10, ImgFBlue::Width, ImgFBlue::Height, ImgFBlue::Datas, ImgFBlue::Length);
	
	//lcd.RotateFlip(ERotFlip::Rot90);
	//lcd.DrawImage(0, 0, ImgFBlue::Width, ImgFBlue::Height, ImgFBlue::Datas, ImgFBlue::Length);
	//lcd.RotateFlip(ERotFlip::Rot90 | ERotFlip::FlipHorizontal);
	//lcd.DrawImage(lcd.Width() - ImgFBlue::Width, 0, ImgFBlue::Width, ImgFBlue::Height, ImgFBlue::Datas, ImgFBlue::Length);

}

void	setup1()
{
	Serial.begin(115200);
	delay(2000);

	//初期化
	lcd.Initialize();
Serial.println("^--- ERotFlip::Normal ---v");

	//LCD設定
Serial.println("ClearScreen");
	lcd.ClearScreen(ColorBlue);	//表示テストを兼ねて白・黒以外で塗る
	delay(500);

Serial.println("DrawImage-V");
	lcd.RotateFlip(ERotFlip::Normal);
	lcd.DrawImage(0, 0, Img20240210V::Width, Img20240210V::Height, Img20240210V::Datas, Img20240210V::Length);
	delay(500);
	lcd.RotateFlip(ERotFlip::FlipHorizontal);
	lcd.DrawImage(0, 0, Img20240210V::Width, Img20240210V::Height, Img20240210V::Datas, Img20240210V::Length);
	delay(500);
	lcd.RotateFlip(ERotFlip::FlipVertical);
	lcd.DrawImage(0, 0, Img20240210V::Width, Img20240210V::Height, Img20240210V::Datas, Img20240210V::Length);
	delay(500);

	//0度：標準-ok, H-ok, V-ok
	//180度：標準-ok, 180|H-ok, 180|V-ok
	//90度：標準-ok, 90|H-ok, 90|V-ok
	//270度：標準-ok, 270|H-ok, 270|V-ok
}

void	loop1()
{
	delay(1000);
}

void	loop()
{
	lcd.RotateFlip(ERotFlip::Normal);
	lcd.DrawImage(0, 0, Img20251124V::Width, Img20251124V::Height, Img20251124V::Datas, Img20251124V::Length);
	delay(1000);
	lcd.RotateFlip(ERotFlip::FlipHorizontal);
	lcd.DrawImage(0, 0, Img20251124V::Width, Img20251124V::Height, Img20251124V::Datas, Img20251124V::Length);
	delay(1000);
	lcd.RotateFlip(ERotFlip::FlipVertical);
	lcd.DrawImage(0, 0, Img20251124V::Width, Img20251124V::Height, Img20251124V::Datas, Img20251124V::Length);
	delay(1000);
	lcd.ClearScreen(ColorGreen);
	delay(1000);

	lcd.RotateFlip(ERotFlip::Rot90);
	lcd.DrawImage(0, 0, Img20250330H::Width, Img20250330H::Height, Img20250330H::Datas, Img20250330H::Length);
	delay(1000);
	lcd.RotateFlip(ERotFlip::Rot90 | ERotFlip::FlipHorizontal);
	lcd.DrawImage(0, 0, Img20250330H::Width, Img20250330H::Height, Img20250330H::Datas, Img20250330H::Length);
	delay(1000);
	lcd.RotateFlip(ERotFlip::Rot90 | ERotFlip::FlipVertical);
	lcd.DrawImage(0, 0, Img20250330H::Width, Img20250330H::Height, Img20250330H::Datas, Img20250330H::Length);
	delay(1000);
	lcd.ClearScreen(ColorRed);
	delay(1000);

	lcd.RotateFlip(ERotFlip::Normal);
	lcd.DrawImage(0, 0, Img20251124V::Width, Img20251124V::Height, Img20251124V::Datas, Img20251124V::Length);
	delay(1000);
	lcd.RotateFlip(ERotFlip::Rot90);
	lcd.DrawImage(0, 0, Img20250330H::Width, Img20250330H::Height, Img20250330H::Datas, Img20250330H::Length);
	delay(1000);
	lcd.RotateFlip(ERotFlip::Rot180);
	lcd.DrawImage(0, 0, Img20251124V::Width, Img20251124V::Height, Img20251124V::Datas, Img20251124V::Length);
	delay(1000);
	lcd.RotateFlip(ERotFlip::Rot270);
	lcd.DrawImage(0, 0, Img20250330H::Width, Img20250330H::Height, Img20250330H::Datas, Img20250330H::Length);
	delay(1000);
	lcd.RotateFlip(ERotFlip::Normal);
	lcd.DrawImage(0, 0, Img20251124V::Width, Img20251124V::Height, Img20251124V::Datas, Img20251124V::Length);
	delay(1000);

	lcd.ClearScreen(ColorYellow);
	lcd.DrawImage((lcd.Width() - ImgFBlue::Width) / 2, (lcd.Height() - ImgFBlue::Height) / 2, ImgFBlue::Width, ImgFBlue::Height, ImgFBlue::Datas, ImgFBlue::Length);
	delay(1000);
}
