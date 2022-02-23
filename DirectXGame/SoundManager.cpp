#include "SoundManager.h"
#include <fstream>
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

SoundManager::~SoundManager()
{
	//読み込み済みサウンドの波形データを解放
	for (auto& pair : soundDatas) {
		delete pair.second.pBuffer;
	}
}

void SoundManager::Initialize()
{
	HRESULT result;

	//インスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));
	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));

}

void SoundManager::LoadWave(int number,const char* filename)
{
	SoundData soundData{};

	HRESULT result;
	// ファイルストリーム
	std::ifstream file;
	// Waveファイルを開く
	file.open(filename, std::ios_base::binary);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}
	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	// Formatチャンクの読み込み
	FormatChunk format;
	file.read((char*)&format, sizeof(format));
	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	// Dataチャンクのデータ部（波形データ）の読み込み
	soundData.pBuffer = new char[data.size];
	file.read(soundData.pBuffer, data.size);
	//波形データのサイズを記録
	soundData.datasize = data.size;
	// Waveファイルを閉じる
	file.close();
	// 波形フォーマットの設定
	memcpy(&soundData.wfex, &format.fmt, sizeof(format.fmt));
	soundData.wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	//連想配列に要素を追加
	soundDatas.insert(std::make_pair(number, soundData));
}

void SoundManager::PlayWave(int number)
{
	SoundData& soundData = soundDatas[number];
	HRESULT result;

	// 波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex, 0, 2.0f, nullptr);
	assert(SUCCEEDED(result));
	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)soundData.pBuffer;
	buf.pContext = soundData.pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = soundData.datasize;

	// 波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	//XAUDIO2_VOICE_STATE xaudio2state;
	//pSourceVoice->GetState(&xaudio2state);
	result = pSourceVoice->Start();

}
