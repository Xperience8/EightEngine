#pragma once

const UINT RenderingTypeCount = 7;
enum class RenderingType { Actor, Terrain, Sky, Transparent, TransparentActor, UI, Text, NoRendering };

class Renderer;

class IRenderableEntity
{
public:
	IRenderableEntity() { }
	virtual ~IRenderableEntity() { }

	virtual void VRender(Renderer &renderer) = 0;
	virtual void VRenderDebug(Renderer &renderer) = 0;

	virtual RenderingType VGetRenderingType() const = 0;
};