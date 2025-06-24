

#include "Utils.hpp"

CollisionReturn	BreakoutCollisionCheck(entt::registry &registry, Rectangle A, entt::entity ent) {
	auto view = registry.view<Dimensions>();

	for (auto &entity : view) {
		Rectangle rec = registry.get<Dimensions>(entity).dimensions;
		if (entity == ent)
			continue;

		if (A.x < rec.x + rec.width && A.x + A.width > rec.x && 
			A.y < rec.y + rec.height && A.y + A.height > rec.y) {
			
			float overlapX = std::min(A.x + A.width, rec.x + rec.width) - std::max(A.x, rec.x);
			float overlapY = std::min(A.y + A.height, rec.y + rec.height) - std::max(A.y, rec.y);
			if (overlapX < overlapY)
				return CollisionReturn{true, 0};
			else
				return CollisionReturn{0, true};
		}
	}

	return {0};
}
