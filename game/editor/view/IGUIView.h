#pragma once

class IGUIView {
public:
	virtual void OnGUI() = 0;

	void doMenuItem(const char* label, bool enabled);

	bool isOpen = true;
};