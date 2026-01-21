#include "bouncy.h"

bool init_scene(t_scene* scene) {
	scene->balls = NULL;

	scene->acceleration.x = SCENE_ACCELERATION_X;
	scene->acceleration.y = SCENE_ACCELERATION_Y;
	scene->rebounce = SCENE_REBOUCE;
	scene->friction = SCENE_FRICTION;

	scene->balls_count = BALLS_COUNT;
	scene->balls_iteration = BALLS_ITERATION;

	init_balls(scene);
	if (!scene->balls) {
		fprintf(stderr, "Could not initialize balls\n");
		return false;
	}
	return true;
}

void free_scene(t_scene* scene) {
	if (scene->balls)
		free_balls(scene->balls);
}

void draw(t_scene* scene) {
	draw_balls(scene);
}

void update(t_scene* scene) {
	update_balls(scene);
}
