/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <Latchezar Tzvetkoff> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Latchezar Tzvetkoff
 * ----------------------------------------------------------------------------
 */

/* asciifier.cpp */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define IMAGEMAGICK_VERSION 0x71

#include <iostream>
#include <iomanip>
#include <string.h>
#include <math.h>

#include "asciifier.h"

/* max quantum value */
#define MAGICK_QUANTUM_MAX ((1 << MAGICKCORE_QUANTUM_DEPTH) - 1)

static unsigned int TERM_COLORS_HEX_256[256] = {
    0x000000, 0x800000, 0x008000, 0x808000, 0x000080, 0x800080, 0x008080,
    0xc0c0c0, 0x808080, 0xff0000, 0x00ff00, 0xffff00, 0x0000ff, 0xff00ff,
    0x00ffff, 0xffffff,

    0x000000, 0x00005f, 0x000087, 0x0000af, 0x0000d7, 0x0000ff, 0x005f00,
    0x005f5f, 0x005f87, 0x005faf, 0x005fd7, 0x005fff, 0x008700, 0x00875f,
    0x008787, 0x0087af,

    0x0087d7, 0x0087ff, 0x00af00, 0x00af5f, 0x00af87, 0x00afaf, 0x00afd7,
    0x00afff, 0x00d700, 0x00d75f, 0x00d787, 0x00d7af, 0x00d7d7, 0x00d7ff,
    0x00ff00, 0x00ff5f,

    0x00ff87, 0x00ffaf, 0x00ffd7, 0x00ffff, 0x5f0000, 0x5f005f, 0x5f0087,
    0x5f00af, 0x5f00d7, 0x5f00ff, 0x5f5f00, 0x5f5f5f, 0x5f5f87, 0x5f5faf,
    0x5f5fd7, 0x5f5fff,

    0x5f8700, 0x5f875f, 0x5f8787, 0x5f87af, 0x5f87d7, 0x5f87ff, 0x5faf00,
    0x5faf5f, 0x5faf87, 0x5fafaf, 0x5fafd7, 0x5fafff, 0x5fd700, 0x5fd75f,
    0x5fd787, 0x5fd7af,

    0x5fd7d7, 0x5fd7ff, 0x5fff00, 0x5fff5f, 0x5fff87, 0x5fffaf, 0x5fffd7,
    0x5fffff, 0x870000, 0x87005f, 0x870087, 0x8700af, 0x8700d7, 0x8700ff,
    0x875f00, 0x875f5f,

    0x875f87, 0x875faf, 0x875fd7, 0x875fff, 0x878700, 0x87875f, 0x878787,
    0x8787af, 0x8787d7, 0x8787ff, 0x87af00, 0x87af5f, 0x87af87, 0x87afaf,
    0x87afd7, 0x87afff,

    0x87d700, 0x87d75f, 0x87d787, 0x87d7af, 0x87d7d7, 0x87d7ff, 0x87ff00,
    0x87ff5f, 0x87ff87, 0x87ffaf, 0x87ffd7, 0x87ffff, 0xaf0000, 0xaf005f,
    0xaf0087, 0xaf00af,

    0xaf00d7, 0xaf00ff, 0xaf5f00, 0xaf5f5f, 0xaf5f87, 0xaf5faf, 0xaf5fd7,
    0xaf5fff, 0xaf8700, 0xaf875f, 0xaf8787, 0xaf87af, 0xaf87d7, 0xaf87ff,
    0xafaf00, 0xafaf5f,

    0xafaf87, 0xafafaf, 0xafafd7, 0xafafff, 0xafd700, 0xafd75f, 0xafd787,
    0xafd7af, 0xafd7d7, 0xafd7ff, 0xafff00, 0xafff5f, 0xafff87, 0xafffaf,
    0xafffd7, 0xafffff,

    0xd70000, 0xd7005f, 0xd70087, 0xd700af, 0xd700d7, 0xd700ff, 0xd75f00,
    0xd75f5f, 0xd75f87, 0xd75faf, 0xd75fd7, 0xd75fff, 0xd78700, 0xd7875f,
    0xd78787, 0xd787af,

    0xd787d7, 0xd787ff, 0xdfaf00, 0xdfaf5f, 0xdfaf87, 0xdfafaf, 0xdfafdf,
    0xdfafff, 0xdfdf00, 0xdfdf5f, 0xdfdf87, 0xdfdfaf, 0xdfdfdf, 0xdfdfff,
    0xdfff00, 0xdfff5f,

    0xdfff87, 0xdfffaf, 0xdfffdf, 0xdfffff, 0xff0000, 0xff005f, 0xff0087,
    0xff00af, 0xff00df, 0xff00ff, 0xff5f00, 0xff5f5f, 0xff5f87, 0xff5faf,
    0xff5fdf, 0xff5fff,

    0xff8700, 0xff875f, 0xff8787, 0xff87af, 0xff87df, 0xff87ff, 0xffaf00,
    0xffaf5f, 0xffaf87, 0xffafaf, 0xffafdf, 0xffafff, 0xffdf00, 0xffdf5f,
    0xffdf87, 0xffdfaf,

    0xffdfdf, 0xffdfff, 0xffff00, 0xffff5f, 0xffff87, 0xffffaf, 0xffffdf,
    0xffffff, 0x080808, 0x121212, 0x1c1c1c, 0x262626, 0x303030, 0x3a3a3a,
    0x444444, 0x4e4e4e,

    0x585858, 0x626262, 0x6c6c6c, 0x767676, 0x808080, 0x8a8a8a, 0x949494,
    0x9e9e9e, 0xa8a8a8, 0xb2b2b2, 0xbcbcbc, 0xc6c6c6, 0xd0d0d0, 0xdadada,
    0xe4e4e4, 0xeeeeee,
};

