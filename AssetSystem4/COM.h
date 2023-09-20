#pragma once

enum class COMID
{
	MULTITHREADED = 0x0,		// OLE calls objects on any thread.
	APARTMENTTHREADED = 0x2,	// Apartment model
	DISABLE_OLE1DDE = 0x4,		// Don't use DDE for Ole1 support.
	SPEED_OVER_MEMORY = 0x8,	// Trade memory for speed.
};

/**
* �}���`�X���b�h�����Ŋe�X���b�h�� COM ���C�u�����������������邽�ߍ쐬�B
*/
class COM
{
public:

	COM();
	COM(COMID comID);

	~COM();
};