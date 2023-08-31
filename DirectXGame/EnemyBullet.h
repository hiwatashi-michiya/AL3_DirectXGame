#pragma once
#pragma once
#include <vector>
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include "Collider.h"
#include <Audio.h>

class EnemyBullet : public Collider {

	public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() { return isDead_; }

	static const int32_t kLifeTime = 180;

	// 衝突時に呼ばれる関数
	void OnCollision(Collider* collider) override;

	// ワールド座標を取得(純粋仮想関数)
	Vector3 GetWorldPosition() override;

private:

	Audio* audio_ = nullptr;

	// 弾モデル
	std::unique_ptr<Model> modelBullet_ = nullptr;

	WorldTransform worldTransform_;

	// 速度
	Vector3 velocity_;

	// 弾の生存フラグ
	bool isDead_ = false;

	int32_t deathTimer_ = kLifeTime;

	uint32_t deadSE_ = 0u;

};
