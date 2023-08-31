#include "GameTimer.h"

GameTimer* GameTimer::GetInstance() {

	static GameTimer instance;
	return &instance;

}

void GameTimer::Initialize() {

	numTex_ = TextureManager::Load("score/number.png");
	timeTex_ = TextureManager::Load("UI/time_2.png");
	for (int i = 0; i < kMaxTimeDigits; i++) {
		numSprite_[i].reset(Sprite::Create(numTex_, {0.0f, 0.0f}));
		numSprite_[i]->SetSize({32.0f, 64.0f});
		numSprite_[i]->SetTextureRect({0.0f, 0.0f}, {32.0f, 64.0f});
		numSprite_[i]->SetPosition({i * 32.0f + 608.0f, 70.0f});
	}
	timeSprite_.reset(Sprite::Create(timeTex_, {0.0f, 0.0f}));
	timeSprite_->SetPosition({576.0f, 10.0f});

}

void GameTimer::Update() {

	if (isCount_) {

		if (--gameTimer_ <= 0) {
			isCount_ = false;
		}

	}

	// 各桁を表示するための計算
	int divideNum = gameTimer_ / 60;

	int tmpDigit = int(pow(10, kMaxTimeDigits - 1));

	for (int i = 0; i < kMaxTimeDigits; i++) {

		int numDigit = divideNum / tmpDigit;

		numSprite_[i]->SetTextureRect({numDigit * 32.0f, 0.0f}, {32.0f, 64.0f});

		divideNum -= numDigit * tmpDigit;

		tmpDigit /= 10;
	}

}

void GameTimer::Draw() {

	for (int i = 0; i < kMaxTimeDigits; i++) {
		numSprite_[i]->Draw();
	}
	timeSprite_->Draw();

}
