#include "pch.hpp"
#include "entity_component_manager.hpp"

EntityComponentManager entityComponentManager;

struct Vector2D
{
	float x, y;
};

struct Transform
{
	Vector2D position;
	Vector2D scale;
	float rotation;
};

struct Velocity
{
	Vector2D vector;
};

struct AABB
{
	Vector2D min;
	Vector2D max;
};

void CreatePlayer()
{
	std::uniform_real_distribution<float> positionRand(-100.0f, 100.0f);
	std::uniform_real_distribution<float> scaleRand(1.0f, 2.0f);
	std::uniform_real_distribution<float> velocityRand(-5.0f, 5.0f);

	Entity player = entityComponentManager.createEntity();

	Transform& transform = entityComponentManager.createComponent<Transform>(player);
	transform.position = { positionRand(dre), positionRand(dre) };
	transform.scale = { scaleRand(dre), scaleRand(dre) };

	Velocity& velocity = entityComponentManager.createComponent<Velocity>(player);
	velocity.vector = { velocityRand(dre), velocityRand(dre) };

	entityComponentManager.createComponent<AABB>(player);
}

void CalculateMovement(Transform& tf, Velocity& vel)
{
	tf.position.x += vel.vector.x;
	tf.position.y += vel.vector.y;
}

void CalculateAABB(AABB& aabb, Transform& tf)
{
	aabb.min = { tf.position.x - tf.scale.x * 0.5f, tf.position.y - tf.scale.y * 0.5f };
	aabb.max = { tf.position.x + tf.scale.x * 0.5f, tf.position.y + tf.scale.y * 0.5f };
}

static size_t count = 0;

void CollisionSystem(EntityComponentManager& ecm)
{
	ComponentPool<AABB>& aabb_pool = ecm.getComponentPool<AABB>();
	std::vector<size_t>& aabb_indices = aabb_pool.getActiveComponents();

	for (size_t x = 0; x < aabb_indices.size(); ++x)
	{
		AABB& a = aabb_pool.getComponent(aabb_indices[x]);

		for (size_t y = x + 1; y < aabb_indices.size(); ++y)
		{
			AABB& b = aabb_pool.getComponent(aabb_indices[y]);

			if ((a.min.x <= b.max.x && a.max.x >= b.min.x &&
				a.min.y <= b.max.y && a.max.y >= b.min.y))
			{
				++count;
				y = std::numeric_limits<size_t>().max() - 1;
			}
		}
	}
}

int main()
{
	try
	{
		entityComponentManager.registerComponent<Transform>();
		entityComponentManager.registerComponent<Velocity>();
		entityComponentManager.registerComponent<AABB>();

		entityComponentManager.registerFunction<Transform, Velocity>(CalculateMovement);
		entityComponentManager.registerFunction<AABB, Transform>(CalculateAABB);

		entityComponentManager.registerSystem(CollisionSystem);

		for (size_t x = 0; x < MAX_ENTITY_COUNT; ++x)
		{
			CreatePlayer();
		}

		float dt = 0;
		size_t frameCount = 0;

		while (true)
		{
			auto start_time = std::chrono::high_resolution_clock::now();
			entityComponentManager.update();
			auto end_time = std::chrono::high_resolution_clock::now();

			std::chrono::duration<float> time = end_time - start_time;
			++frameCount;

			if ((frameCount % 60) == 0)
			{
				dt = time.count();
				std::cout << "[ Frame: " << frameCount << " | FPS: " << (1.0f / dt) << " ]" << std::setprecision(2) << std::fixed << std::endl;
			}
		}
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}