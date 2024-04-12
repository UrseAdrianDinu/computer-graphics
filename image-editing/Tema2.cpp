#include "lab_m2/Tema2/Tema2.h"

#include <vector>
#include <iostream>

#include "pfd/portable-file-dialogs.h"

using namespace std;
using namespace m2;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */



Tema2::Tema2()
{
    outputMode = 0;
    gpuProcessing = false;
    saveScreenToImage = false;
    window->SetSize(600, 600);
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    // Load default texture fore imagine processing
    originalImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube", "pos_x.png"), nullptr, "image", true, true);
    processedImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube", "pos_x.png"), nullptr, "newImage", true, true);
    GrayScaleImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube", "pos_x.png"), nullptr, "newImage", true, true);
    BlurImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube", "pos_x.png"), nullptr, "newImage", true, true);

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema2", "shaders");

    // Create a shader program for particle system
    {
        Shader *shader = new Shader("ImageProcessing");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    auto resolution = window->GetResolution();

    frameBuffer = new FrameBuffer();
    frameBuffer->Generate(resolution.x, resolution.y, 1, true);
}


void Tema2::FrameStart()
{
}


void Tema2::Update(float deltaTimeSeconds)
{
    ClearScreen();

    auto shader = shaders["ImageProcessing"];
    shader->Use();

    if (saveScreenToImage)
    {
        window->SetSize(originalImage->GetWidth(), originalImage->GetHeight());
    }

    if (saveScreenToImage)
    {
        window->SetSize(originalImage->GetWidth(), originalImage->GetHeight());
    }

    int flip_loc = shader->GetUniformLocation("flipVertical");
    glUniform1i(flip_loc, saveScreenToImage ? 0 : 1);

    int screenSize_loc = shader->GetUniformLocation("screenSize");
    glm::ivec2 resolution = window->GetResolution();
    glUniform2i(screenSize_loc, resolution.x, resolution.y);

    int outputMode_loc = shader->GetUniformLocation("outputMode");
    glUniform1i(outputMode_loc, outputMode);

    int locTexture = shader->GetUniformLocation("textureImage");
    glUniform1i(locTexture, 0);

    auto textureImage = (gpuProcessing == true) ? originalImage : processedImage;
    textureImage->BindToTextureUnit(GL_TEXTURE0);


    RenderMesh(meshes["quad"], shader, glm::mat4(1));


    if (saveScreenToImage)
    {
        saveScreenToImage = false;

        GLenum format = GL_RGB;
        if (originalImage->GetNrChannels() == 4)
        {
            format = GL_RGBA;
        }

        glReadPixels(0, 0, originalImage->GetWidth(), originalImage->GetHeight(), format, GL_UNSIGNED_BYTE, processedImage->GetImageData());
        processedImage->UploadNewData(processedImage->GetImageData());
        SaveImage("shader_processing_" + std::to_string(outputMode));

        float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
        window->SetSize(static_cast<int>(600 * aspectRatio), 600);
    }
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem();
}


void Tema2::OnFileSelected(const std::string &fileName)
{
    if (fileName.size())
    {
        std::cout << fileName << endl;
        originalImage = TextureManager::LoadTexture(fileName, nullptr, "image", true, true);
        processedImage = TextureManager::LoadTexture(fileName, nullptr, "newImage", true, true);

        float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
        window->SetSize(static_cast<int>(600 * aspectRatio), 600);
    }
}


unsigned char* Tema2::GrayScale(unsigned char* data)
{
    unsigned int channels = originalImage->GetNrChannels();

    if (channels < 3)
        return NULL;

    glm::ivec2 imageSize = glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight());
    unsigned char* newData = (unsigned char*)malloc((imageSize.x * imageSize.y) * 3 * sizeof(unsigned char));

    for (int i = 0; i < imageSize.y; i++)
    {
        for (int j = 0; j < imageSize.x; j++)
        {
            int offset = channels * (i * imageSize.x + j);

            // Reset save image data
            char value = static_cast<char>(data[offset + 0] * 0.2f + data[offset + 1] * 0.71f + data[offset + 2] * 0.07);
            memset(&newData[offset], value, 3);
        }
    }
    return newData;
}


