/*

Copyright (c) 2015, Eric Arnebäck(arnebackeric@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:



The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <float.h>

using std::string;
using std::cout;
using std::endl;
using std::cerr;
using std::ifstream;
using std::vector;
using std::stringstream;
using std::stof;
using std::invalid_argument;
using std::ostream;

struct Vector {

public:
    float x,y,z;

    Vector(float x_, float y_, float z_) {
	x = x_;
	y = y_;
	z = z_;
    }

    Vector() {
	x = y = z = 0;
    }

    static float Dot(const Vector& v1, const Vector& v2) {
	return
	    v1.x * v2.x +
	    v1.y * v2.y +
	    v1.z * v2.z;
    }

    friend ostream& operator<<(ostream& os, const Vector& v) {
	os << "(" << v.x << "," << v.y << "," << v.z << ")";
	return os;
    }
};

string StripFileExtension(const string& str);
void PrintHelp();

// split a string on whitespace character.
vector<string> Split(string str);

int main(int argc, char *argv[] ) {

    /*
      Parse command line arguments:
     */

    if(argc < 2) { // not enough arguments.
	PrintHelp();
	exit(1);
    }

    for (int i = 1; i < argc; i++) {

	if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0  ) {
	    PrintHelp();
	    exit(0);
	}
    }

    // last arguent is input file
    const string inputFile = string(argv[argc-1]);




    cout << "input file: " << inputFile << endl;

    ifstream file (inputFile);

    if(file.fail()) {
	cerr << "Error: " << strerror(errno) << endl;
    }

    string line;

    Vector axes[] = {
	Vector(1,0,0),
	Vector(0,1,0),
	Vector(0,0,1),
    };

    Vector vmin[3];
    Vector vmax[3];

    float minproj[3] = {FLT_MAX, FLT_MAX, FLT_MAX };
    float maxproj[3] = {-FLT_MAX, -FLT_MAX, -FLT_MAX };

    while(getline(file, line)) {

	// we are only interested in the vertices of the model.
	// all other information(such as normals) we ignore.
	if(line[0] != 'v' || line[1] != ' ')
	    continue;

	vector<string> tokens = Split(line);

	if(tokens.size() != 4) {
	    cerr << "Invalid obj-file: every vertex line must have three integers:" << endl;
	    cerr << line << endl;
	    exit(1);
	}

	Vector pt;

	try {
	    pt.x = stof(tokens[1], NULL);
	    pt.y = stof(tokens[2], NULL);
	    pt.z = stof(tokens[3], NULL);

	} catch(const invalid_argument& e) {
	    cerr << "Invalid obj-file: found vertex with invalid numbers:" << endl;
	    cerr << line << endl;
	    exit(1);
	}


	for(int iaxis = 0; iaxis < 3; ++iaxis) {

	    Vector axis = axes[iaxis];

	    float proj = Vector::Dot(pt, axis);

	    if (proj < minproj[iaxis]) {
		minproj[iaxis] = proj;

		vmin[iaxis] = pt;
	    }
	    // Keep track of most distant point along direction vector
	    if (proj > maxproj[iaxis]) {
		maxproj[iaxis] = proj;
		vmax[iaxis] = pt;

//		*imax = i;
	    }

	}

    }

    cout << "xmin " << vmin[0] << endl;
    cout << "xmax " << vmax[0] << endl;

    cout << "ymin " << vmin[1] << endl;
    cout << "ymax " << vmax[1] << endl;

    cout << "zmin " << vmin[2] << endl;
    cout << "zmax " << vmax[2] << endl;


}

string StripFileExtension(const string& str) {
    size_t last_dot = str.find_last_of(".");

    return str.substr(0,last_dot);
}

void PrintHelp() {
    printf("Usage:\n");
    printf("aabb_create [FLAGS] input-file\n\n");

//    printf("Flags:\n");
//    printf("\t-h,--help\t\tPrint this message\n");
//    printf( "\t-fs,--font-size\t\tFont size. Default value: %d\n", FONT_SIZE_DEFALT );

}

vector<string> Split(string str) {
    string buffer;
    stringstream ss(str);

    vector<string> tokens;

    while (ss >> buffer)
        tokens.push_back(buffer);

    return tokens;
}
