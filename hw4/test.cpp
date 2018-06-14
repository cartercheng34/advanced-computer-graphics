#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "algebra3.h"
#include "Base.h"
#include "Raytracer.h"
#include "Camera.h"
#include <ctime>

using namespace std;

int main(){
    string fileName;
    cout << "filename : ";
    cin >> fileName;

    clock_t start;
    double duration;

    start = clock();
    //fileName = "example.txt";
    /*
    ifstream ifs(fileName.c_str() , ifstream::in);
    if(ifs.is_open() == false){
        cout<<"Cannot open design \""<<fileName<<"\"!!"<<endl;
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
    
    for(int i = 0;i < lines.size();i++){
        stringstream ss(lines[i]);
        string item;
        vector<string> tokens;
        while (getline(ss, item, delim)) {
            tokens.push_back(item);
        }
        if (i>=1) 
            tokens[0].erase(0,1);

        if (tokens[0] == ""){
            //const float x = stof(tokens[1]);
            //const float y = stof(tokens[2]);
            //const float z = stof(tokens[3]);
            vec3 haha;
            haha = vec3(stof(tokens[1]), stof(tokens[2]),stof(tokens[3]));
            
            //cout<<"abc"<<endl;
            
        }
        if (tokens[0] == "S"){
            //cout<<"haha"<<endl;
            auto tmp = Sphere(vec3(stof(tokens[1]), stof(tokens[2]),stof(tokens[3])) , stof(tokens[4]));
            
        }
    }
    */
    Raytracer rayMgr;
    rayMgr.parseInput(fileName);
    rayMgr.render();

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    cout << "elapsed time: " << duration << "sec" <<endl;
    return 0;
}