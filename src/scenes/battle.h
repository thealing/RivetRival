#pragma once

#include "global.h"

#include "scene.h"

#include "button.h"

#include "levels.h"

#include "cars.h"

#include "bot.h"

#include "engine/window.h"

#include "engine/sound.h"

#include "engine/random.h"

void battle_init();

void battle_enter();

void battle_leave();

void battle_update(double delta_time);

void battle_render();
