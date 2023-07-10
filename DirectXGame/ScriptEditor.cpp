#include "ScriptEditor.h"
#include <ImGuiManager.h>

ScriptEditor::ScriptEditor() {}

ScriptEditor::~ScriptEditor() {}

void ScriptEditor::Initialize() {

	isEdit_ = true;

}

void ScriptEditor::Update() {

	if (isEdit_) {

		ImGui::Begin("Editor");
		ImGui::End();

	}

}