static unsigned int TERM_COLORS_HEX_GS[26] = {
    0x000000, 0xffffff,
    0x080808, 0x121212, 0x1c1c1c, 0x262626, 0x303030, 0x3a3a3a,
    0x444444, 0x4e4e4e, 0x585858, 0x626262, 0x6c6c6c, 0x767676,
    0x808080, 0x8a8a8a, 0x949494, 0x9e9e9e, 0xa8a8a8, 0xb2b2b2,
    0xbcbcbc, 0xc6c6c6, 0xd0d0d0, 0xdadada, 0xe4e4e4, 0xeeeeee,
};

static Magick::Color TERM_COLORS_256[256];

static Magick::Color TERM_COLORS_GS[26];

bool magick_colors_initialized = false;

im2a::Asciifier::Asciifier(Options *options, TermInfo *term_info,
    Magick::Image *image)
{
    _options = options;
    _term_info = term_info;
    _image = image;

    int scale_width = 0, scale_height = 0;

    if (_options->width() > 0 && options->height() > 0) {
        /* use provided values */
        scale_width = options->width();
        scale_height = options->height();
    } else if (_options->width() > 0) {
        /* guess height proportionally from width */
        double prop = (double)_image->columns() / (double)_options->width();
        scale_width = _options->width();
        scale_height = round((double)_image->rows() / prop);

        /* halven the height for terminal output */
        if (!_options->html() && !_options->pixel()) {
            scale_height /= 2;
        }
    } else if (_options->height() > 0) {
        /* guess width proportionally from height */
        double prop = (double)_image->rows() / (double)_options->height();
        scale_width = round((double)_image->columns() / prop);
        scale_height = _options->height();

        /* double the width for terminal output */
        if (!_options->html() && !_options->pixel()) {
            scale_width *= 2;
        }
    } else if (!_options->html()) {
        if (_options->pixel()) {
            /* scale the image to terminal size */
            double prop = (((double)_image->rows()) /
                ((double)(_term_info->lines() * 2 - 2)));
            scale_width = round((double)_image->columns() / prop);
            scale_height = _term_info->lines() * 2 - 2;

            /* fit width */
            if (scale_width > _term_info->columns()) {
                prop = (((double)_image->columns()) /
                    ((double)_term_info->columns() - 1));
                scale_width = _term_info->columns() - 1;
                scale_height = round((double)_image->rows() / prop);
            }
        } else {
            /* scale the image to terminal size */
            double prop = (((double)_image->rows()) /
                    ((double)_term_info->lines() - 1)) / 2.0;
            scale_width = round((double)_image->columns() / prop);
            scale_height = _term_info->lines() - 1;

            /* fit width (and halven result height) */
            if (scale_width > _term_info->columns()) {
                prop = (((double)_image->columns()) /
                        ((double)_term_info->columns() - 1));
                scale_width = _term_info->columns() - 1;
                scale_height = round(((double)_image->rows() / prop) / 2.0);
            }
        }
    }

    /* in pixel mode we need an even amount of rows */
    if (_options->pixel() && scale_height & 1) {
        ++scale_height;
    }

    /* scale it */
    if (scale_width > 0 && scale_height > 0 &&
        (scale_width != _image->columns() || scale_height != _image->rows())) {
        char scale_str[0x40];
        snprintf(scale_str, sizeof(scale_str), "%dx%d!",
            scale_width, scale_height);
        _image->scale(scale_str);
    }

    /* convert term hex colors to Magick::Color objects if needed */
    if (!magick_colors_initialized) {
        for (int x = 0; x < 26; ++x) {
            char buffer[16];
            sprintf(buffer, "#%06x", TERM_COLORS_HEX_GS[x]);
            TERM_COLORS_GS[x] = Magick::Color(buffer);
        }
        for (int x = 0; x < 256; ++x) {
            char buffer[16];
            sprintf(buffer, "#%06x", TERM_COLORS_HEX_256[x]);
            TERM_COLORS_256[x] = Magick::Color(buffer);
        }

        magick_colors_initialized = true;
    }
};

