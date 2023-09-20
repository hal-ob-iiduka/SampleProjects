#pragma once
#include <string>
#include "Event.h"

/**
* �񓯊����[�h���̎��s���ɌĂяo�����f���Q�[�g
*/
DECLARE_DELEGATE_PRAM_1(LoadAsyncDelegate, const std::string&)

/** 
* �������ɂ́A���[�h����A�Z�b�g�p�X���A
* �������ɂ́A���[�h�֐����w�肵�܂��B
* 
* ���̂悤�Ȏ����ɂ���̂͂��������R������܂��B
* ���̃T���v�����g�����Ă����ƍl�����ہAI/O�����ƃR���o�[�g������ʃX���b�h�Ŏ�������ꍇ�ȂǑz�������
* ����̎������ƌ��\���₷����ł͂Ȃ��ł��傤���B�i�����Ɨǂ����@�͂���Ǝv���܂����j
*/
void LoadAssetAsync(const std::string& asseetPath, LoadAsyncDelegate loadAsyncDelegate = LoadAsyncDelegate());