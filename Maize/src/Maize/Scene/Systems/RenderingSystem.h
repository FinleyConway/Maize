#pragma once

#include <entt/entt.hpp>

#include "Maize/Scene/Components.h"
#include "Maize/Renderer/Renderer.h"

namespace Maize {

	class RenderingSystem
	{
	public:
		static void OnRender(entt::registry& reg);

		inline static bool drawDebug = false;

	private:
		struct Drawable
		{
			const sf::Drawable* drawable = nullptr;
			int32_t orderInLayer = 0;
		};

		static sf::FloatRect GetViewSpace(const sf::View& camera);

		static void GetSprites(entt::registry& reg, const sf::View& camera, std::unordered_map<std::string, std::vector<Drawable>>& drawableOrder);

		static void RenderDrawables(std::unordered_map<std::string, std::vector<Drawable>>& drawableOrder);
		static void RenderDebugColliders(entt::registry& reg, const sf::View& camera);

		static sf::Vector2f Rotate(sf::Vector2f point, float degrees)
		{
			sf::Transform transform;
			transform.rotate(degrees);

			return transform.transformPoint(point);
		}
	};

} // Maize
