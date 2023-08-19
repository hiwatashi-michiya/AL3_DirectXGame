#pragma once
#include <vector>
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include "Collider.h"

class Bullet : public Collider {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models, const Vector3& position, const Vector3& velocity);

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
	void OnCollision() override;

	// ワールド座標を取得(純粋仮想関数)
	Vector3 GetWorldPosition() override;

private:

	std::vector<Model*> models_;
	
	WorldTransform worldTransform_;

	//速度
	Vector3 velocity_;

	//弾の生存フラグ
	bool isDead_ = false;

	int32_t deathTimer_ = kLifeTime;

};
