#include "RailCamera.h"
#include <imgui.h>

RailCamera::RailCamera() {}

RailCamera::~RailCamera() {}

void RailCamera::Initialize(const Vector3& transform,const Vector3& radian) {

	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = transform;
	worldTransform_.rotation_ = radian;
	//ビュープロジェクションの初期化
	viewProjection_.farZ = 2200.0f;
	viewProjection_.Initialize();

}

void RailCamera::Update() {

	Vector3 move{0.0f, 0.0f, -0.1f};
	Vector3 rotate{0.0f, 0.001f, 0.0f};

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	worldTransform_.rotation_ = Add(worldTransform_.rotation_, rotate);
	
	if (worldTransform_.rotation_.y >= 6.28f) {
		worldTransform_.rotation_.y = 0.0f;
	}
	
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translation", &worldTransform_.translation_.x, -640.0f, 640.0f, "%1.3f");
	ImGui::SliderFloat3("rotation", &worldTransform_.rotation_.x, 0.0f, 10.0f, "%1.3f");
	ImGui::End();
}
