#include <iostream>
#include "../headers/Image.hpp"
#include "../headers/PPMLoader.hpp"
#include <cstdlib>
#include "../headers/Map.hpp"
#include "../headers/Planner.hpp"
#include <ctime>
//#define TRIPPYCOLOURS
using namespace rw::sensor;
using namespace rw::loaders;

struct RGB
{
  uint8_t r, g, b;
};

RGB mapcolour(uint64_t value, uint64_t max)
{ //map colours according to value and maxvalue
  //the returned colour will be more red the closer it is to 0
  //and more blue the closer it is to max.
  //there exeption however, if the value is 1, the returned colour will be black
  //if value is less than .1% of max, the returned colour will be green
  if(value == 1) return {0, 0, 0};
  if(value <= max / 1000) return {0, 255, 0};
  RGB col;
  col.r = 255 - (value * 255) / max;

  #ifdef TRIPPYCOLOURS
  col.g = (value * 255) % max;
  #else
  col.g = 0 ;
  #endif

  col.b = (value * 255) / max;
  return col;
}

int main(int argc, char** argv) {
    if(argc < 5)
    {
      std::cout << "Not enough arguments" << std::endl;
      std::cout << "arguments should be:" << std::endl;
      std::cout << "<MapFile> <start x> <start y> <goal x> <goal y>" << std::endl;
      exit(0);
    }
    std::string filename(argv[1]);

    std::cout << "loading image..." << std::endl;
    Image* img = PPMLoader::load(filename);
    Image pathimg(img->getWidth(), img->getHeight(), Image::ColorCode::RGB, Image::PixelDepth::Depth8U);
    Image wavefront_img(img->getWidth(), img->getHeight(), Image::ColorCode::RGB, Image::PixelDepth::Depth8U);

    int channel = 0; // always 0 on grayscale image

    std::cout << "Converting image to obstacle map" << std::endl;
    // 0 pixels are obstacles, everything else is freespace
    std::vector< std::vector < mapSpace > > map;
    for(unsigned int x = 0; x < img->getWidth(); x++){
      std::vector< mapSpace > y_line;
        for( unsigned int y = 0; y < img->getHeight(); y++){
            int val = img->getPixelValuei( x, y, channel );
            if(val == 0){
                y_line.push_back(mapSpace::obstacle);
                pathimg.setPixel8U(x, y, 0, 0, 0);
            } else {
                y_line.push_back(mapSpace::freespace);
                pathimg.setPixel8U(x, y, 255, 255, 255);
            }
            if(y == img->getHeight() - 1)
              map.push_back(y_line);
        }
    }
    Planner Plan(map);
    point start = {atoi(argv[2]), atoi(argv[3])};
    point goal = {atoi(argv[4]), atoi(argv[5])};
    Plan.compute_wavefront(goal);
    auto path = Plan.getGoalPath(start);
    std::cout << "coloring the path" << std::endl;
    for(auto &p : path)
    {
      pathimg.setPixel8U(p.x, p.y, 255, 0, 0);
    }
    std::cout << "creating wavefront map" << std::endl;
    //First, find the max wavefront value
    uint64_t max = 0;
    for(unsigned int x = 0; x < img->getWidth(); x++)
    {
      for(unsigned int y = 0; y < img->getHeight(); y++)
      {
        if(Plan.getDistance({(int)x, (int)y}) > max)
          max = Plan.getDistance({(int)x, (int)y});
      }
    }
    //map the colours
    for(unsigned int x = 0; x < img->getWidth(); x++)
    {
      for(unsigned int y = 0; y < img->getHeight(); y++)
      {
        auto col = mapcolour(Plan.getDistance({(int)x, (int)y}), max);
        wavefront_img.setPixel8U(x, y, col.r, col.g, col.b);
      }
    }
    std::cout << "Path Lenght:" << path.size() << std::endl;
    std::cout << "saving image..." << std::endl;
    // save path image
    pathimg.saveAsPPM("path.ppm");
    //save wavefront image
    wavefront_img.saveAsPPM("wavefront.ppm");


    // cleanup
    delete img;
}
