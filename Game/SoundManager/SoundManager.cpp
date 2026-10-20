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

	/**SE*/
	const char* SE_FILEPATH = "Assets/SE/"; 
	const char* SE_FILE_FORMAT = ".wav";
	const char* SE_FILENAME_LIST[enSENum] =
	{
		"ChoiceSE",
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

	/** SEを登録(BGMの後ろの番号から) */
	m_seCount = enSENum;
	for (int i = 0; i < enSENum; i++)
	{
		std::string seFile = std::string(SE_FILEPATH) + SE_FILENAME_LIST[i] + SE_FILE_FORMAT;
		g_soundEngine->ResistWaveFileBank(m_bgmCount + i, seFile.c_str());
	}
}

SoundManager::~SoundManager()
{

}

void SoundManager::Update()
{
	if (m_nowBGM != nullptr)
	{
		m_nowBGM->SetVolume(m_bgmVolume);
	}
}

SoundSource* SoundManager::PlayingBGM(SoundBGM number, bool isLoop)
{

	/** 新しく音を鳴らすためのオブジェクトを生成*/
	SoundSource* bgm = NewGO<SoundSource>(0);
	bgm->Init(number);
	bgm->SetVolume(m_bgmVolume);
	bgm->Play(isLoop);
	m_nowBGM = bgm;
	return bgm;
}


SoundSource* SoundManager::PlayingSE(SoundSE number, bool isLoop)
{
	/** 新しく音を鳴らすオブジェクトを作成*/
	SoundSource* se = NewGO<SoundSource>(0);

	/** SEはBGMの後ろに登録されているのでオフセットを渡す*/
	se->Init(number + m_bgmCount);

	/** 音量を設定*/
	se->SetVolume(m_seVolume);

	se->Play(isLoop);

	return se;
}