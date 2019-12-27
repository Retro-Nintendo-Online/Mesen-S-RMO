#pragma once
#include "stdafx.h"
#include "AluMulDiv.h"
#include "Console.h"
#include "Cpu.h"
#include "Ppu.h"
#include "InternalRegisterTypes.h"
#include "../Utilities/ISerializable.h"

class MemoryManager;

class InternalRegisters final : public ISerializable
{
private:
	Console* _console;
	Cpu* _cpu;
	Ppu* _ppu;
	MemoryManager* _memoryManager;

	AluMulDiv _aluMulDiv;

	InternalRegisterState _state;
	bool _nmiFlag = false;
	bool _irqLevel = false;
	bool _needIrq = false;
	bool _irqFlag = false;
	
	void SetIrqFlag(bool irqFlag);

public:
	InternalRegisters();
	void Initialize(Console* console);

	void Reset();

	void ProcessAutoJoypadRead();

	__forceinline void ProcessIrqCounters();

	uint8_t GetIoPortOutput();
	void SetNmiFlag(bool nmiFlag);

	bool IsVerticalIrqEnabled() { return _state.EnableVerticalIrq; }
	bool IsHorizontalIrqEnabled() { return _state.EnableHorizontalIrq; }
	bool IsNmiEnabled() { return _state.EnableNmi; }
	bool IsFastRomEnabled() { return _state.EnableFastRom; }
	uint16_t GetHorizontalTimer() { return _state.HorizontalTimer; }
	uint16_t GetVerticalTimer() { return _state.VerticalTimer; }
	
	uint8_t Peek(uint16_t addr);
	uint8_t Read(uint16_t addr);
	void Write(uint16_t addr, uint8_t value);

	InternalRegisterState GetState();
	AluState GetAluState();

	void Serialize(Serializer &s) override;
};

void InternalRegisters::ProcessIrqCounters()
{
	if(_needIrq) {
		_needIrq = false;
		SetIrqFlag(true);
	}

	bool irqLevel = (
		(_state.EnableHorizontalIrq || _state.EnableVerticalIrq) &&
		(!_state.EnableHorizontalIrq || (_state.HorizontalTimer <= 339 && (_ppu->GetCycle() == _state.HorizontalTimer) && (_ppu->GetLastScanline() != _ppu->GetRealScanline() || _state.HorizontalTimer < 339))) &&
		(!_state.EnableVerticalIrq || _ppu->GetRealScanline() == _state.VerticalTimer)
	);

	if(!_irqLevel && irqLevel) {
		_needIrq = true;
	}
	_irqLevel = irqLevel;
}