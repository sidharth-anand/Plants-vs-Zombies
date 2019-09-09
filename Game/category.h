#pragma once

namespace Category
{
	enum Type
	{
		None			    = 0,
		SceneLayer			= 1 << 0,
		Plant				= 1 << 1,
		Zombie				= 1 << 2,
		Sun					= 1 << 3,
		Projectile			= 1 << 4,
		Sounds				= 1 << 5,
	};
}