#include "Skydome.h"
#include <cassert>

Skydome::Skydome() {}

Skydome::~Skydome() {}

void Skydome::Initialize(Model* model) {

	assert(model);

	model_ = model;
	worldTransform_.Initialize();

}

void Skydome::Update() {

}

void Skydome::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection);

}
