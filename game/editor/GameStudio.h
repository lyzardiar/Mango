#pragma once

namespace RE {
	class GameStudio {
	public:
		GameStudio();
		~GameStudio();

	public:
		bool Update(float dt);
		int Loop();
	};
}