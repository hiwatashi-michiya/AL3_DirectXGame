#pragma once
#include <Model.h>
#include <WorldTransform.h>
#include <ViewProjection.h>

class EnemyBullet {
public:
	EnemyBullet();
	~EnemyBullet();

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool isDead() const { return isDead_; }

	static const int32_t kLifeTime = 60 * 5;

private:

	WorldTransform worldTransform_;

	Model* model_ = nullptr;
	
	uint32_t textureHandle_ = 0u;

	Vector3 velocity_;

	int32_t deathTimer_ = kLifeTime;

	bool isDead_ = false;

};