unsigned char* Tema2::Blur(int blurRadius, unsigned char* data)
{
    unsigned int channels = originalImage->GetNrChannels();

    if (channels < 3)
        return NULL;

    glm::ivec2 imageSize = glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight());
    
    unsigned char* newData = (unsigned char *)malloc((imageSize.x * imageSize.y) * 3 * sizeof(unsigned char));

    float samples = pow((2 * blurRadius + 1), 2);

    for (int i = blurRadius; i < imageSize.y - blurRadius; i++)
    {
        for (int j = blurRadius; j < imageSize.x - blurRadius; j++)
        {
            glm::vec4 sum = glm::vec4(0);
            int offset = 0;
            for (int blur_i = -blurRadius; blur_i <= blurRadius; blur_i++)
            {
                for (int blur_j = -blurRadius; blur_j <= blurRadius; blur_j++)
                {
                    offset = channels * ((i + blur_i) * imageSize.x + (j + blur_j));
                    sum.r += data[offset + 0];
                    sum.g += data[offset + 1];
                    sum.b += data[offset + 2];
                }
            }
            offset = channels * (i * imageSize.x + j);
            // Reset save image data
            memset(&newData[offset], static_cast<char>(sum.r/samples), 1);
            memset(&newData[offset + 1], static_cast<char>(sum.g/samples), 1);
            memset(&newData[offset + 2], static_cast<char>(sum.b/samples), 1);
        }
    }

    return newData;
}


