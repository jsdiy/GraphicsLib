//	LCDコントローラ ILI9225	SPI-DMA基本操作
//	『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	前提：アプリで使用するグラフィックLCDは1つ
//	2026/01	初版

#pragma	once

#include <Arduino.h>
#include "Color.hpp"
#include "GLcdSpiDma.hpp"
#include "GLcdText.hpp"

//ユーザー設定
namespace	ILI9225Config
{
	//ピンアサイン
	/*
	参考	ESP32のVSPI:	SS = GPIO_NUM_5, MOSI = GPIO_NUM_23, (MISO = GPIO_NUM_19), SCK = GPIO_NUM_18
	参考	ESP32-C3のSPI2:	SS = GPIO_NUM_10, MOSI = GPIO_NUM_7, (MISO = GPIO_NUM_2), SCK = GPIO_NUM_6
	ILI9225-LCDモジュールのピン並び（基板に番号1,11が振られている）
	1:VCC, 2:GND, 3:GND, 4:NC, 5:NC, 6:LED, 7:CLK, 8:SDI, 9:RS, 10:RST, 11:CS
	*/
	constexpr	gpio_num_t
		PinSS		= GPIO_NUM_10,	//ILI9225:CS
		PinDC		= GPIO_NUM_4,	//ILI9225:RS
		PinRESET	= GPIO_NUM_NC;	//ILI9225:RST

	//コントローラー
	enum	class	EController	{ ILI9225 /*, ILI9341, ...*/ };	//どれかを↓へ書く
	constexpr	EController	Controller = EController::ILI9225;

	//表示パネル
	constexpr	uint16_t
		BitSS	= 1,	//0:水平方向において発光素子がコントローラーの端子と同順で結線されているパネル, 1:逆順のパネル
		BitBGR	= 1,	//0:発光素子の並びがR-G-Bのパネル, 1:B-G-Rのパネル
		BitREV	= 1;	//0:色データのとおりに発色するパネル, 1:白の補色で発色するパネル（白データが黒色、青が黄色、赤が水色など）
	/*	SSビット、BGRビット、REVビットの説明
	■SSビット
	SS=0/1は、パネルのカラム方向の端子順に合うよう、コントローラーのソース端子のドライブ順を設定する。
		SS=0: S1 → S528		パネルの端子順がコントローラーのソース端子順と同じ場合は0
		SS=1: S528 → S1		パネルの端子順がコントローラーのソース端子順と逆の場合は1
	・SS=0で表示される画像に対して、SS=1はRGBが逆順(BGR)の発色、かつ、左右反転で表示される。
	・SSビットでは、色の正しさは設定できない。
	
	■BGRビット
	BGR=0/1は、パネルの発光素子の並び順(RGB/BGR)に合うよう、コントローラーのソース端子のドライブ順を設定する。
	イメージとしては次のようになる。　※数字はソース端子を表し、[]はパネル上の1画素を表す。
		BGR=0: [1,2,3][4,5,6]...[523,524,525][526,527,528]	RGBパネルの場合は0
		BGR=1: [3,2,1][6,5,4]...[525,524,523][528,527,526]	BGRパネルの場合は1
	・BGR=0に対してBGR=1はR/B成分が入れ替わった色で表示される。例：赤(#F00)←→青(#00F)
	・BGRビットでは画像の左右は反転しない。また、白黒が反対になるわけではない。

	■REVビット
	REV=0/1は、パネルの色濃度の解釈に合うよう、色データのグレースケールを反対にする。
		REV=0: グレースケールはそのまま		色データをそのまま出力する場合は0
		REV=1: グレースケールを反対にする	色データを0xFFFFの補色で出力する場合は1
	・画像を表示したときに、白(#FFF)←→黒(#000)、青(#00F)←→黄(#FF0)、緑(#0F0)←→紫(#F0F)、赤(#F00)←→水色(#0FF)
	　のように色化けしている場合はREV=1を設定する。

	■SSビット、BGRビット、REVビットの設定手順
	準備
	- R01_DriverOutputControlレジスタに、SS=0 を設定する。
	- R03_EntryModeレジスタに、BGR=0, {I/D[1:0]=11, AM=0}:標準の向き を設定する。
	- R07_DisplayControl1レジスタに、REV=0 を設定する。
	- LCDモジュールを正位置に置く。多くの場合、LCDパネルのフレキシブルケーブルを下側とする向き。
	手順
	(1) (0,0)-(100,100)の領域を青(0x001F)で塗る。(2)へ。
	(2) 色に関係なく、四角形が左上に表示されていたら(3)へ。
		右上に表示されていたらSS=1とし、改めて(1)を実行。
	(3) 四角形が青だったら設定完了。
		赤だったらBGR=1とし、改めて(1)を実行。
		黄だったらREV=1とし、改めて(1)を実行。
		水色だったらBGR=1,REV=1とし、改めて(1)を実行。
	*/
}

