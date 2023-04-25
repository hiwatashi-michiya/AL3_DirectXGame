#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <Input.h>

//自キャラの弾
class PlayerBullet {
public:
	PlayerBullet();
	~PlayerBullet();
	
	//初期化
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

	//isDead_のゲッター
	//関数名の後ろのconstは
	//「このメンバ関数はメンバ変数を書き換えない」という指定
	bool IsDead() const { return isDead_; }

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;

private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	//画像
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

};

