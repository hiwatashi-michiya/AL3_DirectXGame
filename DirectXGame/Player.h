#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <Input.h>
#include "PlayerBullet.h"
#include <list>
#include "Collider.h"

class Player : public Collider {
public:
	Player();
	~Player();

	//初期化
	void Initialize(Model* model, uint32_t textureHandle);

	//更新
	void Update();

	//描画
	void Draw(ViewProjection viewProjection);

	Vector3 GetWorldPosition() override;

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

	//半径の取得
	float GetRadius() { return radius_; }

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

