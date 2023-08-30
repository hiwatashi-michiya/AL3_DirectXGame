#include "Bullet.h"

void Bullet::Initialize(const std::vector<Model*>& models, const Vector3& position, const Vector3& velocity, const int32_t& color) {

	models_ = models;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
	velocity_ = velocity;

	SetCollisionAttribute(0x00000001);
	SetCollisionMask(0xfffffffe);

	colorTex_[kColorRed] = TextureManager::Load("playerBullet/red.png");
	colorTex_[kColorGreen] = TextureManager::Load("playerBullet/green.png");
	colorTex_[kColorBlue] = TextureManager::Load("playerBullet/blue.png");

	bulletTex_ = colorTex_[color];
	SetColorType(color);

}

void Bullet::Update() {

	switch (GetColorType()) {
	default:
	case C_RED:

		break;

	case C_GREEN:

		break;

	case C_BLUE:

		break;
	}

	worldTransform_.translation_ += velocity_;

	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void Bullet::Draw(const ViewProjection& viewProjection) {

	models_[0]->Draw(worldTransform_, viewProjection, bulletTex_);

}

void Bullet::OnCollision(Collider* collider) {

	SetColorType(collider->GetColorType());

	isDead_ = true;

}

Vector3 Bullet::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}
