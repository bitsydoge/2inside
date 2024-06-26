#include <stdio.h>
#include <SDL_image.h>
#include "hdr/texture.h"

Texture* TextureCreate(SDL_Renderer* renderer,  const char* spriteName, int r, int g, int b, int a)
{
	// Create Texture from Image (with RGBA to make Transparancy)
	Texture* tex;
	SDL_Surface* surf;
	surf = IMG_Load(spriteName);

	if(surf == NULL)
	{
		printf( "Erreur (Fonction TextureCreate) : impossible de charger l'image \"%s\".\n", spriteName);
		surf = IMG_Load("res/soul.bmp");
		if(surf == NULL)
		{
			printf("ATTENTION. Impossible de charger l'image \"%s\" : %s \n", "res/soul.bmp", SDL_GetError());
			return NULL;
		}
	}

	else if(r >= 0 && g >= 0 && b >= 0)
	{
		SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, r, g, b));
		if(a < 255 && a >= 0)
			SDL_SetSurfaceAlphaMod(surf, a);
	}

	tex = (Texture*) malloc(sizeof(Texture));

	if(tex != NULL)
	{
		tex->handle = SDL_CreateTextureFromSurface(renderer, surf);
		if(tex->handle == NULL)
			printf( "Erreur (fonction TextureCreate): impossible de créer la texture à partir de la surface (%s)\n", SDL_GetError());
		tex->w = surf->w;
		tex->h = surf->h;
	}
	SDL_FreeSurface(surf);
	return tex;
}

void TextureFree(Texture* tex)
{
	SDL_DestroyTexture(tex->handle);
	free(tex);
}

Texture* TextureCopy(SDL_Renderer* renderer, Texture* tex)
{
	Texture* texDst;
	texDst = (Texture*) malloc(sizeof(Texture));
	texDst->w = tex->w;
	texDst->h = tex->h;

	texDst->handle = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, texDst->w, texDst->h);

	SDL_SetTextureBlendMode(texDst->handle, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(renderer, texDst->handle);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, tex->handle, NULL, NULL);
	SDL_SetRenderTarget(renderer, NULL);

	return texDst;
}

int TextureRender(SDL_Renderer* renderer, Texture* tex, SDL_Rect* source, SDL_Rect* destination)
{
	return SDL_RenderCopy( renderer, tex->handle, source, destination );
}

int TextureRenderEx(SDL_Renderer* renderer, Texture* tex, SDL_Rect* source, SDL_Rect* destination, double angle)
{
	return SDL_RenderCopyEx(renderer, tex->handle, source, destination, angle, NULL, SDL_FLIP_NONE);
}
