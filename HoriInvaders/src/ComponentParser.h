#pragma once

#include "Components.h"
#include <Core/Components.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

template <typename T>
concept CanBeParsed = requires {
	Hori::AnyOf<
		T, 
		Hori::TransformComponent, Hori::VelocityComponent,
		CooldownComponent, DamageComponent, HealthComponent, SpawnerComponent
	>;
};

template <typename T>
concept IsDefaultContructible = requires {
	T{};
};

template<typename ComponentType>
class ComponentParser;

template<typename T>
requires IsDefaultContructible<T> && CanBeParsed<T>
inline T parse(const YAML::Node& node)
{
	try
	{
		return ComponentParser<T>::Parse(node);
	}
	catch (const YAML::BadSubscript& e)
	{
		std::cout << "Error: " << e.what() << " Node: " << node << '\n';
	}
	catch (const YAML::BadConversion& e)
	{
		std::cout << "Error: " << e.what() << " Node: " << node << '\n';
	}
	catch (const YAML::InvalidNode& e)
	{
		std::cout << "Error: " << e.what() << " Node: " << node << '\n';
	}


	return T{};
}

template<>
struct ComponentParser<Hori::TransformComponent>
{
	static Hori::TransformComponent Parse(const YAML::Node& node)
	{
		Hori::TransformComponent t{
			.position = {node["position"]["x"].as<float>(), node["position"]["y"].as<float>()},
			.rotation = node["rotation"].as<float>(),
			.scale = {node["scale"]["x"].as<float>(), node["scale"]["y"].as<float>()}
		};

		return t;
	}
};

/*template<>
struct ComponentParser<CooldownComponent>
{
	static CooldownComponent Parse(const YAML::Node& node)
	{
		std::set<Hori::Entity> prototypes;
		auto projectiles = node["projectiles"];
		// TODO: change to range loop
		for (const auto& projectile : projectiles)
		{
			auto prototype = createProjectilePrototype(projectile);
			prototypes.insert(prototype);
		}

	}
};*/

template<>
struct ComponentParser<Hori::VelocityComponent>
{
	static Hori::VelocityComponent Parse(const YAML::Node& node)
	{
		Hori::VelocityComponent v {
			.dir = glm::rotate(glm::vec2(1.0f, 0.0), glm::radians(node["direction"].as<float>() - 90.f)),
			.speed = node["speed"].as<float>()
		};

		return v;
	}
};


