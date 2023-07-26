#include "ScriptEditor.h"
#include <ImGuiManager.h>
#include <WinApp.h>
#include <TextureManager.h>
#include <cassert>
#include <stdio.h>

ScriptEditor::ScriptEditor() {}

ScriptEditor::~ScriptEditor() {
	for (EnemyData* enemy : enemyData_) {
		delete enemy;
	}
	delete sprite_;
}

void ScriptEditor::Initialize(Model* model) {

	assert(model);
	modelObject_ = model;
	isEdit_ = true;
	canMoveCamera_ = false;
	input_ = Input::GetInstance();
	matCameraMain_ = MakeIdentity4x4();
	matView_ = MakeIdentity4x4();
	matProjection_ = MakePerspectiveFovMatrix(
	    0.45f, float(WinApp::kWindowWidth) / float(WinApp::kWindowHeight), 0.01f, 2000.0f);
	scaleMain_ = {1.0f, 1.0f, 1.0f};
	rotateMain_ = {0.15f, -0.15f, 0.0f};
	translateMain_ = {10.0f, 10.0f, -100.0f};
	matCameraMain_ = MakeAffineMatrix(scaleMain_, rotateMain_, translateMain_);
	worldTransform3DReticle_.Initialize();
	distance_ = 50.0f;
	model3DReticle_ = Model::CreateFromOBJ("point", true);
	enemyEasyTex_ = TextureManager::Load("easy.png");
	enemyNormalTex_ = TextureManager::Load("normal.png");
	enemyHardTex_ = TextureManager::Load("hard.png");
	textureSelect_ = TextureManager::Load("select.png");
	sprite_ = Sprite::Create(textureSelect_, {0.0f, 0.0f}, {0.8f, 0.8f, 0.4f, 0.6f}, {0.5f, 0.5f});

}

