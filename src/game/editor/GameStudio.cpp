#include "GameStudio.h"
#include "imgui/imgui.h"
#include "core/platform/Window.h"
#include "core/base/File.h"
#include "engine/Engine.h"

#include "game/test/test1.h"

RE::GameStudio::GameStudio()
	: _engine (Engine::instance)
{
	_engine.Lua.dofile("studioGUIConfigs.lua");

	ImGui::LoadDock(_engine.L);
}

RE::GameStudio::~GameStudio() {
	RE::FileStream fs("studioGUIConfigs.lua", "wb+");
	ImGui::SaveDock(fs);
}

bool RE::GameStudio::Update(float dt) {
	bool isUpdate = _engine.Loop(dt);

	auto& imIO = ImGui::GetIO();
	ImGui::RootDock(ImVec2(0, 0), ImVec2(imIO.DisplaySize.x, imIO.DisplaySize.y));

	Test1::Update(dt);

	return isUpdate;
}

int RE::GameStudio::Loop() {
	RE::Window window;
	window.RenderHandle = [this](float dt) {
		return Update(dt);
	};
	return window.loop() ? 0 : 1;
}
