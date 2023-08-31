#pragma once
#include <TextureManager.h>
#include <Sprite.h>
#include <memory>

const int kMaxTimeDigits = 2;

class GameTimer {
public:

	static GameTimer* GetInstance();

	void Initialize();

	void Update();

	void Draw();

	int32_t GetGameTime() const { return gameTimer_; }

	void ResetGameTime() { gameTimer_ = kMaxGameTime; }

	bool GetIsCount() const { return isCount_; } 

	void SetIsCount(bool flag) { isCount_ = flag; }

private:

	bool isCount_ = true;

	const int32_t kMaxGameTime = 6000;

	int32_t gameTimer_ = kMaxGameTime;

	uint32_t numTex_ = 0u;
	uint32_t timeTex_ = 0u;

	std::unique_ptr<Sprite> numSprite_[kMaxTimeDigits];
	std::unique_ptr<Sprite> timeSprite_;

private:

	GameTimer() = default;
	~GameTimer() = default;
	GameTimer(const GameTimer&) = delete;
	const GameTimer& operator=(const GameTimer&) = delete;

};
