//グラフィックLCD用SPI-DMA基本操作
//『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2025/09	初版
//	2026/01	pinSS/pinDCの妥当性検証を追加, SendData()のオーバーロードを追加, SetGRamRange()廃止

#pragma	once

#include <Arduino.h>
#include "SpiDma.hpp"

//SPI-DMAを利用するグラフィックLCDの基本制御
//・グラフィックLCD一つごとにインスタンス化する。
class	GLcdSpiDma
{
private:
	gpio_num_t	pinDC;	//LCDのDCピン（データ/コマンド）
	spi_device_handle_t	devHandle;
	bool	IsValidGpio(gpio_num_t pin) { return (GPIO_NUM_0 <= pin) && (pin < GPIO_NUM_MAX); }
	void	SetPinMode(gpio_num_t pinSS, gpio_num_t pinDC);
	void	SetDeviceConfig(uint8_t spiMode, int32_t busSpeedHz, gpio_num_t pinSS, spi_device_interface_config_t* devConfig);
	void	ChangeCommandMode() { gpio_set_level(pinDC, 0); }
	void	ChangeDataMode() { gpio_set_level(pinDC, 1); }

public:
	GLcdSpiDma() { devHandle = nullptr;	pinDC = GPIO_NUM_NC; }
	void	Initialize(uint8_t spiMode, int32_t busSpeedHz, gpio_num_t pinSS, gpio_num_t pinDC);
	void	BeginTransaction() { spiDma.TakeBusControll(); }	//SPIバス排他利用開始（SSのH/Lとは無関係）
	void	EndTransaction()   { spiDma.GiveBusControll(); }	//SPIバス排他利用終了（SSのH/Lとは無関係）
	void	SendCommand(uint8_t cmd) { ChangeCommandMode();	spiDma.Transmit(devHandle, cmd); }
	void	SendCommand(uint8_t cmd, uint8_t data) { SendCommand(cmd);	SendData(data); };
	void	SendData(uint8_t data) { ChangeDataMode();	spiDma.Transmit(devHandle, data); }
	void	SendData(uint8_t data1, uint8_t data2) { ChangeDataMode();	spiDma.Transmit(devHandle, data1, data2); }
	void	SendData(uint16_t data) { ChangeDataMode();	spiDma.Transmit(devHandle, highByte(data), lowByte(data)); }
	void	SendData(uint16_t data1, uint16_t data2) { ChangeDataMode();	spiDma.Transmit(devHandle, highByte(data1), lowByte(data1), highByte(data2), lowByte(data2)); }
	void	SendData(const uint8_t* datas, size_t length);
};
