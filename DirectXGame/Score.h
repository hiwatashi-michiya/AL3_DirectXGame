#pragma once
#include <TextureManager.h>
#include <Sprite.h>
#include <memory>

// 最大桁数
const int kMaxDigits = 8;

class Score {
public:

	static Score* GetInstance();

	void Initialize();

	void Update();

	void Draw();

	/// <summary>
	/// スコア加算
	/// </summary>
	/// <param name="val">加算値</param>
	/// <param name="mag">倍率</param>
	void AddScore(int val, float mag = 1.0f) { 
		currentScore_ += int(val * mag); 
		currentScore_ = int(Clamp(float(currentScore_), 0.0f, 99999999.0f));
	}

	/// <summary>
	/// スコア減算
	/// </summary>
	/// <param name="val">減算値</param>
	/// <param name="mag">倍率</param>
	void SubtractScore(int val, float mag = 1.0f) { 
		currentScore_ -= int(val * mag); 
		currentScore_ = int(Clamp(float(currentScore_), 0.0f, 99999999.0f));
	}

	/// <summary>
	/// 倍率のセット
	/// </summary>
	void SetMagScore(float val) { magScore_ = val; }

	float GetMagScore() { return magScore_; }

	/// <summary>
	/// スコアリセット
	/// </summary>
	void ResetScore() { currentScore_ = 0; }

private:

	int currentScore_;

	int highScore_;

	//スコア倍率
	float magScore_ = 1.0f;

	uint32_t numTex_ = 0u;
	uint32_t scoreTex_ = 0u;

	std::unique_ptr<Sprite> numSprite_[kMaxDigits];
	std::unique_ptr<Sprite> scoreSprite_;

	//スコアの表示更新
	void UpdateViewScore();

private:
	Score() = default;
	~Score() = default;
	Score(const Score&) = delete;
	const Score& operator=(const Score&) = delete;

};
