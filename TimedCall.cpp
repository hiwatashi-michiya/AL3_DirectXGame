#include "TimedCall.h"
#include "Enemy.h"

TimedCall::TimedCall(std::function<void(void)> func, uint32_t time)
 : func_(func), time_(time) {

}

TimedCall::~TimedCall() {}

void TimedCall::Update() {

	if (completion_) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		completion_ = true;
		//コールバック関数呼び出し
		func_();
	}

}
