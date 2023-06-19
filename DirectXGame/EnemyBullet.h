#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>
#include "Collider.h"

class EnemyBullet : public Collider {
public:
	EnemyBullet();
	~EnemyBullet();

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool isDead() const { return isDead_; }

	static const int32_t kLifeTime = 60 * 5;

	Vector3 GetWorldPosition() override;

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	// 半径の取得
	float GetRadius() { return radius_; }

private:

	// 半径
	float radius_ = 1.0f;

	WorldTransform worldTransform_;

	Model* model_ = nullptr;
	
	uint32_t textureHandle_ = 0u;

	Vector3 velocity_;

	int32_t deathTimer_ = kLifeTime;

	bool isDead_ = false;

};


