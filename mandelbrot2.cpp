#include <cstdint>
#include <iostream>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>
#include <cmath>
#include <complex>

std::uint32_t mandelbrot(double x0, double y0)
{
    double x = 0, y = 0; // z = x + iy
    std::uint32_t iteration = 0, imax = 1020;
    // |z| <= 2, z^2 <= 4
    while (x * x + y * y <= 2 * 2 && iteration < imax) {
        double proxZX, proxZY;
        // proxZ = Z^2 + C
        proxZX = x * x - y * y + x0;
        proxZY = 2 * x * y + y0;

        x = proxZX;
        y = proxZY;

        iteration++;
    }

    return iteration;
}

struct fractalpixel {
    std::int32_t X, Y;
    olc::Pixel Color;
};

class fractal : public olc::PixelGameEngine {
public:
    double mXmin = -2.0;
    double mXmax = 0.47;
    double mYmin = -1.12;
    double mYmax = 1.12;

    std::vector<fractalpixel> mImage;
    std::complex<double> c; // pontos C

public:
    fractal()
    {
        sAppName = "fractal";
    }

    void Render()
    {
        for (int x = 0; x < ScreenWidth(); x++)
            for (int y = 0; y < ScreenHeight(); y++) {
                auto it = mandelbrot(ScreenToX(x), ScreenToY(y));
                std::uint8_t color = it % 255;
                mImage[x + y * ScreenWidth()] = { x, y, olc::Pixel(color, color, color) };
            }
    }

    bool OnUserCreate() override
    {
        // Called once at the start, so create things here
        mImage.resize(ScreenWidth() * ScreenHeight());
        Render();
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // called once per frame
        for (const auto& pixel : mImage) Draw(pixel.X, pixel.Y, pixel.Color);

        if (GetMouse(0).bHeld) {
            int32_t mouseX = GetMouseX();
            int32_t mouseY = GetMouseY();

            c = std::complex<double>(ScreenToX(mouseX), ScreenToY(mouseY));

            std::vector<std::complex<double>> sequence;
            std::complex<double> z(0.0, 0.0);
            
            const int sequenceN = 255;

            // sequencia de pontos na tela
            for (int i = 0; i < sequenceN; i++) {
                sequence.push_back(z);
                z = z * z + c;
            }
            for (const auto& point : sequence) {
                auto index = &point-&sequence.front();
                int32_t x = XToScreen(point.real());
                int32_t y = YToScreen(point.imag());
                DrawCircle(x, y, 0, olc::Pixel(0,255-index,255));
                DrawCircle(x, y, 1, olc::Pixel(0,255-index,255));
                DrawCircle(x, y, 2, olc::Pixel(0,255-index,255));

            }
        }
        

        return true;
    }

    double ScreenToX(int32_t x)
    {
        return mXmin + x*(mXmax - mXmin)/ScreenWidth();
    }

    double ScreenToY(int32_t y)
    {
        return mYmin + y*(mYmax - mYmin)/ScreenHeight();
    }

    int32_t XToScreen(double x)
    {
        return static_cast<int32_t>((x - mXmin)/(mXmax - mXmin)*ScreenWidth());
    }

    int32_t YToScreen(double y)
    {
        return static_cast<int32_t>((y - mYmin)/(mYmax - mYmin)*ScreenHeight());
    }
};

int main()
{
    fractal myfractal;
    if (myfractal.Construct(1000, 800, 1, 1))
        myfractal.Start();

    return 0;
}

