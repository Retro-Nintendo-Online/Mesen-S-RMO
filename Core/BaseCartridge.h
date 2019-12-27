#pragma once
#include "stdafx.h"
#include "IMemoryHandler.h"
#include "CartTypes.h"
#include "../Utilities/ISerializable.h"

class BaseCoprocessor;
class MemoryMappings;
class VirtualFile;
class EmuSettings;
class NecDsp;
class Sa1;
class Gsu;
class Cx4;
class Console;
class SpcFileData;

class BaseCartridge : public ISerializable
{
private:
	Console *_console;

	vector<unique_ptr<IMemoryHandler>> _prgRomHandlers;
	vector<unique_ptr<IMemoryHandler>> _saveRamHandlers;
	SnesCartInformation _cartInfo = {};

	unique_ptr<BaseCoprocessor> _coprocessor;
	NecDsp *_necDsp = nullptr;
	Sa1 *_sa1 = nullptr;
	Gsu *_gsu = nullptr;
	Cx4 *_cx4 = nullptr;

	CartFlags::CartFlags _flags = CartFlags::CartFlags::None;
	CoprocessorType _coprocessorType = CoprocessorType::None;
	bool _hasBattery = false;
	string _romPath;
	string _patchPath;

	uint8_t* _prgRom = nullptr;
	uint8_t* _saveRam = nullptr;
	
	uint32_t _prgRomSize = 0;
	uint32_t _saveRamSize = 0;
	uint32_t _coprocessorRamSize = 0;
	
	shared_ptr<SpcFileData> _spcData;
	vector<uint8_t> _embeddedFirmware;

	void LoadBattery();

	int32_t GetHeaderScore(uint32_t addr);
	void DisplayCartInfo();

	CoprocessorType GetCoprocessorType();
	CoprocessorType GetSt01xVersion();
	CoprocessorType GetDspVersion();

	bool MapSpecificCarts(MemoryMappings &mm);
	void ApplyConfigOverrides();
	
	void LoadRom();
	void LoadSpc();
	void InitCoprocessor();
	void LoadEmbeddedFirmware();

	string GetCartName();
	string GetGameCode();

public:
	virtual ~BaseCartridge();

	static shared_ptr<BaseCartridge> CreateCartridge(Console* console, VirtualFile &romFile, VirtualFile &patchFile);

	void Reset();

	void SaveBattery();

	void Init(MemoryMappings &mm);

	RomInfo GetRomInfo();
	string GetSha1Hash();
	CartFlags::CartFlags GetCartFlags();

	void RegisterHandlers(MemoryMappings &mm);

	uint8_t* DebugGetPrgRom() { return _prgRom; }
	uint8_t* DebugGetSaveRam() { return _saveRam; }
	uint32_t DebugGetPrgRomSize() { return _prgRomSize; }
	uint32_t DebugGetSaveRamSize() { return _saveRamSize; }

	NecDsp* GetDsp();
	Sa1* GetSa1();
	Gsu* GetGsu();
	Cx4* GetCx4();

	void RunCoprocessors();

	BaseCoprocessor* GetCoprocessor();

	vector<unique_ptr<IMemoryHandler>>& GetPrgRomHandlers();
	vector<unique_ptr<IMemoryHandler>>& GetSaveRamHandlers();

	SpcFileData* GetSpcData();

	void Serialize(Serializer &s) override;
};
