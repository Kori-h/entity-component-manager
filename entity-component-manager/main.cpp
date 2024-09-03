#include "pch.hpp"
#include "entity_component_manager.hpp"

EntityComponentManager ecm;

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
			}
		}
	}
}

int main()
{
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_real_distribution<float> positionRand(-100.0f, 100.0f);
	std::uniform_real_distribution<float> scaleRand(1.0f, 2.0f);
	std::uniform_real_distribution<float> velocityRand(-5.0f, 5.0f);
	
	ecm.registerComponent<Transform>();
	ecm.registerComponent<Velocity>();
	ecm.registerComponent<AABB>();

	ecm.registerFunction<Transform, Velocity>(CalculateMovement);
	ecm.registerFunction<AABB, Transform>(CalculateAABB);
	ecm.registerSystem(CollisionSystem);

	for (size_t x = 0; x < MAX_ENTITY_COUNT; ++x)
	{
		Entity player = ecm.createEntity();

		Transform& transform = ecm.createComponent<Transform>(player);
		transform.position = { positionRand(dre), positionRand(dre) };
		transform.scale = { scaleRand(dre), scaleRand(dre) };

		Velocity& velocity = ecm.createComponent<Velocity>(player);
		velocity.vector = { velocityRand(dre), velocityRand(dre) };

		ecm.createComponent<AABB>(player);
	}

	int total_iterations = 100;
	int iterations = total_iterations;
	float duration = 0;
	float dt = 0;

	while (iterations--)
	{	
		std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();
		// start of loop

		ecm.update();

		// end of loop
		std::chrono::steady_clock::time_point stopTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsedTime = stopTime - startTime;
		dt = elapsedTime.count();
		duration += dt;
	}

	std::cout << "Average Time: " << duration / total_iterations * 1000.0f << "ms" << std::endl;
	std::cout << "Duration: " << duration << "s" << std::endl;
	std::cout << "FPS: " << total_iterations / duration << " fps" << std::endl;
	std::cout << "Collisions: " << count << std::endl;

	return 0;
}