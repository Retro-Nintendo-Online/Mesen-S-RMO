#pragma once
#include "stdafx.h"
#include "IMemoryHandler.h"
#include "DebugTypes.h"

class Sa1IRamHandler : public IMemoryHandler
{
private:
	uint8_t * _ram;

	__forceinline uint8_t InternalRead(uint32_t addr)
	{
		if(addr & 0x800) {
			return 0;
		} else {
			return _ram[addr & 0x7FF];
		}
	}

public:
	Sa1IRamHandler(uint8_t *ram)
	{
		_ram = ram;
		_memoryType = SnesMemoryType::Sa1InternalRam;
	}

	uint8_t Read(uint32_t addr) override
	{
		return InternalRead(addr);
	}

	uint8_t Peek(uint32_t addr) override
	{
		return InternalRead(addr);
	}

	void PeekBlock(uint32_t addr, uint8_t *output) override
	{
		for(int i = 0; i < 0x1000; i++) {
			output[i] = InternalRead(i);
		}
	}

	void Write(uint32_t addr, uint8_t value) override
	{
		if(!(addr & 0x800)) {
			_ram[addr & 0x7FF] = value;
		}
	}

	AddressInfo GetAbsoluteAddress(uint32_t addr) override
	{
		AddressInfo info;
		if(addr & 0x800) {
			info.Address = -1;
			info.Type = SnesMemoryType::CpuMemory;
		} else {
			info.Address = (addr & 0x7FF);
			info.Type = _memoryType;
		}
		return info;
	}
};