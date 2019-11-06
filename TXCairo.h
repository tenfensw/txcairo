/*
 TXCairo - image rendering library for TXLin
 Copyright (c) 2019, Tim K/RoverAMD <timprogrammer@rambler.ru>
 
 TXLin's licensing terms are different from other libraries. See
 http://meow.roveramd.com/archives/LICENSE.TXLIN for more info.
 */

#ifndef TXCAIRO_ALREADYIN
#define TXCAIRO_ALREADYIN

#ifdef _WIN32
#error "TXCairo requires macOS or Linux and TXLin. If you use Windows and/or TXLib, you'll have to implement txBlitPNG and txBlitBMP by yourself."
#endif

#include "TXLin.h"
#include <SDL.h>
#include <SDL_render.h>
#include <cairo.h>
#include <cstdlib>

#define TXCAIRO_VERSION 0.21

#ifndef THIS_IS_TXLIN
#error "TXLin 1.76 or 1.74 is required to use TXCairo (though 1.74 must be heavily patched first)."
#endif

inline SDL_Surface* _txLinUnportableSDLSurfaceFromCairo(cairo_surface_t* srf) {
    if (!srf)
        return nullptr;
    int cw = cairo_image_surface_get_width(srf);
    int ch = cairo_image_surface_get_height(srf);
    SDL_Surface* sdlsurf = SDL_CreateRGBSurface(0, cw, ch, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0);
    if (!sdlsurf)
        return nullptr;
    cairo_surface_t* cairosurf = cairo_image_surface_create_for_data((unsigned char*)(sdlsurf->pixels), CAIRO_FORMAT_RGB24, sdlsurf->w, sdlsurf->h, sdlsurf->pitch);
    cairo_t* cairoblitter = cairo_create(cairosurf);
    cairo_set_source_surface(cairoblitter, srf, 0, 0);
    cairo_paint(cairoblitter);
    cairo_destroy(cairoblitter);
    cairo_surface_destroy(cairosurf);
    return sdlsurf;
}


inline bool _txLinUnportableBlitAnySurface(SDL_Surface* actualSurf, HDC dcOut, unsigned x, unsigned y) {
    HDC realOut = dcOut;
    if (!realOut)
        realOut = txDC();
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
    SDL_RenderCopy(realOut, actualText, nullptr, mkRect);
    SDL_free(mkRect);
    SDL_RenderPresent(realOut);
    SDL_DestroyTexture(actualText);
    return true;
}


inline bool txBlitPNG(const char* pngIn, HDC dcOut = txDC(), unsigned x = 0, unsigned y = 0) {
    if (!pngIn)
        return false;
    cairo_surface_t* srf = cairo_image_surface_create_from_png(pngIn);
    SDL_Surface* actualSurf = _txLinUnportableSDLSurfaceFromCairo(srf);
    bool result = _txLinUnportableBlitAnySurface(actualSurf, dcOut, x, y);
    SDL_FreeSurface(actualSurf);
    cairo_surface_destroy(srf);
    return result;
}

inline bool txBlitBMP(const char* bmpIn, HDC dcOut = txDC(), unsigned x = 0, unsigned y = 0) {
    if (!bmpIn)
        return false;
    SDL_Surface* actualSurf = SDL_LoadBMP(bmpIn);
    bool result = _txLinUnportableBlitAnySurface(actualSurf, dcOut, x, y);
    SDL_FreeSurface(actualSurf);
    return result;
}

