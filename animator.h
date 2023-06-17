#pragma once
#include "components.c"


typedef struct ANIMATOR ANIMATOR;

struct ANIMATOR
{

	int curr;
	int frames;
	int freq;
	void(*transform)(ANIMATOR*);

	TEXTURE* tex;

};

void resize_anim_test(ANIMATOR*);

void animator_init(ANIMATOR* anim, TEXTURE* tex, int frames, int freq)
{
	anim->curr = 0;
	anim->frames = frames;
	anim->freq = freq;
	anim->tex = tex;
	// a->transform = NULL;
	anim->transform = resize_anim_test;
}


void animator_bind(ANIMATOR* anim, void(*fn)(ANIMATOR*))
{
	anim->transform = fn;
}

void animator_trigger(ANIMATOR* anim)
{
	if (anim->curr == anim->frames) return;
	anim->curr++;
	anim->transform(anim);
}

void animator_reset(ANIMATOR* anim)
{
	anim->curr = 0;
}


void resize_anim_test(ANIMATOR* anim)
{
	anim->tex->rect.w += 5;
	anim->tex->rect.h += 5;
}

