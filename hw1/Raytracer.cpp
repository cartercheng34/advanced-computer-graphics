#include <iostream>
#include <fstream>
#include "algebra3.h"
#include <vector>
#include <sstream>
#include "Base.h"
#include "Camera.h"
#include "Raytracer.h"

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//output .PPM

struct Pixel {
	unsigned char R, G, B;  // Blue, Green, Red
};


class ColorImage {
	Pixel *pPixel;
	int xRes, yRes;
public:
	ColorImage();
	~ColorImage();
	void init(int xSize, int ySize);
	void clear(Pixel background);
	Pixel readPixel(int x, int y);
	void writePixel(int x, int y, Pixel p);
	void outputPPM(char *filename);
};

ColorImage::ColorImage()
{
	pPixel = 0;
}

ColorImage::~ColorImage()
{
	if (pPixel) delete[] pPixel;
	pPixel = 0;
}

void ColorImage::init(int xSize, int ySize)
{
	Pixel p = {0,0,0};
	xRes = xSize;
	yRes = ySize;
	pPixel = new Pixel[xSize*ySize];
	clear(p);
}

void ColorImage::clear(Pixel background)
{
	int i;
	
	if (! pPixel) return;
	for (i=0; i<xRes*yRes; i++) pPixel[i] = background;
}

Pixel ColorImage::readPixel(int x, int y)
{
	assert(pPixel); // die if image not initialized
	return pPixel[x + y*xRes];
}

void ColorImage::writePixel(int x, int y, Pixel p)
{
	assert(pPixel); // die if image not initialized
	pPixel[x + y*xRes] = p;
}

void ColorImage::outputPPM(char *filename)
{
    FILE *outFile = fopen(filename, "wb");

	assert(outFile); // die if file can't be opened

	fprintf(outFile, "P6 %d %d 255\n", xRes, yRes);
	fwrite(pPixel, 1, 3*xRes*yRes, outFile );

	fclose(outFile);
}

//raytracer part

bool Raytracer::parseInput(const string& fileName) {
    ifstream ifs(fileName.c_str() , ifstream::in);
    if(ifs.is_open() == false){
        cout<<"Cannot open file: \""<<fileName<<"\"!!"<<endl;
        return false;
    }
    char c;
    string tmp;
    vector<string> lines;
    while(ifs.get(c)){
        if(c == '\n'){
            lines.push_back(tmp);
            tmp.clear();
        }
        tmp += c;
    }
    char delim = ' ';
    for(size_t i = 0;i < lines.size();i++){
        stringstream ss(lines[i]);
        string item;
        vector<string> tokens;
        while (getline(ss, item, delim)) {
            tokens.push_back(item);
        }
        if (i >= 1) {
            tokens[0].erase(0,1);
        }

        if (tokens[0] == "E"){
            origin = vec3(stof(tokens[1]), stof(tokens[2]),stof(tokens[3]));
        }
        if (tokens[0] == "V"){
            unit_view_d = vec3(stof(tokens[1]), stof(tokens[2]),stof(tokens[3]));
            unit_view_d = unit_view_d.normalize();
        }
        if (tokens[0] == "F"){
            fov = stoi(tokens[1]);
        }
        if (tokens[0] == "R"){
            resolution = vec2(stof(tokens[1]), stof(tokens[2]));
        }
        if (tokens[0] == "S"){
            //auto tmp = new Sphere(vec3(stof(tokens[1]), stof(tokens[2]),stof(tokens[3])) , stof(tokens[4]));
            s_buffer.push_back(new Sphere(vec3(stof(tokens[1]), stof(tokens[2]),stof(tokens[3])) , stof(tokens[4])));
        }
        if (tokens[0] == "T"){            
            auto p0 = vec3(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
            auto p1 = vec3(stof(tokens[4]), stof(tokens[5]), stof(tokens[6]));
            auto p2 = vec3(stof(tokens[7]), stof(tokens[8]), stof(tokens[9]));
            //auto tmp =  new Triangle(p0, p1, p2)
            t_buffer.push_back(new Triangle(p0, p1, p2));
        }
    }
    cam = new Camera(origin, unit_view_d, resolution, fov);
}

void Raytracer::render(){
    vec3 UpLeft = cam->lookAtPoint - cam->xPixVector * (resolution[0]/2 - 0.5) 
                        + cam->yPixVector * (resolution[1]/2 - 0.5); // resolution is even

    ColorImage image;
	

    image.init(resolution[0], resolution[1]);

    cout << cam->xPixVector[0] << "," << cam->xPixVector[1] << "," << cam-> xPixVector[2] <<endl;
    cout << cam->yPixVector[0] << "," << cam->yPixVector[1] << "," << cam-> yPixVector[2] <<endl;
    cout << UpLeft[0] << "," << UpLeft[1] << "," << UpLeft[2] << endl;
    //cout << unit_view_d[0] << "," << unit_view_d[1] << "," << unit_view_d[2] <<endl;

    for (int w = 0; w < resolution[0]; w++){
        for (int h = 0 ; h < resolution[1]; h++){
            bool skip = false;
            Ray tmp(origin , (UpLeft + cam->xPixVector * w - cam->yPixVector * h - origin).normalize());
            Pixel p = {0,0,0};

            for(int n = 0; n < t_buffer.size();n++){
                if (t_buffer[n]->intersect(tmp).result == true){
                    skip = true;
                    p.R = 255;
                    p.G = 255;
                    p.B = 255;
                    image.writePixel(w, h, p);
                    break;                    
                }
            }
            if (skip == true){                
                continue;
            }
            
            for(int n = 0; n < s_buffer.size();n++){
                if (s_buffer[n]->intersect(tmp).result == true){
                    skip = true;
                    p.R = 255;
                    p.G = 255;
                    p.B = 255;
                    image.writePixel(w, h, p);
                    break;                    
                }
            }
            if (skip == true){
                continue;
            }
            p.R = 0;
            p.G = 0;
            p.B = 0;
            image.writePixel(w, h, p);
        }
    }
    image.outputPPM("hw1.ppm");
}