void im2a::Asciifier::asciify()
{
    /* invert the image if needed */
    if (_options->invert()) {
        _image->negate();
    }

    /* allocate a buffer for color index and output char */
    im2a::Pixel *buffer = (im2a::Pixel *)calloc(_image->columns() *
        _image->rows(), sizeof(im2a::Pixel));

    /* pixel packet */
#if IMAGEMAGICK_VERSION >= 0x7
    Magick::Quantum *pixels = _image->getPixels(0, 0, _image->columns(),
        _image->rows());
#else
    Magick::PixelPacket *pixels = _image->getPixels(0, 0, _image->columns(),
        _image->rows());
#endif

    /* red, green and blue weights as came from options */
    double red_weight = _options->red_weight(),
        green_weight = _options->green_weight(),
        blue_weight = _options->blue_weight();

    /* grayscale or not */
    bool grayscale = _options->grayscale();

    /* transparency support */
#if IMAGEMAGICK_VERSION >= 0x7
    bool transparent = _options->transparent() && _image->alpha();
    int alpha_idx = transparent ?
        MagickCore::GetPixelChannelOffset(_image->image(),
            MagickCore::AlphaPixelChannel) : -1;
    /* in imagemagick-7 the alpha channel is actually opacity ;[ */
    Magick::Quantum transparency_threshold =
        (1.0f - _options->transparency_threshold()) * MAGICK_QUANTUM_MAX;
#else
    bool transparent = _options->transparent() && _image->matte();
    /* in imagemagick-6 the opacity is actually alpha ;{ */
    Magick::Quantum transparency_threshold =
        _options->transparency_threshold() * MAGICK_QUANTUM_MAX;
#endif

    /* walk */
    for (ssize_t row = 0; row < _image->rows(); ++row) {
        for (ssize_t column = 0; column < _image->columns(); ++column) {
            /* calculate pixel offset */
            ssize_t offset = row * _image->columns() + column;

            /* get pixel's grayscale values */
#if IMAGEMAGICK_VERSION >= 0x7
            Magick::Quantum *pixel = pixels +
                offset * MagickCore::GetPixelChannels(_image->image());
            Magick::Quantum gs = red_weight * (double)(pixel[0])
                               + green_weight * (double)(pixel[1])
                               + blue_weight * (double)(pixel[2]);
            if (transparent && pixel[alpha_idx] <= transparency_threshold) {
                buffer[offset].transparent = 1;
            }
#else
            Magick::PixelPacket *pixel = pixels + offset;
            Magick::Quantum gs = red_weight * pixel->red
                               + green_weight * pixel->green
                               + blue_weight * pixel->blue;
            if (transparent && pixel->opacity >= transparency_threshold) {
                buffer[offset].transparent = 1;
            }
#endif

            /* start with an impossibly long distance */
            double min = INFINITY;

            /* selected index */
            int idx = 0;

            /* walk the grayscale terminal colors to find the nearest one */
            for (int index = 0; index < 26; ++index) {
                /* get the color itself */
                Magick::Color *color = &TERM_COLORS_GS[index];

                /* calculate distance */
#if IMAGEMAGICK_VERSION >= 0x7
                double distance = sqrt(
                    pow(color->quantumRed() - gs, 2) +
                    pow(color->quantumGreen() - gs, 2) +
                    pow(color->quantumBlue() - gs, 2)
                );
#else
                double distance = sqrt(
                    pow(color->redQuantum() - gs, 2) +
                    pow(color->greenQuantum() - gs, 2) +
                    pow(color->blueQuantum() - gs, 2)
                );
#endif

                if (distance < min) {
                    min = distance;
                    idx = index;
                }
            }

            /* store the char index */
            unsigned char char_index = idx;
            buffer[offset].char_index = char_index;

            /* if grayscale, store the resulted color index too, otherwise
               go and calculate it from the 256 colors */
            if (grayscale) {
                unsigned char color_index =
                    (idx == 0 ? 0 : idx == 1 ? 15 : idx + 230) & 0xFF;
                buffer[offset].color_index = color_index;
            } else {
                /* start with an impossibly long distance again */
                min = INFINITY;

                /* walk the 256 terminal colors */
                for (int index = 0; index < 256; ++index) {
                    /* get the color itself */
                    Magick::Color *color = &TERM_COLORS_256[index];

#if IMAGEMAGICK_VERSION >= 0x7
                    double distance = sqrt(
                        pow(color->quantumRed() - pixel[0], 2) +
                        pow(color->quantumGreen() - pixel[1], 2) +
                        pow(color->quantumBlue() - pixel[2], 2)
                    );
#else
                    /* calculate distance */
                    double distance = sqrt(
                        pow(color->redQuantum() - pixel->red, 2) +
                        pow(color->greenQuantum() - pixel->green, 2) +
                        pow(color->blueQuantum() - pixel->blue, 2)
                    );
#endif

                    if (distance < min) {
                        min = distance;
                        idx = index;
                    }
                }

                /* store the color index */
                unsigned char color_index = idx & 0xFF;
                buffer[offset].color_index = color_index;
            }
        }
    }

    /* get charset and length */
    const char *charset = _options->charset();
    size_t charset_len = strlen(charset);

    /* print header */
    print_header();

    /* print image */
    for (ssize_t row = 0; row < _image->rows(); ++row) {
        /* we can only optimize sequences on the same line */
        im2a::Pixel empty = {-1, -1, 0};
        im2a::Pixel *prev_color = &empty;
        im2a::Pixel *prev_color2 = &empty;

        /* put some leading spaces if needed */
        begin_line();

        for (ssize_t column = 0; column < _image->columns(); ++column) {
            /* calculate offset */
            ssize_t offset = row * _image->columns() + column;

            if (_options->pixel()) {
                /* pixel mode - use box-drawing characters */
                size_t offset2 = (row + 1) * _image->columns() + column;
                print_pixel(buffer + offset, buffer + offset2,
                    prev_color, prev_color2);
                prev_color = buffer + offset;
                prev_color2 = buffer + offset2;
            } else {
                /* normal mode - use the charset */
                print_char(buffer + offset, prev_color, charset, charset_len);
                prev_color = buffer + offset;
            }
        }

        /* clear colors and feed line */
        feed_line();

        /* in pixel mode we draw 2 rows at the same time */
        if (_options->pixel()) {
            ++row;
        }
    }

    /* print footer */
    print_footer();

    /* free the buffer */
    free(buffer);
}

