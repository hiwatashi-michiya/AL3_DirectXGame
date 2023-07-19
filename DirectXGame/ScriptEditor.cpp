#include "ScriptEditor.h"
#include <ImGuiManager.h>
#include <WinApp.h>
#include <TextureManager.h>
#include <Sprite.h>

ScriptEditor::ScriptEditor() {}

ScriptEditor::~ScriptEditor() {}

void ScriptEditor::Initialize() {

	isEdit_ = true;
	input_ = Input::GetInstance();
	model_ = Model::Create();
	matCamera_ = MakeIdentity4x4();
	matView_ = MakeIdentity4x4();
	matProjection_ = MakePerspectiveFovMatrix(
	    0.45f, float(WinApp::kWindowWidth) / float(WinApp::kWindowHeight), 0.01f, 2000.0f);
	scale_ = {1.0f, 1.0f, 1.0f};
	rotate_ = {0.0f, 0.0f, 0.0f};
	translate_ = {0.0f, 0.0f, -100.0f};
	matCamera_ = MakeAffineMatrix(scale_, rotate_, translate_);
	worldTransform3DReticle_.Initialize();
	distance_ = 50.0f;
	model3DReticle_ = Model::CreateFromOBJ("point", true);
	enemyEasyTex_ = TextureManager::Load("easy.png");
	enemyNormalTex_ = TextureManager::Load("normal.png");
	enemyHardTex_ = TextureManager::Load("hard.png");

}

void ScriptEditor::Update() {

	if (isEdit_) {

		ImGui::Begin("Camera and Reticle");
		ImGui::SliderFloat3("camera scale%d", &scale_.x, 1.0f, 10.0f);
		ImGui::DragFloat3("camera rotate", &rotate_.x, 0.01f);
		ImGui::DragFloat3("camera translate", &translate_.x, 0.1f);
		ImGui::DragFloat("reticle distance", &distance_, 0.1f);
		ImGui::End();

		//ImGui開始
		ImGui::Begin("Editor");

		if (ImGui::Button("Add")) {

			EnemyData enemydata;
			enemydata.worldTransform = worldTransform3DReticle_;
			enemydata.type = EASY;
			enemyData_.push_back(enemydata);

		}
		
		
		ImGui::End();

		{

		}

		matCamera_ = MakeAffineMatrix(scale_, rotate_, translate_);
		matView_ = Inverse(matCamera_);
		
		{

			Vector3 offset = {0.0f, 0.0f, 1.0f};

			offset = TransformNormal(offset, matCamera_);
			offset = Multiply(distance_, Normalize(offset));

			worldTransform3DReticle_.translation_ = Add(translate_, offset);
			worldTransform3DReticle_.UpdateMatrix();
		}

	}

}

void ScriptEditor::Draw(ViewProjection viewProjection) {

	for (EnemyData enemy : enemyData_) {

		switch (enemy.type) {

		default:
		case EASY:

			model_->Draw(enemy.worldTransform, viewProjection, enemyEasyTex_);

			break;

		case NORMAL:

			model_->Draw(enemy.worldTransform, viewProjection, enemyNormalTex_);

			break;

		case HARD:

			model_->Draw(enemy.worldTransform, viewProjection, enemyHardTex_);

			break;
		
		}

	}

	model3DReticle_->Draw(worldTransform3DReticle_, viewProjection);

}
