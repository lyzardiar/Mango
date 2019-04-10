#include "Object.h"



void RE::Object::Retain() {
	++_ref;
}

void RE::Object::Release() {
	if (_ref > 0) {
		--_ref;

		if (_ref == 0) {
			delete this;
		}
	}
}
