#include "stdafx.h"
#include "SoundManager.h"
namespace
{
	const char* BGM_FILEPATH = "Assets/BGM/";
	const char* SOUND_FILE_FORMAT = ".wav";

	/** BGMファイル名一覧(enumと同じ順番にすること)*/
	const char* BGM_FILENAME_LIST[enBGMNum] =
	{
		"TitleBGM",
		"InGameBGM",
	};
}

SoundManager::SoundManager()
{
	m_bgmCount = enBGMNum;
	/** BGMを登録(番号は0〜)*/
	for (int i = 0; i < enBGMNum; i++)
	{
		std::string bgmFile = std::string(BGM_FILEPATH) + BGM_FILENAME_LIST[i] + (SOUND_FILE_FORMAT);
		const char* bgmFilePath = bgmFile.c_str();
		g_soundEngine->ResistWaveFileBank(i, bgmFilePath);
	}
}

SoundManager::~SoundManager()
{

}

void SoundManager::Update()
{

}

SoundSource* SoundManager::PlayingBGM(SoundBGM number, bool isLoop)
{
	/** 新しく音を鳴らすためのオブジェクトを生成*/
	SoundSource* bgm = NewGO<SoundSource>(0);
	bgm->Init(number);
	bgm->SetVolume(m_bgmVolume);
	bgm->Play(isLoop);
	return bgm;
}