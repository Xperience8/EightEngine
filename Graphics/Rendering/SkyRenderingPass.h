#pragma once

#include "RenderingPass.h"

class SkyRenderingPass : public RenderingPass
{
private:

public:
	SkyRenderingPass() { }
	~SkyRenderingPass() { }

	void Render(Renderer &renderer,
		const std::shared_ptr<GameCamera> &pCamera) override;
};