unsigned char* Tema2::Sobel(unsigned char* data)
{
    unsigned int channels = originalImage->GetNrChannels();

    if (channels < 3)
        return NULL;

    glm::ivec2 imageSize = glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight());
    unsigned char* newData = (unsigned char*)malloc((imageSize.x * imageSize.y) * 3 * sizeof(unsigned char));
    double max_gradient = -1;
    for (int i = 2; i < imageSize.y - 1; i++)
    {
        for (int j = 2; j < imageSize.x - 1; j++)
        {
            int offset = 0;

            int dx = 0, dy = 0;

            for (int depl_i = 0; depl_i < 3; depl_i++) {
                for (int depl_j = 0; depl_j < 3; depl_j++) {
                    offset = channels * ((i + depl_i - 1) * imageSize.x + (j + depl_j - 1));
                    dx += data[offset] * gx[depl_i][depl_j];
                }
            }

            for (int depl_i = 0; depl_i < 3; depl_i++) {
                for (int depl_j = 0; depl_j < 3; depl_j++) {
                    offset = channels * ((i + depl_i - 1) * imageSize.x + (j + depl_j - 1));
                    dy += data[offset] * gy[depl_i][depl_j];
                }
            }

            offset = channels * (i * imageSize.x + j);
            double gradient = sqrt(dx * dx + dy * dy);
            double theta = 180 / (2 * M_PI) * (atan2(dy, dx) + M_PI);


            if ((0 <= theta && theta < 22.5) || (157.5 <= theta && theta <= 180)) {
                double da = 0, db = 0;
                dx = 0;
                dy = 0;
                for (int depl_i = 0; depl_i < 3; depl_i++) {
                    for (int depl_j = 0; depl_j < 3; depl_j++) {
                        offset = channels * ((i + depl_i - 1) * imageSize.x + (j - 1 + depl_j - 1));
                        dx += data[offset] * gx[depl_i][depl_j];
                    }
                }

                for (int depl_i = 0; depl_i < 3; depl_i++) {
                    for (int depl_j = 0; depl_j < 3; depl_j++) {
                        offset = channels * ((i + depl_i - 1) * imageSize.x + (j - 1 + depl_j - 1));
                        dy += data[offset] * gy[depl_i][depl_j];
                    }
                }

                da = sqrt(dx*dx + dy*dy);
                dx = 0;
                dy = 0;

                for (int depl_i = 0; depl_i < 3; depl_i++) {
                    for (int depl_j = 0; depl_j < 3; depl_j++) {
                        offset = channels * ((i + depl_i - 1) * imageSize.x + (j + 1 + depl_j - 1));
                        dx += data[offset] * gx[depl_i][depl_j];
                    }
                }

                for (int depl_i = 0; depl_i < 3; depl_i++) {
                    for (int depl_j = 0; depl_j < 3; depl_j++) {
                        offset = channels * ((i + depl_i - 1) * imageSize.x + (j + 1 + depl_j - 1));
                        dy += data[offset] * gy[depl_i][depl_j];
                    }
                }
                db = sqrt(dx * dx + dy * dy);

                if (da > gradient || db > gradient) {
                    gradient = 0;
                }

            }
            else if (22.5 <= theta && theta < 67.5) {
                double da = 0, db = 0;
                dx = 0;
                dy = 0;
                for (int depl_i = 0; depl_i < 3; depl_i++) {
                    for (int depl_j = 0; depl_j < 3; depl_j++) {
                        offset = channels * ((i + 1 + depl_i - 1) * imageSize.x + (j + 1 + depl_j - 1));
                        dx += data[offset] * gx[depl_i][depl_j];
                    }
                }

                for (int depl_i = 0; depl_i < 3; depl_i++) {
                    for (int depl_j = 0; depl_j < 3; depl_j++) {
                        offset = channels * ((i + 1 + depl_i - 1) * imageSize.x + (j + 1 + depl_j - 1));
                        dy += data[offset] * gy[depl_i][depl_j];
                    }
                }

                da = sqrt(dx * dx + dy * dy);
                dx = 0;
                dy = 0;

                for (int depl_i = 0; depl_i < 3; depl_i++) {
                    for (int depl_j = 0; depl_j < 3; depl_j++) {
                        offset = channels * ((i - 1 + depl_i - 1) * imageSize.x + (j - 1 + depl_j - 1));
                        dx += data[offset] * gx[depl_i][depl_j];
                    }
                }

                for (int depl_i = 0; depl_i < 3; depl_i++) {
                    for (int depl_j = 0; depl_j < 3; depl_j++) {
                        offset = channels * ((i - 1 + depl_i - 1) * imageSize.x + (j - 1 + depl_j - 1));
                        dy += data[offset] * gy[depl_i][depl_j];
                    }
                }
                db = sqrt(dx * dx + dy * dy);

                if (da > gradient || db > gradient) {
                    gradient = 0;
                }

            }
            else if (67.5 <= theta && theta < 112.5) {
                double da = 0, db = 0;
                dx = 0;
                dy = 0;
                for (int depl_i = 0; depl_i < 3; depl_i++) {
                    for (int depl_j = 0; depl_j < 3; depl_j++) {
                        offset = channels * ((i + 1 + depl_i - 1) * imageSize.x + (j + depl_j - 1));
                        dx += data[offset] * gx[depl_i][depl_j];
                    }
                }

                for (int depl_i = 0; depl_i < 3; depl_i++) {
                    for (int depl_j = 0; depl_j < 3; depl_j++) {
                        offset = channels * ((i + 1 + depl_i - 1) * imageSize.x + (j + depl_j - 1));
                        dy += data[offset] * gy[depl_i][depl_j];
                    }
                }

                da = sqrt(dx * dx + dy * dy);
                dx = 0;
                dy = 0;

                for (int depl_i = 0; depl_i < 3; depl_i++) {
                    for (int depl_j = 0; depl_j < 3; depl_j++) {
                        offset = channels * ((i - 1 + depl_i - 1) * imageSize.x + (j + depl_j - 1));
                        dx += data[offset] * gx[depl_i][depl_j];
                    }
                }

                for (int depl_i = 0; depl_i < 3; depl_i++) {
                    for (int depl_j = 0; depl_j < 3; depl_j++) {
                        offset = channels * ((i - 1 + depl_i - 1) * imageSize.x + (j + depl_j - 1));
                        dy += data[offset] * gy[depl_i][depl_j];
                    }
                }
                db = sqrt(dx * dx + dy * dy);

                if (da > gradient || db > gradient) {
                    gradient = 0;
                }

            }
            else if (112.5 <= theta && theta < 157.5) {
                double da = 0, db = 0;
                dx = 0;
                dy = 0;
                for (int depl_i = 0; depl_i < 3; depl_i++) {
                    for (int depl_j = 0; depl_j < 3; depl_j++) {
                        offset = channels * ((i + 1 + depl_i - 1) * imageSize.x + (j - 1 + depl_j - 1));
                        dx += data[offset] * gx[depl_i][depl_j];
                    }
                }

                for (int depl_i = 0; depl_i < 3; depl_i++) {
                    for (int depl_j = 0; depl_j < 3; depl_j++) {
                        offset = channels * ((i + 1 + depl_i - 1) * imageSize.x + (j - 1 + depl_j - 1));
                        dy += data[offset] * gy[depl_i][depl_j];
                    }
                }

                da = sqrt(dx * dx + dy * dy);
                dx = 0;
                dy = 0;

                for (int depl_i = 0; depl_i < 3; depl_i++) {
                    for (int depl_j = 0; depl_j < 3; depl_j++) {
                        offset = channels * ((i - 1 + depl_i - 1) * imageSize.x + (j + 1 + depl_j - 1));
                        dx += data[offset] * gx[depl_i][depl_j];
                    }
                }

                for (int depl_i = 0; depl_i < 3; depl_i++) {
                    for (int depl_j = 0; depl_j < 3; depl_j++) {
                        offset = channels * ((i - 1 + depl_i - 1) * imageSize.x + (j + 1 + depl_j - 1));
                        dy += data[offset] * gy[depl_i][depl_j];
                    }
                }
                db = sqrt(dx * dx + dy * dy);

                if (da > gradient || db > gradient) {
                    gradient = 0;
                }
            }

           if (gradient < 50)
               gradient = 0;
           else if (gradient > 100)
               gradient = 255;
           else
               gradient = 100;
          

           offset = channels * (i * imageSize.x + j);
           memset(&newData[offset], static_cast<unsigned char>(gradient), 3);

        }
    }
    return newData;
}


