#include "bitmaps.hpp"

// 16x9

const uint8_t controller_bmp[] = {
    0b00111111,
    0b01000000,
    0b10000010,
    0b10010000,
    0b10111001,
    0b10010001,
    0b10000000,
    0b01000111,
    0b00111000,

    0b11111100,
    0b00000010,
    0b01000001,
    0b00001001,
    0b10010101,
    0b10001001,
    0b00000001,
    0b11100010,
    0b00011100};

const Tetromino tetrominos[TETROMINO_COUNT] = {
    // O piece
    {
        .name = 'O',
        .size = 2,
        .bitmap = (uint8_t[]){
            1, 1, //
            1, 1, //
        },
    },
    // S piece
    {
        .name = 'S',
        .size = 3,
        .bitmap = (uint8_t[]){
            0, 2, 2, //
            2, 2, 0, //
            0, 0, 0, //
        },
    },
    // Z piece
    {
        .name = 'Z',
        .size = 3,
        .bitmap = (uint8_t[]){
            3, 3, 0, //
            0, 3, 3, //
            0, 0, 0, //
        },
    },
    // T piece
    {
        .name = 'T',
        .size = 3,
        .bitmap = (uint8_t[]){
            0, 4, 0, //
            4, 4, 4, //
            0, 0, 0, //
        },
    },
    // L piece
    {
        .name = 'L',
        .size = 3,
        .bitmap = (uint8_t[]){
            0, 0, 5, //
            5, 5, 5, //
            0, 0, 0, //
        },
    },
    // J piece
    {
        .name = 'J',
        .size = 3,
        .bitmap = (uint8_t[]){
            6, 0, 0, //
            6, 6, 6, //
            0, 0, 0, //
        },
    },
    // I piece
    {
        .name = 'I',
        .size = 4,
        .bitmap = (uint8_t[]){
            0, 0, 0, 0, //
            7, 7, 7, 7, //
            0, 0, 0, 0, //
            0, 0, 0, 0, //
        },
    },
};

const CRGB TetrisColorMap[TETROMINO_COUNT + 1] = {
    CRGB::Black,
    CRGB::Yellow,
    CRGB::Green,
    CRGB::Red,

    CRGB::Purple,
    CRGB::OrangeRed,
    CRGB::Blue,
    CRGB::Cyan,
};