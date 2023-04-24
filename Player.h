#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <Input.h>
#include "PlayerBullet.h"

class Player {
public:
	Player();
	~Player();

	//初期化
	void Initialize(Model* model, uint32_t textureHandle);

	//更新
	void Update();

	//描画
	void Draw(ViewProjection viewProjection);


private:

	//キーボード入力
	Input* input_ = nullptr;

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//弾
	PlayerBullet* bullet_ = nullptr;
	//攻撃
	void Attack();

};

