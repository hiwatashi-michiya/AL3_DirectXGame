#include "ScriptEditor.h"
#include <ImGuiManager.h>

ScriptEditor::ScriptEditor() {}

ScriptEditor::~ScriptEditor() {}

void ScriptEditor::Initialize() {

	isEdit_ = true;
	input_ = Input::GetInstance();
	model_ = Model::Create();

}

void ScriptEditor::Update() {

	if (isEdit_) {

		if (input_->PushKey(DIK_1)) {
			
		}

		//ImGui開始
		ImGui::Begin("Editor");

		if (ImGui::Button("Add")) {

		}
		
		ImGui::End();

	}

}

void ScriptEditor::Draw(ViewProjection viewProjection) {

	for (EnemyData enemy : enemyData_) {

		model_->Draw(enemy.worldTransform, viewProjection, enemy.textureHandle);

	}

}
