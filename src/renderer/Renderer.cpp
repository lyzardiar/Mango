#include "Renderer.h"
#include "RenderState.h"



RE::Renderer* RE::Renderer::CurrentRenderer = nullptr;

RE::Renderer::Renderer()
	: rs(*(new RenderState()))
{
	CurrentRenderer = this;
}
