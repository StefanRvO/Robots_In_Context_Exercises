#include <iostream>
#include "../headers/Image.hpp"
#include "../headers/PPMLoader.hpp"
#include <cstdlib>
#include "../headers/Map.hpp"
#include "../headers/Bug1.hpp"
#include "../headers/Bug2.hpp"
#include <ctime>

using namespace rw::sensor;
using namespace rw::loaders;


int main(int argc, char** argv) {
    if(argc < 5)
    {
      std::cout << "Not enough arguments" << std::endl;
      std::cout << "arguments should be:" << std::endl;
      std::cout << "<MapFile> <start_x> <start_y> <end_y> <end_x>" << std::endl;
      exit(0);
    }
    srand(time(NULL));
    std::string filename(argv[1]);

    std::cout << "loading image..." << std::endl;
    Image* img = PPMLoader::load(filename);
    // do stuff here
    int channel = 0; // allways 0 on grayscale image

    std::cout << "Converting image to obstacle map" << std::endl;
    // 255 pixels are obstacles, 0 is free space
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
    Bug1 Bug(&map);
    point startPoint = {atoi(argv[2]), atoi(argv[3])};
    point endPoint = {atoi(argv[4]), atoi(argv[5])};

    std::vector<point> movepath = Bug.getPath(startPoint, endPoint);

    std::cout << "coloring the move path" << std::endl;
    for(auto &p : movepath)
    {
      img->setPixel8U(p.x, p.y, 100);
      std::cout << p.x << "\t" << p.y << std::endl;
    }
    std::cout << "Path Lenght:" << movepath.size() << std::endl;
    std::cout << "saving image..." << std::endl;
    // save image
    img->saveAsPGM("testout.pgm");

    // cleanup
    delete img;
}
