#pragma once

#include "entity.hpp"

namespace base {
	class bLightComponent : bComponent {
	public:
		// The light component requires a transform component.
		bool hasRequirements() override;

	private:
		glm::vec3 mColor;

		float mConstant;
		float mLinear;
		float mQuadratic;
	};
}