unsigned char* Tema2::HysteresisThreshold(unsigned char* data)
{
    unsigned int channels = originalImage->GetNrChannels();

    if (channels < 3)
        return NULL;

    glm::ivec2 imageSize = glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight());
    unsigned char* newData = (unsigned char*)malloc((imageSize.x * imageSize.y) * 3 * sizeof(char));
    memcpy(&newData, &data, sizeof(newData));
    for (int i = 2; i < imageSize.y - 2; i++)
    {
        for (int j = 2; j < imageSize.x - 2; j++)
        {
            int offset = channels * (i * imageSize.x + j);

            if (data[offset] == 100)
            {
                int ok = 0;
                for (int ii = -1; ii <= 1; ii++) {
                    for (int jj = -1; jj <= 1; jj++) {
                        offset = channels * ((i + ii) * imageSize.x + j + jj);
                        if (data[offset] == 255)
                        {
                            ok = 1;
                            break;
                        }
                    }
                }

                offset = channels * (i * imageSize.x + j);
                if (ok == 1)
                {
                    memset(&newData[offset], static_cast<char>(255), 3);
                }
                else {
                    memset(&newData[offset], static_cast<char>(0), 3);
                }

            }
        }
    }

    return newData;
}

bool compareByRed(colPixel p1, colPixel p2) {
    return (p1.r < p2.r);
}

bool compareByGreen(colPixel p1, colPixel p2) {
    return (p1.g < p2.g);
}

bool compareByBlue(colPixel p1, colPixel p2) {
    return (p1.b < p2.b);
}

void Tema2::MedianCut(std::vector<colPixel>& data, int n, int start, int end)
{
    if (start == end)
        return;
    if (n == 0) {
        int r_sum = 0, g_sum = 0, b_sum = 0;
        for (int i = start; i < end; i++)
        {
            r_sum += data[i].r;
            g_sum += data[i].g;
            b_sum += data[i].b;
        }
        r_sum /= (end - start);
        g_sum /= (end - start);
        b_sum /= (end - start);
        for (int i = start; i < end; i++)
        {
            data[i].r = r_sum;
            data[i].g = g_sum;
            data[i].b = b_sum;
        }
        return;
    }
    int min_r = data[0].r, max_r = data[0].r;
    int min_g = data[0].g, max_g = data[0].g;
    int min_b = data[0].b, max_b = data[0].b;
    for (int i = start; i < end; i++)
    {
        if (data[i].r < min_r)
            min_r = data[i].r;
        else
            max_r = data[i].r;

        if (data[i].g < min_g)
            min_g = data[i].g;
        else
            max_g = data[i].g;

        if (data[i].r < min_r)
            min_b = data[i].r;
        else
            max_b = data[i].r;
    }

    int diff_r = max_r - min_r;
    int diff_g = max_g - min_g;
    int diff_b = max_b - min_b;

    if (diff_r > diff_g && diff_r > diff_b) {
        sort(data.begin() + start, data.begin() + end, compareByRed);
    } else if (diff_g > diff_r && diff_g > diff_b) {
        sort(data.begin() + start, data.begin() + end, compareByGreen);
    } else {
        sort(data.begin() + start, data.begin() + end , compareByBlue);
    }
    int mid = (start + end) / 2;
    MedianCut(data, n - 1, start, mid);
    MedianCut(data, n - 1, mid + 1, end);

}

