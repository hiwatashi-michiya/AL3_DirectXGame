#include "Bullet.h"

void Bullet::Initialize(const std::vector<Model*>& models, const Vector3& position, const Vector3& velocity) {

	models_ = models;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
	velocity_ = velocity;

	SetCollisionAttribute(0x00000001);
	SetCollisionMask(0xfffffffe);

}

void Bullet::Update() {


	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void Bullet::Draw(const ViewProjection& viewProjection) {

	models_[0]->Draw(worldTransform_, viewProjection);

}

void Bullet::OnCollision() {

	isDead_ = true;

}

Vector3 Bullet::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}