//反転/回転表示のパラメータ
//指定パターン:	Normal, RotXXX, FlipXXX, RotXXX|FlipXXX
enum	class	ERotFlip	: uint8_t
{
	Normal	= 0x00,	//LCDの正位置（多くの場合、LCDパネルのフレキシブルケーブルを下側とする向き）
	Rot90	= 0x01,	//LCDの正位置に対して画像を90度回転させる（画面座標系が回転する）
	Rot180	= 0x02,	//LCDの正位置に対して画像を180度回転させる（画面座標系が回転する）
	Rot270	= 0x04,	//LCDの正位置に対して画像を270度(-90度)回転させる（画面座標系が回転する）
	FlipHorizontal	= 0x08,	//画像の正位置に対して水平反転（画面座標系は反転しない）
	FlipVertical	= 0x10,	//画像の正位置に対して垂直反転（画面座標系は反転しない）
};

//グラフィックLCDクラス
class	LcdILI9225	: public GLcdText
{
private:
	//レジスタ番号	※利用頻度が高そうなもの
	//データシート	8.2. Instruction Descriptions
	enum	class	ERegIdx	: uint8_t
	{
		IdxDRIVER	= 0x01,	//Driver Output Control
		IdxENTRY	= 0x03,	//Entry Mode
		IdxDISPLAY	= 0x07,	//Display Control 1
		IdxPOWER1	= 0x10,	//Power Control 1
		IdxPOWER2	= 0x11,	//Power Control 2
		IdxPOWER3	= 0x12,	//Power Control 3
		IdxPOWER4	= 0x13,	//Power Control 4
		IdxPOWER5	= 0x14,	//Power Control 5
		IdxRAMADL	= 0x20,	//RAM Address Set 1 (Lo-byte)	※下位バイトは短辺方向（ソース）の描画開始位置
		IdxRAMADH	= 0x21,	//RAM Address Set 2	(Hi-byte)	※上位バイトは長辺方向（ゲート）の描画開始位置
		IdxWRGRAM	= 0x22,	//Write Data to GRAM
		IdxSWRESET	= 0x28,	//Software Reset
		IdxWNDHED	= 0x36,	//Horizontal Window Address -1（終了位置）	※水平方向＝短辺方向（ソース）
		IdxWNDHST	= 0x37,	//Horizontal Window Address -2（開始位置）
		IdxWNDVED	= 0x38,	//Vertical Window Address -1（終了位置）	※垂直方向＝長辺方向（ゲート）
		IdxWNDVST	= 0x39,	//Vertical Window Address -2（開始位置）
	};

	//レジスタのビット値
	/*	struct{enum{}}とした理由
	「enum class BitVal : uint16_t」だとOR演算時にuint16_tへの明示的なキャストが必要。
	「struct BitVal { enum : uint16_t 変数名}」とすれば「BitVal::変数名」と書け、その上でキャストも不要。
	なお、「enum BitVal : uint16_t」だとキャストは不要だが「BitVal::」なしで変数名が書けてしまう。
	*/
	struct	BitVal
	{
		enum : uint16_t
		{
			//回転・反転
			EntryAM_dirH	= (0 << 3),	//0:address update horizontal writing direction, 1:vertical
			EntryAM_dirV	= (1 << 3),
			EntryID_HdecVdec	= (0b00 << 4),	//00,01,10,11	※I/D: Increment/Decrement
			EntryID_HincVdec	= (0b01 << 4),
			EntryID_HdecVinc	= (0b10 << 4),
			EntryID_HincVinc	= (0b11 << 4),
			EntryAMID_BitMask	= EntryAM_dirV | EntryID_HincVinc,	//AMとI/Dのビットが全て'1'

			//表示パネル特性
			DriverSS	= (ILI9225Config::BitSS << 8),	//0:ソースドライブ正順(S1→S528), 1:ソースドライブ逆順(S528→S1)
			EntryBGR	= (ILI9225Config::BitBGR << 12),	//0:RGBパネル, 1:BGRパネル
			DisplayREV	= (ILI9225Config::BitREV << 2),		//0:色反転なし 1:色反転あり（白の補色）
		};
	};

