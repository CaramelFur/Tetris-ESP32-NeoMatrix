#include "bitmaps.hpp"

// 16x9

const uint8_t controller_bmp[] = {
    0b00111111,
    0b01000000,
    0b10000010,
    0b10010000,
    0b10111001,
    0b10010000,
    0b10000000,
    0b01000111,
    0b00111000,

    0b11111100,
    0b00000010,
    0b01000001,
    0b00001001,
    0b10010101,
    0b00001001,
    0b00000001,
    0b11100010,
    0b00011100};

const TetrisPiece tetris_pieces[] = {
    // O piece
    {
        .size = 2,
        .color = CRGB::Yellow,
        .bitmap = (uint8_t[]){
            0b11000000,
            0b11000000,
        },
    },
    // S piece
    {
        .size = 3,
        .color = CRGB::Green,
        .bitmap = (uint8_t[]){
            0b01100000,
            0b11000000,
            0b00000000,
        },
    },
    // Z piece
    {
        .size = 3,
        .color = CRGB::Red,
        .bitmap = (uint8_t[]){
            0b11000000,
            0b01100000,
            0b00000000,
        },
    },
    // T piece
    {
        .size = 3,
        .color = CRGB::Purple,
        .bitmap = (uint8_t[]){
            0b01000000,
            0b11100000,
            0b00000000,
        },
    },
    // L piece
    {
        .size = 3,
        .color = CRGB::Orange,
        .bitmap = (uint8_t[]){
            0b00100000,
            0b11100000,
            0b00000000,
        },
    },
    // J piece
    {
        .size = 3,
        .color = CRGB::Blue,
        .bitmap = (uint8_t[]){
            0b10000000,
            0b11100000,
            0b00000000,
        },
    },
    // I piece
    {
        .size = 4,
        .color = CRGB::Cyan,
        .bitmap = (uint8_t[]){
            0b00000000,
            0b11110000,
            0b00000000,
            0b00000000,
        },
    },
};