#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include "EnemyBullet.h"
#include <list>

enum class Phase {
	Approach, //接近
	Leave, //離脱
};

class TimedCall;

class Enemy {
public:
	Enemy();
	~Enemy();

	//初期化
	void Initialize(Model* model, uint32_t textureHandle);
	//接近フェーズ初期化
	void InitPhaseApproach();
	//更新
	void Update();
	//描画
	void Draw(ViewProjection viewProjection);
	//弾発射
	void Fire();
	//弾を発射してタイマーをリセットするコールバック関数
	void LoopFire();
	//発射間隔
	static const int kFireInterval = 60;

private:

	//接近フェーズ
	void PhaseApproach();

	//離脱フェーズ
	void PhaseLeave();

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//弾
	std::list<EnemyBullet*> bullets_;

	//時限発動のリスト
	std::list<TimedCall*> timedCalls_;

	//発射タイマー
	int32_t fireTimer_ = 0;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//フェーズ
	Phase phase_ = Phase::Approach;

};


