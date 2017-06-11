#pragma once

namespace RE {

	class Engine;

	class GameStudio {
	public:
		GameStudio();
		~GameStudio();

	public:
		bool Update(float dt);
		int Loop();

	protected:
		Engine* _engine = nullptr;
	};
}