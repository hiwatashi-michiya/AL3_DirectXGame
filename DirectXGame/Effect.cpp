#include "Effect.h"
#include "Rand.h"

void Effect::Initialize(int32_t lifeTime) {

	effectModel_.reset(Model::CreateFromOBJ("particlecube", true));
	for (int i = 0; i < kMaxParticles; i++) {
		worldTransforms_[i].Initialize();
		worldTransforms_[i].scale_ *= 0.5f;
	}

	lifeTimer_ = lifeTime;

	colorTex_[kColorRed] = TextureManager::Load("particleball/enemy_red.png");
	colorTex_[kColorGreen] = TextureManager::Load("particleball/enemy_green.png");
	colorTex_[kColorBlue] = TextureManager::Load("particleball/enemy_blue.png");

	effectTex_ = colorTex_[kColorRed];

}

void Effect::Update() {

	if (--lifeTimer_ <= 0) {
		isDead_ = true;
	}

	for (int i = 0; i < kMaxParticles; i++) {
		worldTransforms_[i].translation_ += velocities_[i];
		worldTransforms_[i].UpdateMatrix();
	}

}

void Effect::Draw(const ViewProjection& viewProjection) {

	for (int i = 0; i < kMaxParticles; i++) {

		effectModel_->Draw(worldTransforms_[i], viewProjection, effectTex_);

	}

}

void Effect::SetEffect(const Vector3& startPosition) {

	for (int i = 0; i < kMaxParticles; i++) {

		worldTransforms_[i].translation_ = startPosition;
		
		if (i % 2 == 0) {

			velocities_[i].x = cosf(i * 3.14f / 5.0f);
			velocities_[i].y = 0.5f;
			velocities_[i].z = sinf(i * 3.14f / 5.0f);

			velocities_[i] *= 0.1f;

		}
		else {

			velocities_[i].x = cosf(i * 3.14f / 5.0f);
			velocities_[i].y = 1.0f;
			velocities_[i].z = sinf(i * 3.14f / 5.0f);

			velocities_[i] *= 0.1f;

		}

		worldTransforms_[i].UpdateMatrix();

	}


}

void Effect::SetColor(int32_t color) {

	effectTex_ = colorTex_[color];

}