void ScriptEditor::Update(const ViewProjection& viewProjection) {

	if (isEdit_) {

		if (input_->IsTriggerMouse(0) || input_->IsTriggerMouse(1)) {
			canMoveCamera_ = true;
		}

		ImGui::Begin("MainCamera and Reticle");
		JudgeCanMoveCamera();
		ImGui::SliderFloat3("camera scale", &scaleMain_.x, 1.0f, 10.0f);
		ImGui::DragFloat3("camera rotate", &rotateMain_.x, 0.01f);
		ImGui::DragFloat3("camera translate", &translateMain_.x, 0.1f);
		ImGui::DragFloat("reticle distance", &distance_, 0.1f);
		ImGui::End();

		//ImGui開始
		ImGui::Begin("Editor");

		JudgeCanMoveCamera();

		if (ImGui::Button("Add")) {

			EnemyData* enemydata = new EnemyData();
			enemydata->worldTransform.Initialize();
			enemydata->worldTransform.SetTranslation(worldTransform3DReticle_.translation_);
			enemydata->worldTransform.UpdateMatrix();
			enemydata->type = EASY;
			enemydata->spawnTimer = 60;
			enemyData_.push_back(enemydata);

			dataNumber++;
			dataSelectNumber = dataNumber;

		}

		if (dataNumber != 0) {

			ImGui::InputInt("dataNumber", &dataSelectNumber);
			
			dataSelectNumber = Clamp(dataSelectNumber, 1, dataNumber);

			ImGui::InputText(".csv", filename_, sizeof(filename_));

			std::string length = filename_;

			if (length.size() != 0) {

				if (ImGui::Button("Save")) {

					Save(filename_);

				}

			}

			

		}

		ImGui::End();

		if (dataNumber != 0) {

			ImGui::Begin("enemy data");

			JudgeCanMoveCamera();

			for (int i = 1; EnemyData * enemy : enemyData_) {

				Vector3 positionReticle = Vector3(
				    enemy->worldTransform.matWorld_.m[3][0],
				    enemy->worldTransform.matWorld_.m[3][1],
				    enemy->worldTransform.matWorld_.m[3][2]);

				//ビューポート

				Matrix4x4 matViewport =
				    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

				Matrix4x4 matViewProjectionViewport = Multiply(
				    Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

				positionReticle = Transform(positionReticle, matViewProjectionViewport);

				if (positionReticle.x - 40.0f <= input_->GetMousePosition().x &&
					positionReticle.x + 40.0f >= input_->GetMousePosition().x &&
					positionReticle.y - 40.0f <= input_->GetMousePosition().y &&
					positionReticle.y + 40.0f >= input_->GetMousePosition().y) {
					if (input_->IsTriggerMouse(0)) {
						dataSelectNumber = i;
					}
				}

				if (dataSelectNumber == i) {
					ImGui::Text("enemy %d", i);
					ImGui::DragFloat3("position", &enemy->worldTransform.translation_.x, 0.1f);
					ImGui::Combo("type", &enemy->type, typeString_.data(), int(typeString_.size()));
					ImGui::DragInt("spwanTimer", &enemy->spawnTimer, 0.1f, 0, 6000);
					enemy->spawnTimer = Clamp(enemy->spawnTimer, 0, enemy->spawnTimer);
					sprite_->SetPosition({positionReticle.x, positionReticle.y});
				}

				i++;
			}

			ImGui::End();

		}

		if (canMoveCamera_ && input_->IsPressMouse(0)) {

			//縦横移動
			translateMain_.x = translateMain_.x + -float(input_->GetMouseMove().lX) / 30.0f;
			translateMain_.y = translateMain_.y + float(input_->GetMouseMove().lY) / 30.0f;

		}

		//奥行き移動
		translateMain_.z = translateMain_.z + float(input_->GetMouseMove().lZ) / 30.0f;

		if (canMoveCamera_ && input_->IsPressMouse(1)) {

			//Y軸
			rotateMain_.y = rotateMain_.y + float(input_->GetMouseMove().lX) / 500.0f;

			//X軸,Z軸
			rotateMain_.x = rotateMain_.x + float(input_->GetMouseMove().lY) / 500.0f * fabsf(cosf(rotateMain_.y));
			rotateMain_.z = rotateMain_.z + float(input_->GetMouseMove().lY) / 500.0f * fabsf(sinf(rotateMain_.y));

		}

		matCameraMain_ = MakeAffineMatrix(scaleMain_, rotateMain_, translateMain_);

		matView_ = Inverse(matCameraMain_);
		
		{

			Vector3 offset = {0.0f, 0.0f, 1.0f};

			offset = TransformNormal(offset, matCameraMain_);
			offset = Multiply(distance_, Normalize(offset));

			worldTransform3DReticle_.translation_ = Add(translateMain_, offset);
			worldTransform3DReticle_.UpdateMatrix();
		}

	}

}

void ScriptEditor::Draw(ViewProjection viewProjection) {

	for (EnemyData* enemy : enemyData_) {

		enemy->worldTransform.UpdateMatrix();

		switch (enemy->type) {

		default:
		case EASY:

			modelObject_->Draw(enemy->worldTransform, viewProjection, enemyEasyTex_);

			break;

		case NORMAL:

			modelObject_->Draw(enemy->worldTransform, viewProjection, enemyNormalTex_);

			break;

		case HARD:

			modelObject_->Draw(enemy->worldTransform, viewProjection, enemyHardTex_);

			break;
		
		}

	}

	model3DReticle_->Draw(worldTransform3DReticle_, viewProjection);

}

void ScriptEditor::DrawUI() {

	if (dataNumber != 0) {
		sprite_->Draw();
	}

}

void ScriptEditor::Save(const char* filename) {

	FILE* fp = NULL;

	// ./Resource/Commandsに作成するためのファイルパス
	std::string filepass = "./Resources/Commands/";
	filepass += filename;
	filepass += ".csv";

	fopen_s(&fp, filepass.c_str(), "w+b");

	assert(fp != NULL);

	for (EnemyData* enemy : enemyData_) {

		std::string str;

		//待機時間を文字列に変換
		if (enemy->spawnTimer != 0) {
			str += "WAIT,";
			str += std::to_string(enemy->spawnTimer);
			str += ",,\n";
		}

		//座標を文字列に変換
		str += "POP,";
		str += std::to_string(enemy->worldTransform.translation_.x);
		str += ",";
		str += std::to_string(enemy->worldTransform.translation_.y);
		str += ",";
		str += std::to_string(enemy->worldTransform.translation_.z);
		str += ",";
		//敵の種類(int型)を文字列に変換
		str += std::to_string(enemy->type);
		str += "\n";

		//最後尾に書き込み
		fseek(fp, 0, SEEK_END);
		fputs(str.c_str(), fp);

	}

	fclose(fp);

}

bool ScriptEditor::CheckWithinWindow() {

	// ImGuiのウィンドウ内かどうか判定
	if (ImGui::GetWindowPos().x <= input_->GetMousePosition().x &&
	    input_->GetMousePosition().x <= (ImGui::GetWindowPos().x + ImGui::GetWindowWidth()) &&
	    ImGui::GetWindowPos().y <= input_->GetMousePosition().y &&
	    input_->GetMousePosition().y <= (ImGui::GetWindowPos().y + ImGui::GetWindowHeight())) {
		return true;
	}

	return false;

}

void ScriptEditor::JudgeCanMoveCamera() {

	if (input_->IsTriggerMouse(0) || input_->IsTriggerMouse(1)) {

		//ImGuiウィンドウの範囲内ならカメラ移動できない
		if (CheckWithinWindow()) {
			canMoveCamera_ = false;
		}

	}

}

void ScriptEditor::SetAxisCamera() {

	rotatesSub_[0] = {0.0f, 0.0f, 0.0f};

	rotatesSub_[1] = {0.0f, 3.14f / 2.0f, 0.0f};

	rotatesSub_[2] = {3.14f / 2.0f, 0.0f, 0.0f};

}

void EnemyData::Update() {

	// 敵タイプの文字列格納リスト
	std::vector<const char*> typeString_ = {"EASY", "NORMAL", "HARD"};

	ImGui::Begin("enemy");

	if (ImGui::CollapsingHeader("enemy")) {
		ImGui::Text("enemy");
		ImGui::DragFloat3("position", &worldTransform.translation_.x, 0.1f);
		ImGui::Combo("type", &type, typeString_.data(), int(typeString_.size()));
	}

	ImGui::End();

}

