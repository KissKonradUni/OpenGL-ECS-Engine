#include "bLightComponent.hpp"

namespace base {
    bool bLightComponent::hasRequirements()
    {
        return (
            this->mEntity->hasComponent<bTransformComponent>()
        );
    }
}