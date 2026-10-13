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

/** サウンドマネージャークラス */
class SoundManager : public IGameObject
{
public:
	SoundManager();
	~SoundManager();
	void Update();

	/** BGMを再生する関数 */
	SoundSource* PlayingBGM(SoundBGM number, bool isLoop = true);

public:
	/** BGMの音量 */
	float m_bgmVolume = 0.3f;

	/** BGMの数 */
	uint8_t m_bgmCount = 0;
};

