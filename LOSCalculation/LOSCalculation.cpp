// LOSCalculation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <CL/cl.hpp>
#include <string>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


cl::Program createProgram(const char* srcFileName);


int main()
{
    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* imgData = stbi_load("../myDEM_256x256_24bit_GrayCh.bmp", &width, &height, &nrChannels, 0);
    unsigned char* outData = new unsigned char[width * height * nrChannels];

    cl_int err = CL_SUCCESS;

    cl::Program program = createProgram("LOS.cl");
    cl::Context context = program.getInfo<CL_PROGRAM_CONTEXT>();
    cl::Device device = context.getInfo<CL_CONTEXT_DEVICES>().front();

    cl::CommandQueue queue = cl::CommandQueue(context, device);


    cl::ImageFormat imageFormat = cl::ImageFormat(CL_RGB, CL_UNSIGNED_INT8);
    cl::Image2D inputImage = cl::Image2D(context, CL_MEM_READ_ONLY, imageFormat, width, height);
    cl::Image2D inputImage = cl::Image2D(context, CL_MEM_WRITE_ONLY, imageFormat, width, height);

    cl::size_t<3> origin;
    origin[0] = 0;
    origin[1] = 0;
    origin[2] = 0;

    cl::size_t<3> region;
    region[0] = width;
    region[1] = height;
    region[2] = nrChannels;

    queue.enqueueWriteImage(inputImage, CL_TRUE, origin, region, 0, 0, imgData);






    
    std::cout << "Hello World!\n";
}


cl::Program createProgram(const char* srcFileName)
{
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    cl::Platform platform = platforms.front();
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
    cl::Device device = devices.front();

    std::ifstream srcFile(srcFileName);
    std::string src(std::istreambuf_iterator<char>(srcFile), (std::istreambuf_iterator<char>()));

    cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

    cl::Context context(device);
    cl::Program program(context, sources);

    program.build("-cl-std=CL1.2");

    return program;

}