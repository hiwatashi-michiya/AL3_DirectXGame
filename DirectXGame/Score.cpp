#include "Score.h"
#include "GameTimer.h"

Score* Score::GetInstance() {

	static Score instance;
	return &instance;

}

void Score::Initialize() {

	currentScore_ = 0;
	highScore_ = 0;

	numTex_ = TextureManager::Load("score/number.png");
	scoreTex_ = TextureManager::Load("UI/score.png");
	for (int i = 0; i < kMaxDigits; i++) {
		numSprite_[i].reset(Sprite::Create(numTex_, {0.0f, 0.0f}));
		numSprite_[i]->SetSize({32.0f, 64.0f});
		numSprite_[i]->SetTextureRect({0.0f, 0.0f}, {32.0f, 64.0f});
		numSprite_[i]->SetPosition({i * 32.0f + 1046.0f, 10.0f});
	}
	scoreSprite_.reset(Sprite::Create(scoreTex_, {0.0f, 0.0f}));
	scoreSprite_->SetPosition({886.0f, 10.0f});

}

void Score::Update() {

	if (GameTimer::GetInstance()->GetGameTime() <= 0) {
		
		for (int i = 0; i < kMaxDigits; i++) {
			numSprite_[i]->SetPosition({i * 32.0f + 528.0f, 360.0f});
		}

		scoreSprite_->SetTextureRect({0.0f, 0.0f}, {135.0f, 64.0f});
		scoreSprite_->SetPosition({560.0f, 300.0f});

	}
	else {

		for (int i = 0; i < kMaxDigits; i++) {
			numSprite_[i]->SetPosition({i * 32.0f + 1046.0f, 10.0f});
		}

		scoreSprite_->SetTextureRect({0.0f, 0.0f}, {160.0f, 64.0f});
		scoreSprite_->SetPosition({886.0f, 10.0f});

	}

	UpdateViewScore();

}

void Score::Draw() {

	for (int i = 0; i < kMaxDigits; i++) {
		numSprite_[i]->Draw();
	}

	scoreSprite_->Draw();

}

void Score::UpdateViewScore() {

	//各桁を表示するための計算
	int divideNum = currentScore_;

	int tmpDigit = int(pow(10, kMaxDigits - 1));

	for (int i = 0; i < kMaxDigits; i++) {

		int numDigit = divideNum / tmpDigit;

		numSprite_[i]->SetTextureRect({numDigit * 32.0f, 0.0f}, {32.0f,64.0f});

		divideNum -= numDigit * tmpDigit;

		tmpDigit /= 10;

	}

}
