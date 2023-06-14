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

//自機クラスの前方宣言
class Player;

//ゲームシーンの前方宣言
class GameScene;

class Enemy {
public:
	Enemy();
	~Enemy();

	//初期化
	void Initialize(Model* model, uint32_t textureHandle, GameScene* gameScene);
	//接近フェーズ初期化
	void InitPhaseApproach();
	//更新
	void Update();
	//描画
	void Draw(ViewProjection viewProjection);
	//弾発射
	void Fire();
	//発射間隔
	static const int kFireInterval = 60;

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// 半径の取得
	float GetRadius() { return radius_; }

	//ゲームシーンのセット
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//初期座標のセット
	void SetStartPosition(Vector3 position);

	//生きているかどうかの判定取得
	bool IsDead() const { return isDead_; }

private:

	// 半径
	float radius_ = 1.0f;

	//接近フェーズ
	void PhaseApproach();

	//離脱フェーズ
	void PhaseLeave();

	//生きているか
	bool isDead_ = false;

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//発射タイマー
	int32_t fireTimer_ = 0;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//フェーズ
	Phase phase_ = Phase::Approach;

	//自キャラ
	Player* player_ = nullptr;

	//ゲームシーン
	GameScene* gameScene_ = nullptr;

	//当たったかどうか
	bool isHit_ = false;

};


