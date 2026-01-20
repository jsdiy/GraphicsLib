//グラフィックLCD用SPI-DMA基本操作
//『昼夜逆転』工作室	@jsdiy	https://github.com/jsdiy
//	2025/09	初版
//	2026/01	pinSS/pinDCの妥当性検証を追加, ILI9xxx系関数を作成

#pragma	once

#include <Arduino.h>
#include "SpiDma.hpp"

//SPI-DMAを利用するグラフィックLCDの基本制御
//・グラフィックLCD一つごとにインスタンス化する。
class	GLcdSpiDma
{
private:
	gpio_num_t	pinDC;	//LCDのDCピン（コマンド/データ）
	spi_device_handle_t	devHandle;
	bool	IsValidGpio(gpio_num_t pin) { return (GPIO_NUM_0 <= pin) && (pin < GPIO_NUM_MAX); }
	void	SetPinMode(gpio_num_t pinSS, gpio_num_t pinDC);
	void	SetDeviceConfig(uint8_t spiMode, int32_t busSpeedHz, gpio_num_t pinSS, spi_device_interface_config_t* devConfig);
	void	ChangeCommandMode() { gpio_set_level(pinDC, 0); }	//digitalWrite()より高速（コールバック関数からの利用を考慮して速さ重視）
	void	ChangeDataMode() { gpio_set_level(pinDC, 1); }	//digitalWrite()より高速（コールバック関数からの利用を考慮して速さ重視）
	//ILI9xxx系
	//void	SpiBasicDataTransmittion(uint8_t startByte, uint16_t data){ spiDma.Transmit(devHandle, startByte, highByte(data), lowByte(data)); }

public:
	GLcdSpiDma() { devHandle = nullptr;	pinDC = GPIO_NUM_NC; }
	void	Initialize(uint8_t spiMode, int32_t busSpeedHz, gpio_num_t pinSS, gpio_num_t pinDC);
	void	BeginTransaction() { spiDma.TakeBusControll(); }	//排他制御開始（SSのH/Lとは無関係）
	void	EndTransaction()   { spiDma.GiveBusControll(); }	//排他制御終了（SSのH/Lとは無関係）
	//ST77xx系
	void	SendCommand(uint8_t cmd) { ChangeCommandMode();	spiDma.Transmit(devHandle, cmd); }
	void	SendCommand(uint8_t cmd, uint8_t data) { SendCommand(cmd);	SendData(data); };
	void	SendData(uint8_t data) { ChangeDataMode();	spiDma.Transmit(devHandle, data); }
	void	SendData(const uint8_t* datas, size_t length);
	void	SetGRamRange(uint8_t cmd, int16_t start, int16_t end);
	//ILI9xxx系
	//void	SendIndexRegister(uint8_t startByte, uint16_t regIndex) { ChangeCommandMode(); SpiBasicDataTransmittion(startByte, regIndex); }
	//void	SendRegisterSetting(uint8_t startByte, uint16_t regValue) { ChangeDataMode(); SpiBasicDataTransmittion(startByte, regValue); }
	void	SendRegisterSetting(uint8_t regIndex, uint16_t regValue) { SendCommand(regIndex); SendData(regValue); }
	void	SendData(uint16_t data) { ChangeDataMode();	spiDma.Transmit(devHandle, highByte(data), lowByte(data)); }
};
