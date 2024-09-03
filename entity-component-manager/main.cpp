#include "pch.hpp"
#include "entity_component_manager.hpp"

EntityComponentManager ecm;

struct Position
{
	float x, y;
	void set(float a, float b) { x = a; y = b; }
};

struct Velocity
{
	float x, y;
	void set(float a, float b) { x = a; y = b; }
};

int main()
{
	ecm.registerComponent<Position>();
	ecm.registerComponent<Velocity>();

	Entity player = ecm.createEntity();
	ecm.createComponent<Position>(player).set(10, 10);
	ecm.createComponent<Velocity>(player).set(1, -1);

	int iterations = 100;
	float duration = 0;
	float dt;

	while (iterations--)
	{	
		std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();
		// start of loop

		for (Entity entity : ecm.getAvailableEntities())
		{
			if (ecm.hasComponent<Position>(entity) && ecm.hasComponent<Velocity>(entity))
			{
				ecm.getComponent<Position>(entity).x += ecm.getComponent<Velocity>(entity).x;
				ecm.getComponent<Position>(entity).y += ecm.getComponent<Velocity>(entity).y;
			}
		}

		// end of loop
		std::chrono::steady_clock::time_point stopTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsedTime = stopTime - startTime;
		dt = elapsedTime.count();
		duration += dt;
	}

	std::cout << ecm.getComponent<Position>(player).x << std::endl;
	std::cout << ecm.getComponent<Position>(player).y << std::endl;

	return 0;
}