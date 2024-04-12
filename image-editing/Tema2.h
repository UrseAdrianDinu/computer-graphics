#pragma once

#include <string>

#include "components/simple_scene.h"
#include "core/gpu/frame_buffer.h"

struct colPixel {
    float r, g, b;
    int x, y;

    colPixel(float rr, float gg, float bb, int xx, int yy) : r(rr), g(gg), b(bb), x(xx), y(yy) {}

};

namespace m2
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
         Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void OpenDialog();
        void OnFileSelected(const std::string &fileName);

        // Processing effects
        unsigned char* GrayScale(unsigned char* data);
        unsigned char* Blur(int blurRadius, unsigned char* data);
        unsigned char* Sobel(unsigned char* data);
        unsigned char* HysteresisThreshold(unsigned char* data);
        void MedianCut(std::vector<colPixel> &data, int n, int start, int end);
        unsigned char* MedianFilter(unsigned char* data);


        void SaveImage(const std::string &fileName);

     private:
        Texture2D *originalImage;
        Texture2D *processedImage;
        Texture2D *GrayScaleImage;
        Texture2D *BlurImage;
        Texture2D *SobelCannyImage;
        Texture2D * HysteresisThresholdImage;

        int outputMode;
        bool gpuProcessing;
        bool saveScreenToImage;
        FrameBuffer* frameBuffer;

        float gx[3][3] = {{ -1, 0, 1}, { -2, 0, 2}, { -1, 0, 1}};
        float gy[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    };
}   // namespace m2
