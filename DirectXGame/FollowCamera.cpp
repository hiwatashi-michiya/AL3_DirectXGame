#include "FollowCamera.h"

FollowCamera::FollowCamera() {}

FollowCamera::~FollowCamera() {}

void FollowCamera::Initialize() {

	viewProjection_.Initialize();
	viewProjection_.farZ = 2000.0f;
	viewProjection_.rotation_.x = 3.14f / 6.0f;

}

void FollowCamera::Update() {

	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		//回転速度
		float rotateSpeed = 0.001f;

		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX * rotateSpeed * rotateSpeed;
		viewProjection_.rotation_.x -= (float)joyState.Gamepad.sThumbRY * rotateSpeed * rotateSpeed / 3.0f;
		viewProjection_.rotation_.x = Clamp(viewProjection_.rotation_.x, 3.14f / 12.0f, 3.14f / 6.0f);

	}

	//追従対象がいれば
	if (target_) {

		//追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 30.0f, -60.0f};

		Matrix4x4 matRotate = MakeRotateYMatrix(viewProjection_.rotation_.y);

		//オフセットをカメラの回転に合わせて回転させる
		offset = TransformNormal(offset, matRotate);

		//座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset);

	}

	//ビュー行列の更新
	viewProjection_.UpdateMatrix();

}