void im2a::Asciifier::print_header()
{
    
}

void im2a::Asciifier::print_footer()
{
   
}

void im2a::Asciifier::print_char(im2a::Pixel *current, im2a::Pixel *prev,
    const char *charset, int charset_len)
{
    char c = charset[current->char_index % charset_len];

    if (current->transparent) {
        if (!prev->transparent) {
            std::cout << "\x1b[49m";
        }

        std::cout << " ";
    } else {
        if (current->color_index != prev->color_index) {
            std::cout << "\x1b[48;5;" << current->color_index << "m";
        }

        std::cout << c;
    }
}

void im2a::Asciifier::print_pixel(Pixel *current1, Pixel *current2,
    Pixel *prev1, Pixel *prev2)
{
    if (current1->color_index != prev1->color_index ||
        current1->transparent != prev1->transparent) {
        if (current1->transparent) {
            std::cout << "\x1b[49m";
        } else {
            std::cout << "\x1b[48;5;" << current1->color_index << "m";
        }
    }
    if (current2->color_index != prev2->color_index ||
        current2->transparent != prev2->transparent) {
        if (current2->transparent) {
            std::cout << "\x1b[39m";
        } else {
            std::cout << "\x1b[48;5;" << current2->color_index << "m";
        }
    }

    if (current1->color_index == current2->color_index ||
        (current1->transparent && current2->transparent)) {
        std::cout << " ";
    } else if (current1->transparent) {
        std::cout << "???";
    } else {
        std::cout << "???";
    }
}

void im2a::Asciifier::begin_line()
{
    if (_options->center() && !_options->html()) {
        std::cout <<
            std::setw((_term_info->columns() - _image->columns()) / 2) << " ";
    }
}

void im2a::Asciifier::feed_line()
{
    if (_options->html()) {
        std::cout << std::endl;
    } else {
        std::cout << "\x1b[0;0m" << std::endl;
    }
}
