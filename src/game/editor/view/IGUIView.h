#pragma once

class IGUIView {
public:
	virtual void OnGUI() = 0;

	void doMenuItem(const char* label, bool enabled);

	bool drawDot(struct ImVec2 pos, float size);

	bool isOpen = true;
};