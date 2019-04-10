#pragma once

namespace RE {
	class Object {
	public:
		virtual void Retain();
		virtual void Release();

	protected:
		int _ref = 1;
	};
}