#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include<map>

class SoundManager
{
public:
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	template<class T1, class T2> using map = std::map<T1, T2>;

public:
	
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

	struct SoundData
	{
		WAVEFORMATEX wfex{};//波形フォーマット
		char* pBuffer;//波形データ
		unsigned int datasize;//波形データのサイズ
	};

public:
	//デスクトラクタ
	~SoundManager();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// サウンド読み込み
	/// </summary>
	/// <param name="filename"></param>
	void LoadWave(int number ,const char* filename);
	
	//サウンドの再生
	void PlayWave(int number);
private:
	//xAudio2のインスタンス
	ComPtr<IXAudio2> xAudio2;
	//マスターボイス
	IXAudio2MasteringVoice* masterVoice;
	//波形データの連想配列
	map<int, SoundData>soundDatas;
};

