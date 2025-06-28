

#include "Buffs/ExtraLifeBuff.hpp"

#include "Player.hpp"


/* ------------ HANDLER ------------ */
void	HandleAddExtraLifeBuff(entt::registry &registry, entt::entity player) {
	if (!registry.all_of<PlayerTag, Lives>(player)) {
		std::cerr << "Error: invalid entity/missing component in HandleAddExtraLifeBuff" << std::endl;
		return ;
	}
	
	int &lives = registry.get<Lives>(player).lives;
	if (lives < 5)
		lives += 1;
}
