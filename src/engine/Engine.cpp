#include "Engine.h"
#include "base/Array.h"
#include "math/Math.h"
#include "object/GameObject.h"
#include "object/Image.h"
#include "platform/Platform.h"
#include "component/Camera.h"
#include "renderer/FrameBuffer.h"
#include "renderer/Renderer.h"
#include "renderer/PipeLine.h"
#include "renderer/Texture2D.h"
#include "system/InputSystem.h"
#include "system/GLProgramSystem.h"
#include "system/Texture2DSystem.h"
#include <algorithm>

RE::Engine RE::Engine::instance;

RE::Engine::Engine()
	: L (luaL_newstate())
	, Lua(L)
	, camera(*((new GameObject("Root"))->AddComponent<Camera>()))
	, input(*(new InputSystem()))
	, renderer(*(new Renderer()))
{
}

RE::Engine::~Engine() {

}

bool RE::Engine::Init() {
	if (_isInited) return true;
	root = camera.gameObject;
	
	glEnable(GL_BLEND);

	_fbo = new FrameBuffer();

	GLProgramSystem::instance.InitDefault();
	Texture2DSystem::instance.InitDefault();

	Lua.openlibs();
	initLuaEnginie();

	int count = 7;

	root->children.Resize(count);
	for (int i = 0; i < count; ++i) {
		auto img = new Image("image");
		img->transform.x = float(rand() % 500);
		img->transform.y = float(rand() % 500);
		root->AddChild(img);
	}

	_isInited = true;
	return true;
}

void RE::Engine::Update(float dt) {
	if (!Lua["Engine"]["Update"].isNilref()) {
		Lua["Engine"]["Update"](dt);
	}

	root->TransferUpdate(dt);
}

void RE::Engine::Render() {
	root->transform.w = (float)camera.size.width;
	root->transform.h = (float)camera.size.height;
	root->transform.x = -root->transform.w / 2.0f;
	root->transform.y = -root->transform.h / 2.0f;
	root->transform.ax = 0;
	root->transform.ay = 0;

	Vec2::SceneSize.width = (float)camera.size.width;
	Vec2::SceneSize.height = (float)camera.size.height;
	Vec2::HalfSceneSize = Vec2::SceneSize / 2.0f;

	drawCalls = 0;
	verticeCount = 0;
	
	_fbo->Begin({ 0, camera.size.width, camera.size.height, 0 });

	root->Render(Affine::Identity);

	PipeLine::instance.Commit();

	_fbo->End();
}

bool RE::Engine::Loop(float dt)
{
	Init();
	time.Update();

	static double elapseUpdate = 0;
	static double elapseRender = 0;
	static int frame = 0;

	Time up;
	Update(dt);
	elapseUpdate += up.Elapse();

	up.ResetElapse();
	Render();
	elapseRender += up.Elapse();

	frame++;

	if (frame % 10 == 0) {
		fps = float(1.0f / (elapseUpdate / frame + elapseRender / frame));
		if (fps > 60) fps = 60;
	}

	time.ResetElapse();
	return true;
}

void RE::Engine::SetFPS(int fps) {
	_fps = fps;
	_interval = 1.0f / _fps;
}

GLuint RE::Engine::GetTextureHandle() {
	return _fbo->GetTextureHandle();
}

void RE::Engine::SelectObjects(class GameObject** objs, int count) {
	selectedObjs.Clear();
	for (int idx = 0; objs != nullptr && idx < count; ++idx) {
		selectedObjs.Push(objs[idx]);
	}
}

void RE::Engine::initLuaEnginie() {
	RE::LuaWrapper::registerLua();

	if (Lua["Engine"].isNilref())
		Lua["Engine"] = kaguya::NewTable();

	Lua.dofile("scripts/Engine.lua");
}
