#include "Image.h"

#include "engine/object/GameObject.h"


RE::Image::Image() {

}

void RE::Image::Awake() {
	renderer = gameObject->AddComponent<TriangleRenderer>();
}
