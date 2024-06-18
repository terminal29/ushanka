#pragma once 
#include "platform/RenderObject.h"

namespace U {
	class OpenGLRenderObject : public RenderObject {

		inline long bind() override {

		}

	public:
		inline OpenGLRenderObject() : RenderObject() {}
	};
}