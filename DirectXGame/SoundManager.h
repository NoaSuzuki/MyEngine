#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class SoundManager
{
public:
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//オーディオのコールバック
	class XAudio2VoiceCallBack : public IXAudio2VoiceCallback {
	public:
		void OnVoiceProcessingPassStart(UINT32 BytesRequired) {};
		// ボイス処理パスの終了時
		STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};
		//バッファストリームの再生終了時
		STDMETHOD_(void, OnStreamEnd) (THIS) {};

		//バッファの使用次
		STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext) {};
		//バッファの末尾に達したとき
		STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext) {
			delete[] pBufferContext;//バッファ開放
		};
		//ループの達したとき
		STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext) {};
		//ボイスのエラー時
		STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error) {};
	};

	//チャンクヘッダ
	struct ChunkHeader
	{
		char id[4];//チャンク毎のID
		int size;//チャンクサイズ
	};

	//RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk;//RIFF
		char type[4];//WAVE
	};

	//FMTチャンク
	struct FormatChunk
	{
		ChunkHeader chunk;//fmt
		WAVEFORMAT fmt;//波形フォーマット
	};

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	//サウンドの再生
	void PlayWave(const char* filename);
private:
	//xAudio2のインスタンス
	ComPtr<IXAudio2> xAudio2;
	//マスターボイス
	IXAudio2MasteringVoice* masterVoice;
	XAudio2VoiceCallBack voiceCallback;
};

