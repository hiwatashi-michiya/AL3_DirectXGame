#pragma once
#include <WorldTransform.h>
#include <ViewProjection.h>
#include <Model.h>
#include <memory>

const int kMaxParticles = 10;

class Effect {
public:

	void Initialize(int32_t lifeTime);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void SetEffect(const Vector3& startPosition);

	void SetColor(int32_t color);

	bool IsDead() const { return isDead_; }

private:

	std::unique_ptr<Model> effectModel_;

	WorldTransform worldTransforms_[kMaxParticles];
	Vector3 velocities_[kMaxParticles];

	//生存時間
	int32_t lifeTimer_;

	bool isDead_ = false;

	const int32_t kColorRed = 0;
	const int32_t kColorGreen = 1;
	const int32_t kColorBlue = 2;

	uint32_t colorTex_[3]{};

	uint32_t effectTex_ = 0u;

};
