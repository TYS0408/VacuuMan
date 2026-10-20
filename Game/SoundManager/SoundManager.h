#pragma once
#include"sound/SoundSource.h"
enum SoundBGM
{
	/** タイトルBGM*/
	enTitleBGM,
	/** インゲームBGM*/
	enInGameBGM,
	enBGMNum
};

enum SoundSE
{
	enChoiceSE,
	enSENum,
};


/** サウンドマネージャークラス */
class SoundManager : public IGameObject
{
public:
	SoundManager();
	~SoundManager();
	void Update();

	/** BGMを再生する関数 */
	SoundSource* PlayingBGM(SoundBGM number, bool isLoop = true);

	SoundSource* PlayingSE(SoundSE number, bool isLoop = true);
public:
	/** BGMの音量 */
	float m_bgmVolume = 0.5f;

	/** seを再生する変数*/
	float m_seVolume = 0.5f;
	/** BGMの数 */
	uint8_t m_bgmCount = 0;

	/** 今鳴っているBGM*/
	SoundSource* m_nowBGM = nullptr;
	/** SEの数 */
	uint8_t m_seCount = 0;
};

