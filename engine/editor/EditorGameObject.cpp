#pragma once

#include "engine/object/GameObject.h"
#include "engine/editor/IEditor.h"

void RE::GameObject::OnGUI() {
	IEditor::DrawTransformUI(this);
}