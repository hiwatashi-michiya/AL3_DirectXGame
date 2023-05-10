#pragma once

#include <functional>

class TimedCall {
public:
	TimedCall(std::function<void(void)> func, uint32_t time);
	~TimedCall();

	//更新処理
	void Update();
	//完了ならtrueを返す
	bool isFinished() { return completion_; }

private:

	//コールバック
	std::function<void(void)> func_;
	//残り時間
	uint32_t time_;
	//完了フラグ
	bool completion_ = false;

};


