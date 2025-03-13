#pragma once

#include <filesystem>

#include <Core/Ecs.h>
#include <Entity.h>
#include <Core/Collider.h>
#include <Core/ResourceManager.h>
#include <Core/PrimitivesGeneration.h>
#include <Core/Components.h>
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
		auto& resourceMng = Hori::ResourceManager::GetInstance();

		auto screenDim = Hori::Renderer::GetInstance().GetWindowSize();

		auto position = glm::vec2{ playerInfo["spawn"][0].as<float>(), playerInfo["spawn"][1].as<float>() };
		float rotation = 0.0f;
		Hori::TransformComponent transform = {
			.position = position,
			.rotation = rotation,
			.scale = {playerInfo["size"].as<float>(), playerInfo["size"].as<float>()}
		};

		fs::path shaderPath = playerInfo["shader"].as<std::string>();
		fs::path texturePath = playerInfo["sprite"].as<std::string>();
		
		auto spriteHandle = resourceMng.Load<Hori::SpriteComponent>(texturePath);
		auto shaderHandle = resourceMng.Load<Hori::ShaderComponent>(shaderPath);

		auto sprite = *resourceMng.Get(spriteHandle);
		auto shader = *resourceMng.Get(shaderHandle);

		float speed = playerInfo["speed"].as<float>();
		glm::vec2 direction = glm::rotate(glm::vec2(1.0f, 0.0), glm::radians(rotation - 90.f));;
		Hori::VelocityComponent velocity(direction, speed);

		auto health = HealthComponent(playerInfo["health"].as<int>());
		auto collider = Hori::SphereCollider(transform);

		auto vertices = generateCircleVertices(0.5f, 10);
		auto wireframe = Hori::WireframeComponent(vertices, glm::vec3(0.0f, 1.0f, 0.0f));

		world.AddComponents(entity, std::move(transform), std::move(shader), std::move(sprite), std::move(velocity), std::move(collider), PlayerComponent(), Hori::SpriteComponent(), Hori::ControllerComponent(), std::move(wireframe));
	}


	Hori::Entity entity{};
};