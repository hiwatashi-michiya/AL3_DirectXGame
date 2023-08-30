#include "EnemyBullet.h"

void EnemyBullet::Initialize(const Vector3& position, const Vector3& velocity) {

	modelBullet_.reset(Model::CreateFromOBJ("enemybullet", true));

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
	velocity_ = velocity;

	SetCollisionAttribute(0x00000002);
	SetCollisionMask(0xfffffffd);
}

void EnemyBullet::Update() {

	worldTransform_.translation_ += velocity_;
	worldTransform_.rotation_.x += 3.14f / 12.0f;

	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {

	modelBullet_->Draw(worldTransform_, viewProjection);
}

void EnemyBullet::OnCollision(Collider* collider) {

	SetColorType(collider->GetColorType());

	isDead_ = true;

}

Vector3 EnemyBullet::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
