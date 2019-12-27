#pragma once
#include "stdafx.h"
#include "BaseControlDevice.h"
#include "Console.h"
#include "Debugger.h"

class SystemActionManager : public BaseControlDevice
{
private:
	bool _needReset = false;
	bool _needPowerCycle = false;

protected:
	string GetKeyNames() override
	{
		return "RP";
	}
	
public:
	enum Buttons { ResetButton = 0, PowerButton = 1 };

	SystemActionManager(Console* console) : BaseControlDevice(console, BaseControlDevice::ConsoleInputPort)
	{
	}

	ControllerType GetControllerType() override
	{
		return ControllerType::None;
	}

	uint8_t ReadRam(uint16_t addr) override
	{
		return 0;
	}

	void WriteRam(uint16_t addr, uint8_t value) override
	{
	}

	void OnAfterSetState() override
	{
		if(_needReset) {
			SetBit(SystemActionManager::Buttons::ResetButton);
		}
		if(_needPowerCycle) {
			SetBit(SystemActionManager::Buttons::PowerButton);
		}
	}

	bool Reset()
	{
		if(!_needReset) {
			shared_ptr<Debugger> debugger = _console->GetDebugger(false);
			if(debugger) {
				debugger->SuspendDebugger(false);
				debugger->Run();
			}

			_needReset = true;
			return true;
		}
		return false;
	}

	bool PowerCycle()
	{
		if(!_needPowerCycle) {
			shared_ptr<Debugger> debugger = _console->GetDebugger(false);
			if(debugger) {
				debugger->SuspendDebugger(false);
				debugger->Run();
			}

			_needPowerCycle = true;
			return true;
		}
		return false;
	}

	bool IsResetPressed()
	{
		_needReset = false;
		return IsPressed(SystemActionManager::Buttons::ResetButton);
	}

	bool IsPowerCyclePressed()
	{
		_needPowerCycle = false;
		return IsPressed(SystemActionManager::Buttons::PowerButton);
	}
};