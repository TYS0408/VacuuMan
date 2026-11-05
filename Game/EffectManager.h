#pragma once
/** エフェクトのID*/
enum class EffectID
{
	/** ステージ分割エフェクト*/
	EffectID_StageDivision,
};
class EffectManager : public IGameObject
{
public:
	EffectManager() {};
	~EffectManager() {};

	/** エフェクトの初期化*/
	bool Start()override;

	/** エフェクトの読み込む関数*/
	void LoadEffects();
};

