#include <iostream>
#include "../headers/Image.hpp"
#include "../headers/PPMLoader.hpp"
#include <cstdlib>
#include "../headers/Map.hpp"
#include "../headers/PathFinder.hpp"
#include <ctime>

using namespace rw::sensor;
using namespace rw::loaders;


int main(int argc, char** argv) {
    if(argc < 3)
    {
      std::cout << "Not enough arguments" << std::endl;
      std::cout << "arguments should be:" << std::endl;
      std::cout << "<MapFile> <x> <y>" << std::endl;
      exit(0);
    }
    srand(time(NULL));
    std::string filename(argv[1]);

    std::cout << "loading image..." << std::endl;
    Image* img = PPMLoader::load(filename);
    // do stuff here
    int channel = 0; // allways 0 on grayscale image

    std::cout << "Converting image to obstacle map" << std::endl;
    // 0 pixels are obstacles, 255 is free space
    std::vector< std::vector < mapSpace > > map;
    for(unsigned int x=0;x<img->getWidth();x++){
      std::vector< mapSpace > y_line;
        for( unsigned int y=0;y<img->getHeight();y++){
            int val = img->getPixelValuei( x, y, channel );
            if(val==0){
                y_line.push_back(mapSpace::obstacle);
            } else if( val==255 ) {
                y_line.push_back(mapSpace::freespace);
            }
            if(y == img->getHeight() - 1)
              map.push_back(y_line);
        }
    }
    PathFinder PF(map);
    point start = {atoi(argv[2]), atoi(argv[3])};
    auto closestobst = PF.FindClosestObstacle(start);
    std::cout << "Closest obstacle is at (" <<  closestobst.x << ", " << closestobst.y << ")" << std::endl;
    std::cout << "Distance(straight line) to obstacle is " << start.GetDistance(closestobst) << " pixels." << std::endl;
    std::cout << "direction (vectorized) is (" << closestobst.x - start.x <<
        ", " << closestobst.y - start.y << ")" << std::endl;
    img->saveAsPGM("path.pgm");

    // cleanup
    delete img;
}
