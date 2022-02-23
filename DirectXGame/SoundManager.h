#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class SoundManager
{
public:
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//�I�[�f�B�I�̃R�[���o�b�N
	class XAudio2VoiceCallBack : public IXAudio2VoiceCallback {
	public:
		void OnVoiceProcessingPassStart(UINT32 BytesRequired) {};
		// �{�C�X�����p�X�̏I����
		STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};
		//�o�b�t�@�X�g���[���̍Đ��I����
		STDMETHOD_(void, OnStreamEnd) (THIS) {};

		//�o�b�t�@�̎g�p��
		STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext) {};
		//�o�b�t�@�̖����ɒB�����Ƃ�
		STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext) {
			delete[] pBufferContext;//�o�b�t�@�J��
		};
		//���[�v�̒B�����Ƃ�
		STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext) {};
		//�{�C�X�̃G���[��
		STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error) {};
	};

	//�`�����N�w�b�_
	struct ChunkHeader
	{
		char id[4];//�`�����N����ID
		int size;//�`�����N�T�C�Y
	};

	//RIFF�w�b�_�`�����N
	struct RiffHeader
	{
		ChunkHeader chunk;//RIFF
		char type[4];//WAVE
	};

	//FMT�`�����N
	struct FormatChunk
	{
		ChunkHeader chunk;//fmt
		WAVEFORMAT fmt;//�g�`�t�H�[�}�b�g
	};

public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	//�T�E���h�̍Đ�
	void PlayWave(const char* filename);
private:
	//xAudio2�̃C���X�^���X
	ComPtr<IXAudio2> xAudio2;
	//�}�X�^�[�{�C�X
	IXAudio2MasteringVoice* masterVoice;
	XAudio2VoiceCallBack voiceCallback;
};