	//レジスタ値	※初期化以降で書き換えが発生するレジスタの値を保持しておく目的
	struct RegisterValue
	{
		uint16_t
			R01DriverOutputControl,
			R03EntryMode,
			R07DisplayControl1;
	};

private:
	static	constexpr	uint8_t		SpiMode = 0;	//SPIモード(0-3)
	static	constexpr	uint32_t	SpiClock = 40UL * 1000000;	//SPIクロック(Hz)	※80MHzの分周値が望ましい
	/*	データシート	14.6.3. Serial Data Transfer Interface Timing Characteristics
	Serial clock cycle time
	Write ( received )  : tSCYC Min. = 20ns	-> SPI_busclock Max. = 50MHz
	Read ( transmitted ): tSCYC Min. = 40ns	-> SPI_busclock Max. = 25MHz
	*/

private:
	GLcdSpiDma	glcd;
	uint8_t*	grBuffer;
	size_t	grBufferLength;
	int16_t	LcdWidth, LcdHeight;	//LCDモジュールの正位置での縦横サイズ
	int16_t	screenWidth, screenHeight;	//90度回転を考慮した論理的な縦横サイズ
	RegisterValue lcdReg;
	void	SendRegVal(ERegIdx regIdx, uint16_t regVal);
	void	SendGRamData(uint8_t* datas, size_t length);
	void	SetGRamArea(int16_t x, int16_t y, int16_t w, int16_t h);
	void	SpiTxStart() { digitalWrite(ILI9225Config::PinSS, LOW); }
	void	SpiTxEnd() { digitalWrite(ILI9225Config::PinSS, HIGH); }

public:
	LcdILI9225();
	int16_t	Width() { return screenWidth; }
	int16_t	Height() { return screenHeight; }
	void	Initialize();
	void	HwReset();
	void	RotateFlip(ERotFlip param);
	void	FillRect(int16_t x, int16_t y, int16_t w, int16_t h, const Color& color);
	void	DrawImage(int16_t x, int16_t y, int16_t w, int16_t h, const uint8_t* imgData, size_t dataLength);
	void	ClearScreen(const Color& color);
	uint8_t*	GetLineBuffer(size_t* bufLength = nullptr);
};

//ビットOR演算子'|'のオーバーロード	※実装はヘッダーに記述する
//・func(EnumType n)において、func((EnumType)(A|B)) を func(A|B) と書ける。
constexpr	ERotFlip	operator | (ERotFlip lhs, ERotFlip rhs)
{
	return static_cast<ERotFlip>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

/*	8.2.18. RAM Address Set (R20h, R21h)
GRAM Address Range
ADDR[15:0]		Gram setting
0000h to 00AFh	Bitmap data for G1
0100h to 01AFh	Bitmap data for G2
0200h to 02AFh	Bitmap data for G3
0300h to 03AFh	Bitmap data for G4
:				:
D800h to D8AFh	Bitmap data for G217
D900h to D9AFh	Bitmap data for G218
DA00h to DAAFh	Bitmap data for G219
DB00h to DBAFh	Bitmap data for G220

※Hi: 00xxh～DBxxh --> 0～219 (220line)
※Lo: xx00h～xxAFh --> 0～175 (176px)
*/
/*	ILI9225の命令一覧
No.	Registers Name
00h	Driver Code Read	※読み込み命令
01h	Driver Output Control
02h	LCD AC Driving Control
03h	Entry Mode
07h	Display Control 1
08h	Blank Period Control 1	(Display Control 2)
0Bh	Frame Cycle Control
0Ch	Interface Control
0Fh	Oscillation Control
10h	Power Control 1
11h	Power Control 2
12h	Power Control 3
13h	Power Control 4
14h	Power Control 5
15h	VCI Recycling
20h	RAM Address Set 1
21h	RAM Address Set 2
22h	Write Data to GRAM
22h	Read Data from GRAM	※読み込み命令（Writeと同じNo.だが、StartByteのR/Wビットで区別する）
28h	Software Reset
30h	Gate Scan Control
31h	Vertical Scroll Control 1
32h	Vertical Scroll Control 2
33h	Vertical Scroll Control 3
34h	Partial Driving Position -1
35h	Partial Driving Position -2
36h	Horizontal Window Address -1
37h	Horizontal Window Address -2
38h	Vertical Window Address -1
39h	Vertical Window Address -2
50h	Gamma Control 1
51h	Gamma Control 2
52h	Gamma Control 3
53h	Gamma Control 4
54h	Gamma Control 5
55h	Gamma Control 6
56h	Gamma Control 7
57h	Gamma Control 8
58h	Gamma Control 9
59h	Gamma Control 10
*/
