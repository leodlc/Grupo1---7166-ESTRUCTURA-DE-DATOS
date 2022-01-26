/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <Latchezar Tzvetkoff> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Latchezar Tzvetkoff
 * ----------------------------------------------------------------------------
 */

/* options.cpp */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define IM2A_VERSION_STRING "1.0"

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "getopt.h"

#include "options.h"

im2a::Options::Options()
{

    /* defaults */
    _html = false;
    _invert = false;
    _center = false;
    _pixel = false;
    _width = 0;
    _height = 0;
    _charset = NULL;
    _grayscale = false;
    _transparent = false;
    _transparency_threshold = 1.0000f;
    _red_weight = 0.2989f;
    _green_weight = 0.5866f;
    _blue_weight = 0.1145f;
    _source = NULL;

    if (!_charset) {
        _charset = _strdup(" M   ...',;:clodxkO0KXNWMM");
    }
}

im2a::Options::~Options()
{
    if (_charset) {
        free(_charset);
    }
    if (_source) {
        free(_source);
    }
}

void im2a::Options::usage(std::ostream &out, int exit_code)
{
    
}

void im2a::Options::source(const char* source) {
    _source = (char*)calloc(strlen(source), sizeof(char) + 1);
    strcpy(_source, source);
}

void im2a::Options::width(int width) {
    _width = width;
}

void im2a::Options::height(int height) {
    _height = height;
}

void im2a::Options::transparency_threshold(double treshold) {
    _transparency_threshold = treshold;
}

void im2a::Options::red_weight(double weight) {
    _red_weight = weight;
}

void im2a::Options::green_weight(double weight) {
    _green_weight = weight;
}

void im2a::Options::blue_weight(double weight) {
    _blue_weight = weight;
}

bool im2a::Options::html() const
{
    return _html;
}

bool im2a::Options::invert() const
{
    return _invert;
}

bool im2a::Options::center() const
{
    return _center;
}

bool im2a::Options::pixel() const {
    return _pixel;
}

int im2a::Options::width() const
{
    return _width;
}

int im2a::Options::height() const
{
    return _height;
}

char *im2a::Options::charset() const
{
    return _charset;
}

bool im2a::Options::grayscale() const
{
    return _grayscale;
}

bool im2a::Options::transparent() const
{
    return _transparent;
}

double im2a::Options::transparency_threshold() const
{
    return _transparency_threshold;
}

double im2a::Options::red_weight() const
{
    return _red_weight;
}

double im2a::Options::green_weight() const
{
    return _green_weight;
}

double im2a::Options::blue_weight() const
{
    return _blue_weight;
}

char *im2a::Options::source() const
{
    return _source;
}
