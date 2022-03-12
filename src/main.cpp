#include "fileutils.h"
#include "neofontlib/NeoFont.h"
#include "settings.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

void saveCharacter(NeoCharacter &c, std::filesystem::path path) {
    Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    auto surface = SDL_CreateRGBSurface(
        0, c.width(), c.height(), 32, rmask, gmask, bmask, amask);

    if (!surface) {
        throw "could not create surface";
    }

    auto pixels = reinterpret_cast<uint8_t *>(surface->pixels);
    for (size_t y = 0; y < c.height(); ++y) {
        for (size_t x = 0; x < c.width(); ++x) {
            //            auto pixel = &pixels[x + y * surface->pitch];

            auto pixel =
                reinterpret_cast<uint32_t *>(&pixels[x + y * surface->pitch]);
            *pixel = c.getPixel(x, y) ? 0x000000ff : 0x000000ff;
            //            *pixel = 1;
            //            *pixel = 0x000000ff;
            std::cout << (c.getPixel(x, y) ? '*' : ' ');
        }
        std::cout << "\n";
    }

    IMG_SavePNG(surface, path.string().c_str());

    SDL_FreeSurface(surface);
}

int main(int argc, char *argv[]) {
    IMG_Init(IMG_INIT_PNG);

    const auto settings = Settings{argc, argv};

    auto content = loadFile(settings.in);

    auto font = NeoFont{};
    font.decodeApplet(content);

    auto &c = font.character(54);

    saveCharacter(c, "test-file.png");

    return 0;
}
