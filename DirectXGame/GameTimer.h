#pragma once
#include <TextureManager.h>
#include <Sprite.h>
#include <memory>

const int kMaxTimeDigits = 2;

class GameTimer {
public:

	void Initialize();

	void Update();

	void Draw();

private:

	uint32_t numTex_ = 0u;

	std::unique_ptr<Sprite> numSprite_[kMaxTimeDigits];

};
