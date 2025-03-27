#pragma once

#include <Core/Ecs.h>
#include <Core/EventManager.h>
#include <Core/Renderer.h>
#include <Core/PrimitivesGeneration.h>
#include <Core/Components.h>
#include <Core/ResourceManager.h>

#include <yaml-cpp/yaml.h>

#include "ComponentParser.h"

namespace entities
{
	inline auto& world = Hori::Ecs::GetInstance();
	inline auto& resourceMng = Hori::ResourceManager::GetInstance();

	inline void addTranform(Hori::Entity e, const YAML::Node& data)
	{
		auto transform = parse<Hori::Transform>(data["transform"]);
		world.AddComponents(e, std::move(transform));
	}

	inline void addHealth(Hori::Entity e, const YAML::Node& data)
	{
		Health health{ data["health"].as<int>() };
		world.AddComponents(e, std::move(health));
	}
	
	inline void addDamage(Hori::Entity e, const YAML::Node& data)
	{
		Damage damage{ data["damage"].as<float>() };
		world.AddComponents(e, std::move(damage));
	}

	inline void addWireframe(Hori::Entity e)
	{
		float radius = world.GetComponent<Hori::SphereCollider>(e)->radius;
		std::vector<float> vertices = generateCircleVertices(0.3f, 10);
		auto wireframe = Hori::Wireframe(vertices, glm::vec3(0.0f, 1.0f, 0.0f));
		world.AddComponents(e, std::move(wireframe));
	}

	inline void makeDynamic(Hori::Entity e, const YAML::Node& data)
	{
		auto velocity = parse<Hori::Velocity>(data["velocity"]);
		world.AddComponents(e, std::move(velocity));
	}

	inline void makeDrawable(Hori::Entity e, const YAML::Node& data)
	{
		std::filesystem::path spritePath = data["sprite"].as<std::string>();
		std::filesystem::path shaderPath = data["shader"].as<std::string>();
		
		auto spritePtr = resourceMng.Get<Hori::Sprite>(spritePath);
		auto shaderPtr = resourceMng.Get<Hori::Shader>(shaderPath);

		if (!spritePtr || !shaderPtr)
		{
			std::cout << "Failed to load resources\n";
			return;
		}

		Hori::Sprite sprite = *spritePtr;
		Hori::Shader shader = *shaderPtr;
		
		world.AddComponents(e, std::move(sprite), std::move(shader));
	}

	inline void makeCollidable(Hori::Entity e, const YAML::Node& data)
	{
		bool isTrigger = data["collider"]["is_trigger"].as<bool>();
		Hori::SphereCollider collider{*world.GetComponent<Hori::Transform>(e), 0.3f, isTrigger};
		world.AddComponents(e, std::move(collider));
	}

	inline void makeShooter(Hori::Entity e, const YAML::Node& data)
	{
		std::set<Hori::Entity> prototypes;
		for (int i = 0; i < data["projectiles"].size(); i++)
		{
			std::filesystem::path path = data["projectiles"][i].as<std::string>();
			auto projectile = resourceMng.Get<YAML::Node>(path);
			if (!projectile)
				continue;

			Hori::Entity prototype = world.CreatePrototypeEntity();
			addTranform(prototype, *projectile);
			addDamage(prototype, *projectile);
			makeDrawable(prototype, *projectile);
			makeDynamic(prototype, *projectile);
			makeCollidable(prototype, *projectile);
			world.AddComponents(prototype, ActorComponent(), Health{1000});
			
			addWireframe(prototype);
			prototypes.insert(prototype);
		}

		Cooldown cooldowns{};
		for (auto& prototype : prototypes)
		{
			cooldowns.cooldowns.emplace_back(prototype, CooldownType::ProjectileSpawn, 0.5f);
		}
		
		world.AddComponents(e, std::move(cooldowns), SpawnerComponent{});
	}

	inline void makeCreature(Hori::Entity e, const YAML::Node& data)
	{
		addTranform(e, data);
		addHealth(e, data);
		makeDrawable(e, data);
		makeDynamic(e, data);
		makeCollidable(e, data);
		world.AddComponents(e, ActorComponent());
	}
	
	// Requires the entity to have a transform
	// Collider passed is used for additional stuff not related to entity like: isTrigger
	inline Hori::Entity spawnPlayer(const YAML::Node& data)
	{
		Hori::Entity player = world.CreateEntity();
		makeCreature(player, data);
		//makeShooter(player, data);
		addWireframe(player);
		world.AddComponents(player, PlayerComponent(), Hori::Controller());

		return player;
	}

	inline Hori::Entity spawnEnemy(const YAML::Node& data)
	{
		Hori::Entity enemy = world.CreateEntity();
		makeCreature(enemy, data);
		makeShooter(enemy, data);
		addWireframe(enemy);
		world.AddComponents(enemy, EnemyComponent());

		return enemy;
	}
};
