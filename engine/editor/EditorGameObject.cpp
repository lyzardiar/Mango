#pragma once

#include "engine/object/GameObject.h"
#include "engine/editor/IEditor.h"
#include "libs/imgui/imgui.h"

void RE::GameObject::OnGUI() {
	IEditor::DrawClassUI(this);
	IEditor::DrawTransformUI(this);
}