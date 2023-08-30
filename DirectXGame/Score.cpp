#include "Score.h"

Score* Score::GetInstance() {

	static Score instance;
	return &instance;

}

void Score::Initialize() {

	currentScore_ = 0;
	highScore_ = 0;

	numTex_ = TextureManager::Load("score/number.png");
	for (int i = 0; i < kMaxDigits; i++) {
		numSprite_[i].reset(Sprite::Create(numTex_, {0.0f, 0.0f}));
		numSprite_[i]->SetSize({32.0f, 64.0f});
		numSprite_[i]->SetTextureRect({0.0f, 0.0f}, {32.0f, 64.0f});
		numSprite_[i]->SetPosition({i * 32.0f + 1046.0f, 10.0f});
	}
	

}

void Score::Update() {

	UpdateViewScore();

}

void Score::Draw() {

	for (int i = 0; i < kMaxDigits; i++) {
		numSprite_[i]->Draw();
	}

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
