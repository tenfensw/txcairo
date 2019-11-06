/*
 TXCairo - image rendering library for TXLin
 Copyright (c) 2019, Tim K/RoverAMD <timprogrammer@rambler.ru>
 
 TXLin's licensing terms are different from other libraries. See
 http://meow.roveramd.com/archives/LICENSE.TXLIN for more info.
 */

#ifndef TXCAIRO_ALREADYIN
#define TXCAIRO_ALREADYIN

#include "TXLin.h"
#include <SDL.h>
#include <cairo.h>
#include <cstdlib>

#define TXCAIRO_VERSION 0.21

SDL_Surface* _txLinUnportableCairoSurfaceFromPNG(const char* pngPath) {
    if (!pngPath)
        return nullptr;
    cairo_surface_t* srf = cairo_image_surface_create_from_png(pngPath);
    if (!srf)
        return nullptr;
    int cw = cairo_image_surface_get_width(srf);
    int ch = cairo_image_surface_get_height(srf);
    SDL_Surface* sdlsurf = SDL_CreateRGBSurface(0, cw, ch, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0);
    if (!sdlsurf) {
        cairo_surface_destroy(srf);
        return nullptr;
    }
    cairo_surface_t* cairosurf = cairo_image_surface_create_for_data((unsigned char*)(sdlsurf->pixels), CAIRO_FORMAT_RGB24, sdlsurf->w, sdlsurf->h, sdlsurf->pitch);
    cairo_t* cairoblitter = cairo_create(cairosurf);
    cairo_set_source_surface(cairoblitter, srf, 0, 0);
    cairo_paint(cairoblitter);
    cairo_destroy(cairoblitter);
    cairo_surface_destroy(cairosurf);
    cairo_surface_destroy(srf);
    return sdlsurf;
}

bool txBlitPNG(const char* pngIn, HDC dcOut = txDC(), unsigned x = 0, unsigned y = 0) {
    HDC realOut = dcOut;
    if (!pngIn)
        return false;
    else if (!realOut)
        realOut = txDC();
    SDL_Surface* actualSurf = _txLinUnportableCairoSurfaceFromPNG(pngIn);
    if (!actualSurf)
        return false;
    SDL_Texture* actualText = SDL_CreateTextureFromSurface(realOut, actualSurf);
    if (!actualText)
        return false;
    SDL_Rect* mkRect = (SDL_Rect*)(malloc(sizeof(SDL_Rect)));
    mkRect->x = x;
    mkRect->y = y;
    mkRect->w = actualSurf->w;
    mkRect->h = actualSurf->h;
    SDL_FreeSurface(actualSurf);
    SDL_RenderCopy(realOut, actualText, nullptr, mkRect);
    SDL_free(mkRect);
    SDL_RenderPresent(realOut);
    SDL_DestroyTexture(actualText);
    return true;
}

#endif
