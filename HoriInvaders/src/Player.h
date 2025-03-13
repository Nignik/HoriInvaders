#pragma once

#include <filesystem>

#include <Core/Ecs.h>
#include <Entity.h>
#include <Core/Transform.h>
#include <Core/Sprite.h>
#include <Core/VelocityComponent.h>
#include <Core/Controller.h>
#include <Core/Collider.h>
#include <Core/ResourceManager.h>
#include <Core/PrimitivesGeneration.h>
#include <Core/WireframeComponent.h>
#include <Core/Renderer.h>
#include <yaml-cpp/yaml.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "HealthComponent.h"

namespace fs = std::filesystem;

struct PlayerComponent
{

};

struct Player
{
public:
	Player(YAML::Node playerInfo)
		: entity(Hori::Ecs::GetInstance().CreateEntity())
	{
		auto& world = Hori::Ecs::GetInstance();
		auto screenDim = Hori::Renderer::GetInstance().GetWindowSize();

		auto position = glm::vec2{ playerInfo["spawn"][0].as<float>(), playerInfo["spawn"][1].as<float>() };
		float rotation = 0.0f;
		Hori::Transform transform = {
			.position = position,
			.rotation = rotation,
			.scale = {playerInfo["size"].as<float>(), playerInfo["size"].as<float>()}
		};

		fs::path shaderPath = playerInfo["shader"].as<std::string>();
		fs::path texturePath = playerInfo["sprite"].as<std::string>();

		auto& resourceMng = Hori::ResourceManager::GetInstance();

		auto textureHandle = resourceMng.Load<Hori::Texture2D>(texturePath);
		auto shaderHandle = resourceMng.Load<Hori::Shader>(shaderPath);

		auto texture = *resourceMng.Get(textureHandle);
		auto shader = *resourceMng.Get(shaderHandle);


		float speed = playerInfo["speed"].as<float>();
		glm::vec2 direction = glm::rotate(glm::vec2(1.0f, 0.0), glm::radians(rotation - 90.f));;
		Hori::VelocityComponent velocity(direction, speed);

		auto health = HealthComponent(playerInfo["health"].as<int>());
		auto collider = Hori::SphereCollider(transform);

		auto vertices = generateCircleVertices(0.5f, 10);
		auto wireframe = Hori::WireframeComponent(vertices, glm::vec3(0.0f, 1.0f, 0.0f));

		world.AddComponents(entity, std::move(transform), std::move(shader), std::move(texture), std::move(velocity), std::move(collider), PlayerComponent(), Hori::Sprite(), Hori::Controller(), std::move(wireframe));
	}


	Hori::Entity entity{};
};