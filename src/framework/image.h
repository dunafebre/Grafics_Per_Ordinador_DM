/*
    + This file defines the class Image that allows to manipulate images.
    + It defines all the need operators for Color and Image
*/

#pragma once

#include <string.h>
#include <stdio.h>
#include <iostream>
#include "framework.h"

//remove unsafe warnings
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#endif

class FloatImage;
class Entity;
class Camera;
class Button;

// A matrix of pixels
class Image
{
    // A general struct to store all the information about a TGA file
    typedef struct sTGAInfo {
        unsigned int width;
        unsigned int height;
        unsigned int bpp; // Bits per pixel
        unsigned char* data; // Bytes with the pixel information
    } TGAInfo;
    
    //Cell
    struct Cell {
        int minx = 999999;
        int maxx = -999999;
    };

public:
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_pixel = 3; // Bits per pixel

    Color* pixels;

    // Constructors
    Image();
    Image(unsigned int width, unsigned int height);
    Image(const Image& c);
    Image& operator = (const Image& c); // Assign operator

    // Destructor
    ~Image();

    void Render();

    // Get the pixel at position x,y
    Color GetPixel(unsigned int x, unsigned int y) const { return pixels[ y * width + x ]; }
    Color& GetPixelRef(unsigned int x, unsigned int y)    { return pixels[ y * width + x ]; }
    Color GetPixelSafe(unsigned int x, unsigned int y) const {
        x = clamp((unsigned int)x, 0, width-1);
        y = clamp((unsigned int)y, 0, height-1);
        return pixels[ y * width + x ];
    }

    // Set the pixel at position x,y with value C
    void SetPixel(unsigned int x, unsigned int y, const Color& c) { if(x < 0 || x > width-1) return; if(y < 0 || y > height-1) return; pixels[ y * width + x ] = c; }
    inline void SetPixelUnsafe(unsigned int x, unsigned int y, const Color& c) { pixels[ y * width + x ] = c; }

    void Resize(unsigned int width, unsigned int height);
    void Scale(unsigned int width, unsigned int height);
    
    void FlipY(); // Flip the image top-down

    // Fill the image with the color C
    void Fill(const Color& c) { for(unsigned int pos = 0; pos < width*height; ++pos) pixels[pos] = c; }

    // Returns a new image with the area from (startx,starty) of size width,height
    Image GetArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height);

    // Save or load images from the hard drive
    bool LoadPNG(const char* filename, bool flip_y = true);
    bool LoadTGA(const char* filename, bool flip_y = false);
    bool SaveTGA(const char* filename);

    // Drawing primitives
    void DrawLineDDA(int x0, int y0, int x1, int y1, const Color& c);
    void DrawRect(int x, int y, int w, int h, const Color& borderColor, int borderWidth, bool isFilled, const Color& fillColor);
    void DrawTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Color& borderColor, bool isFilled, const Color& fillColor);
    void ScanLineDDA(int x0, int y0, int x1, int y1, std::vector<Cell>& table, int minY);
    void DrawImage(const Image& image, int x, int y);

    // Used to easy code
    #ifndef IGNORE_LAMBDAS

    // Applies an algorithm to every pixel in an image
    // you can use lambda sintax:   img.forEachPixel( [](Color c) { return c*2; });
    // or callback sintax:   img.forEachPixel( mycallback ); //the callback has to be Color mycallback(Color c) { ... }
    template <typename F>
    Image& ForEachPixel( F callback )
    {
        for(unsigned int pos = 0; pos < width*height; ++pos)
            pixels[pos] = callback(pixels[pos]);
        return *this;
    }
    #endif
};

// Image storing one float per pixel instead of a 3 or 4 component Color

class FloatImage
{
public:
    unsigned int width;
    unsigned int height;
    float* pixels;

    // CONSTRUCTORS
    FloatImage() { width = height = 0; pixels = NULL; }
    FloatImage(unsigned int width, unsigned int height);
    FloatImage(const FloatImage& c);
    FloatImage& operator = (const FloatImage& c); //assign operator

    //destructor
    ~FloatImage();

    void Fill(const float& v) { for (unsigned int pos = 0; pos < width * height; ++pos) pixels[pos] = v; }

    //get the pixel at position x,y
    float GetPixel(unsigned int x, unsigned int y) const { return pixels[y * width + x]; }
    float& GetPixelRef(unsigned int x, unsigned int y) { return pixels[y * width + x]; }

    //set the pixel at position x,y with value C
    void SetPixel(unsigned int x, unsigned int y, const float& v) { if (x < 0 || x > width - 1) return; if (y < 0 || y > height - 1) return; pixels[y * width + x] = v; }
    inline void SetPixelUnsafe(unsigned int x, unsigned int y, const float& v) { pixels[y * width + x] = v; }

    void Resize(unsigned int width, unsigned int height);
};

enum class ButtonType
{
    PENCIL,
    BLAU_CLAR,
    BLAU_FOSC,
    VERMELL,
    GROC,
    ROSA,
    BLANC,
    NEGRE,
    VERD,
    ERASER,
    TRIANGLE,
    LINE,
    RECTANGLE,
    CLEAR_IMAGE,
    LOAD_IMAGE,
    SAVE_IMAGE
};


class Button
{
public:
    Image image;
    Vector2 position;
    ButtonType type;
    
    // CONSTRUCTORS
    Button(Image img, Vector2 pos, ButtonType t)
    {
        image = img;
        position = pos;
        type = t;
    }
    
    // funció
    bool IsMouseInside(Vector2 mousePosition)
    {
        if (mousePosition.x >= position.x &&
            mousePosition.x <= position.x + image.width &&
            mousePosition.y >= position.y &&
            mousePosition.y <= position.y + image.height)
        {
            return true;
        }
        return false;
    }
};

class ParticleSystem
{
    
    static const int MAX_PARTICLES = 500;
    Image* framebuffer;
    
    struct Particle {
        Vector2 position;
        Vector2 velocity;
        Color color;
        float acceleration;
        float ttl;
        bool inactive;   
    };
    
    Particle particles[MAX_PARTICLES];
    
public:
    void Init()
    {   
        
        for (int i = 0; i < MAX_PARTICLES; i++)
        {
            particles[i].position = Vector2(rand() % 1500, rand() % 1000); //(rand() % (maximo - minimo + 1)) + minimo;
            particles[i].velocity = Vector2(-3.f + rand() % 6, 8.f + rand() % 12);
            particles[i].color = Color::WHITE;
            particles[i].acceleration = 0.f;
            particles[i].ttl = 100.f;
            particles[i].inactive = false;
        }
    }
    
    void Update(float dt)
    {
        for (int i = 0; i < MAX_PARTICLES; i++)
        {
            if (!particles[i].inactive)
            {
                particles[i].velocity.x += (-1.f + rand() % 3) * dt;
                particles[i].position = particles[i].position + particles[i].velocity * dt; //p(t+1) = p(t) + v*t
    
                particles[i].ttl -= dt;
                if (particles[i].ttl <= 0.f)
                {
                    particles[i].inactive = true;
                }
                
                if (particles[i].position.y > 1000)
                {
                    particles[i].position.y = -5;
                    particles[i].position.x = rand() % 1000;
                }
            }
        }
    }
    
    void Render(Image* framebuffer)
    {
        for (int i = 0; i < MAX_PARTICLES; i++)
        {
            if (!particles[i].inactive)
            {
                int x = (int)particles[i].position.x;
                int y = framebuffer->height - (int)particles[i].position.y;

                framebuffer->SetPixel(x, y, particles[i].color);
            }
        }
    }

};

