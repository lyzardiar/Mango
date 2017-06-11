#include "GameStudio.h"
#include "imgui/imgui.h"
#include "core/platform/Window.h"

#include "engine/Engine.h"

#include "game/test/test1.h"

RE::GameStudio::GameStudio()
{
	_engine = new Engine();
	ImGui::LoadDock(_engine->L);
}

RE::GameStudio::~GameStudio()
{

}

bool RE::GameStudio::Update(float dt)
{
	auto& imIO = ImGui::GetIO();
	ImGui::RootDock(ImVec2(0, 0), ImVec2(imIO.DisplaySize.x, imIO.DisplaySize.y));

	Test1::Update(dt);

	return true;
}

int RE::GameStudio::Loop() {
	RE::Window window;
	window.RenderHandle = [this](float dt) {
		Update(dt);
	};
	return window.loop() ? 0 : 1;
}