unsigned char* Tema2::MedianFilter(unsigned char* data)
{
    unsigned int channels = originalImage->GetNrChannels();

    if (channels < 3)
        return NULL;

    glm::ivec2 imageSize = glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight());
    unsigned char* newData = (unsigned char*)malloc((imageSize.x * imageSize.y) * 3 * sizeof(unsigned char));
    memcpy(&newData, &data, sizeof(newData));
    for (int i = 2; i < imageSize.y - 2; i++)
    {
        for (int j = 2; j < imageSize.x - 2; j++)
        {
            vector<float> reds;
            vector<float> greens;
            vector<float> blues;
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    int offset = channels * ((i + di) * imageSize.x + j + dj);
                    reds.push_back(data[offset]);
                    greens.push_back(data[offset + 1]);
                    blues.push_back(data[offset + 2]);
                }
            }

            sort(reds.begin(), reds.end());
            sort(greens.begin(), greens.end());
            sort(blues.begin(), blues.end());
             
            int offset = channels * (i * imageSize.x + j);

            memset(&newData[offset], reds[reds.size()/2], 1);
            memset(&newData[offset + 1], greens[greens.size()/2], 1);
            memset(&newData[offset + 2], blues[greens.size()/2], 1);
            
        }
    }
    cout << "Done\n";
    return newData;
}


void Tema2::SaveImage(const std::string &fileName)
{
    cout << "Saving image! ";
    processedImage->SaveToFile((fileName + ".png").c_str());
    cout << "[Done]" << endl;
}


void Tema2::OpenDialog()
{
    std::vector<std::string> filters =
    {
        "Image Files", "*.png *.jpg *.jpeg *.bmp",
        "All Files", "*"
    };

    auto selection = pfd::open_file("Select a file", ".", filters).result();
    if (!selection.empty())
    {
        std::cout << "User selected file " << selection[0] << "\n";
        OnFileSelected(selection[0]);
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F || key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE)
    {
        OpenDialog();
    }

    if (key == GLFW_KEY_E)
    {
        gpuProcessing = !gpuProcessing;
        if (gpuProcessing == false)
        {
            outputMode = 0;
        }
        cout << "Processing on GPU: " << (gpuProcessing ? "true" : "false") << endl;
    }


    if (key == GLFW_KEY_S && mods & GLFW_MOD_CONTROL)
    {
        if (!gpuProcessing)
        {
            SaveImage("processCPU_" + std::to_string(outputMode));
        } else {
            saveScreenToImage = true;
        }
    }

    if (key == GLFW_KEY_M)
    {
        unsigned char* data = originalImage->GetImageData();
        glm::ivec2 imageSize = glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight());
        unsigned int channels = originalImage->GetNrChannels();
        unsigned char *frontiers = (unsigned char*)malloc((imageSize.x * imageSize.y) * 3 * sizeof(unsigned char));
        memcpy(&frontiers, &data, sizeof(data));

        frontiers = GrayScale(frontiers);
        frontiers = Blur(3, frontiers);
        frontiers = Sobel(frontiers);
        frontiers = HysteresisThreshold(frontiers);


        vector<colPixel> pixels;
        for (int i = 0; i < imageSize.y; i++)
        {
            for (int j = 0; j < imageSize.x; j++)
            {
                int offset = channels * (i * imageSize.x + j);
                pixels.push_back(colPixel(data[offset], data[offset + 1], data[offset + 2], j, i));
            }
        }

        int len = (int)pixels.size();
        MedianCut(pixels, 3, 0, len);

        for (int i = 0; i < len; i++)
        {
            int offset = channels * (pixels[i].y * imageSize.x + pixels[i].x);
            memset(&data[offset], static_cast<char>(pixels[i].r), 1);
            memset(&data[offset + 1], static_cast<char>(pixels[i].g), 1);
            memset(&data[offset + 2], static_cast<char>(pixels[i].b), 1);
            
        }

        data = MedianFilter(data);

        for (int i = 0; i < imageSize.y; i++)
        {
            for (int j = 0; j < imageSize.x; j++)
            {
                int offset = channels * (i * imageSize.x + j);
                if (frontiers[offset] == 255)
                    memset(&data[offset], static_cast<char>(0), 3);
            }
        }

        processedImage->UploadNewData(data);
    }

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Tema2::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
