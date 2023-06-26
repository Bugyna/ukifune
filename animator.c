#pragma once
#include "components.c"
#include "entity.c"

struct ANIMATOR
{

	ENTITY* e;
	int curr;
	int frame_count;
	TEXTURE* frame;
	int freq;
	void(*transform)(ANIMATOR*);
	// ATLAS* atlas;
	TEXTURE* frames;
};

DEFINE_HASHMAP(ANIMATOR_MAP, ANIMATOR)
DEFINE_LINKED_LIST(ANIMATOR, ANIMATOR)

void resize_anim_test(ANIMATOR*);
void animator_default(ANIMATOR* anim)
{
	// anim->frame
	anim->frame = &anim->frames[anim->curr];
	// anim->frame->rect.x = 400;
	// anim->frame->rect.y = 300;
	// anim->frame->angle += 22;
	anim->e->tex = anim->frame;
	// anim->e->tex->angle += 0.22;
	// SDL_Log("frame: %d %d %d %d", anim->e->tex->rect.x, anim->e->tex->rect.y, anim->e->tex->rect.w, anim->e->tex->rect.h);
}


ANIMATOR* create_anim(ENTITY* e, TEXTURE* frames, int frame_count, int freq, void(*transform)(ANIMATOR*))
{
	ANIMATOR* anim = malloc(sizeof(ANIMATOR));

	anim->e = e;
	anim->curr = 0;
	anim->frame_count = frame_count;
	anim->frames = frames;
	anim->frame = &frames[0];
	// e->tex = anim->frame;
	anim->freq = freq;
	if (transform != NULL)
		anim->transform = transform;
	else
		anim->transform = animator_default;

	return anim;
}

void animator_init(ANIMATOR* anim, TEXTURE* frames, int frame_count, int freq)
{
	anim->curr = 0;
	anim->frame_count = frame_count;
	anim->freq = freq;
	anim->frames = frames;
	anim->frame = &frames[0];
	// anim->atlas = atlas;
	// a->transform = NULL;
	anim->transform = resize_anim_test;
}


void animator_bind(ANIMATOR* anim, void(*fn)(ANIMATOR*))
{
	anim->transform = fn;
}

void animator_reset(ANIMATOR* anim)
{
	anim->curr = 0;
}


void animator_trigger(ANIMATOR* anim)
{
	anim->curr++;
	if (anim->curr == anim->frame_count-2) { anim->curr = 0; }
	anim->transform(anim);
}



void resize_anim_test(ANIMATOR* anim)
{
	// anim->tex->rect.w += 5;
	// anim->tex->rect.h += 5;
}

