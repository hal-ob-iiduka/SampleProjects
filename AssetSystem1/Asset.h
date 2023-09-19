#pragma once
#include <string>

class IAsset
{
public:

	virtual ~IAsset()
	{

	}

	/** 
	* �h����̃A�Z�b�g�Ǝ��̐�p���[�h��������������B
	* ���[�h�������ɖ߂�l�Ƃ��āAtrue��Ԃ��悤�ɂ���B
	*/
	virtual bool Load(const std::string& assetPath) = 0;
};