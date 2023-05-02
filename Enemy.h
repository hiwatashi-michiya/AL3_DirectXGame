#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>

enum class Phase {
	Approach, //接近
	Leave, //離脱
};

class Enemy {
public:
	Enemy();
	~Enemy();

	//初期化
	void Initialize(Model* model, uint32_t textureHandle);
	//更新
	void Update();
	//描画
	void Draw(ViewProjection viewProjection);

private:

	//接近フェーズ
	void PhaseApproach();

	//離脱フェーズ
	void PhaseLeave();

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//フェーズ
	Phase phase_ = Phase::Approach;

};


