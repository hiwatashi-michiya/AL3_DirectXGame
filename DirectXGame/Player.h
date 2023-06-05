#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <Input.h>
#include "PlayerBullet.h"
#include <list>

class Player {
public:
	Player();
	~Player();

	//初期化
	void Initialize(Model* model, uint32_t textureHandle, Vector3 position);

	//更新
	void Update();

	//描画
	void Draw(ViewProjection viewProjection);

	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

	//半径の取得
	float GetRadius() { return radius_; }

	//親となるワールドトランスフォームをセット
	void SetParent(const WorldTransform* parent);

private:

	//半径
	float radius_ = 1.0f;

	//キーボード入力
	Input* input_ = nullptr;

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//弾
	std::list<PlayerBullet*> bullets_;
	//攻撃
	void Attack();

};

