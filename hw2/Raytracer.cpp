#include <iostream>
#include <fstream>
#include "algebra3.h"
#include <vector>
#include <sstream>
#include "Base.h"
#include "Camera.h"
#include "Raytracer.h"
#include <climits>

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

vec3 clamp(vec3 color){
    for(int i = 0 ; i < 3; i++){
        if(color[i] < 0.0)
            color[i] = 0.0;
        if(color[i] > 1.0)
            color[i] = 1.0;
    }
    return color;
}

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
    Material* tmp_m;
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
            o_buffer.push_back(new Sphere(vec3(stof(tokens[1]), stof(tokens[2]),stof(tokens[3])) , stof(tokens[4]), tmp_m));
        }
        if (tokens[0] == "T"){            
            auto p0 = vec3(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
            auto p1 = vec3(stof(tokens[4]), stof(tokens[5]), stof(tokens[6]));
            auto p2 = vec3(stof(tokens[7]), stof(tokens[8]), stof(tokens[9]));
            
            o_buffer.push_back(new Triangle(p0, p1, p2, tmp_m));
            
        }
        if (tokens[0] == "L"){   
                     
            l_buffer.push_back(new Light(vec3(stof(tokens[1]), stof(tokens[2]),stof(tokens[3]))));
        }
        if (tokens[0] == "M"){
             
            tmp_m = new Material(vec3(stof(tokens[1]), stof(tokens[2]), stof(tokens[3])), stof(tokens[4]), stof(tokens[5]), stof(tokens[6]), stof(tokens[7]), stof(tokens[8]), stof(tokens[9]), stof(tokens[10]));
            
        }
    }
    cam = new Camera(origin, unit_view_d, resolution, fov);
}

bool Raytracer::findFirstObject(Ray ray , Object** nearestObject, Intersection* nearestIntersection){
    bool found = false;
    auto distance = INT_MAX;
    for (size_t i = 0 ; i < o_buffer.size(); i++){
        Object* current_object = o_buffer[i];
        Intersection current_intersection = current_object->intersect(ray);

        if (current_intersection.result == true){
            found = true;
            double tmp_dis = (ray.get_origin() - current_intersection.intersect_Point).length();
            if (tmp_dis < distance){
                distance = tmp_dis;
                if (nearestObject)
                    *nearestObject = current_object;
                *nearestIntersection = current_intersection;
            }
        }
    }
    

    return found;
}

vec3 Raytracer::trace(Ray ray, int reflect_count){
    vec3 final_result = vec3(0,0,0); //background default color
    Object* object;
    Material* material;
    Intersection intersection;

    if (reflect_count > max_reflect)
        return final_result;
    if (findFirstObject(ray, &object, &intersection) == false)
        return final_result;
    //object = nearest object
    if (ray.get_direction() * intersection.unit_normal > 0)
        intersection.unit_normal = -intersection.unit_normal;

    material = object->material_;
    vec3 ambient = material->kAmbient * material->m_color;
    
    vec3 diffuse = vec3(0,0,0);
    vec3 specular = vec3(0,0,0);
    double distance = (intersection.intersect_Point - ray.get_origin()).length();
    vec3 reflected = ray.get_direction() - 2 * ray.get_direction() * intersection.unit_normal * intersection.unit_normal ;
    reflected = reflected.normalize();
    final_result += ambient;

    for (int i = 0 ; i < l_buffer.size() ; i++) {
        //shadowray
        Ray lightray(l_buffer[i]->light_pos, (intersection.intersect_Point - l_buffer[i]->light_pos).normalize());        
        
        Intersection light_intersection;
        Object* check_object;

        bool found = findFirstObject(lightray, &check_object, &light_intersection);
        if (found == false || (light_intersection.intersect_Point - intersection.intersect_Point ).length() > 0.0001) //if in shadow
            cout << "LUL" << endl;
        else {
            float diffusion = intersection.unit_normal * ((l_buffer[i]->light_pos - intersection.intersect_Point).normalize());
            if (diffusion > 0)                
                diffuse += prod(l_buffer[i]->color , material->m_color * material->kDiffuse * l_buffer[i]->intensity * diffusion / (distance * distance));
            vec3 tmp = intersection.intersect_Point;
            float cosLightReflect = reflected * (-lightray.get_direction());
            if (cosLightReflect > 0)                 
                specular += prod(l_buffer[i]->color , material->m_color * material->kSpecular * l_buffer[i]->intensity * pow (cosLightReflect, material->specularity) / (distance * distance));
        }
        
    }
    //cout << "???:" << material->reflectionFactor << endl;
    vec3 reflection_color = trace(Ray(intersection.intersect_Point, reflected.normalize()) , ++reflect_count);
    final_result = ambient + diffuse + specular + material->reflectionFactor * reflection_color;
    counter = 0;

    final_result = clamp(final_result);

    return final_result;

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
            

            for(int n = 0; n < o_buffer.size();n++){
                if (o_buffer[n]->intersect(tmp).result == true){
                    vec3 color = trace(tmp, counter);
                    cout << color[0]<< "," << color[1] << "," << color[2] << endl;
                    p.R = color[0] * 255;
                    p.G = color[1] * 255;
                    p.B = color[2] * 255;
                    image.writePixel(w, h, p);
                    break;                    
                }
            }
            
            /*
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
            */            
            
            
        }
    }
    image.outputPPM("hw2.ppm");
}






