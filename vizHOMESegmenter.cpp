// vizHOMESegmenter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

#include <string>
#include <vector>

using namespace glm;
using namespace std;

struct Object
{
	string name;
	mat4 matrix;
	FILE *fp;
};

vector<Object*> objects;

char *objectFilename = "102New.txt";
char *pointFilename = "D1_full_home_simplified_1cm.xyz";// "MU1_full_home_simplified_1cm.xyz";

int main(int argc, char** argv)
{
	if (argc == 3)
	{
		if (argv[1] != 0)
		{
			objectFilename = argv[1];
		}

		if (argv[2] != 0)
		{
			pointFilename = argv[2];
		}
	}

	FILE *objectFile = fopen(objectFilename, "r");

	if (!objectFile)
	{
		printf("file %f not found\n");
		return -1;
	}
	char name[1024];
	float m[16];
	memset(m, 0, sizeof(float) * 16);

	while (fscanf(objectFile, "%s %f  %f  %f  %f  %f  %f  %f  %f  %f  %f  %f  %f  %f  %f  %f  %f\n", &name[0], &m[0], &m[1], 
		&m[2], &m[3], &m[4], &m[5], &m[6], &m[7], &m[8], &m[9], &m[10], &m[11], &m[12], &m[13], &m[14], &m[15]) > 0)
	{
		Object *o = new Object();
		printf("%s\n", name);
		o->matrix = glm::make_mat4(m);
		float *valPtr = glm::value_ptr(o->matrix);
		/*for (int i = 0; i < 16; ++i)
		{
			printf("%f\n", valPtr[i]);
		}*/
		o->name = name;
		objects.push_back(o);
	}

	float x,y,z;
	int r,g,b;
	
	//create output files
	for (int i=0; i < objects.size(); i++)
	{
		objects[i]->fp = fopen((objects[i]->name + string(".xyz")).c_str(), "w");
	
		mat4 obj0Inv = glm::inverse(objects[i]->matrix);

		FILE *pointFile = fopen(pointFilename, "r");
		int index = 0;

		while (fscanf(pointFile,"%f %f %f %d %d %d", &x, &y, &z, &r, &g, &b) > 0)
		{
			vec4 v(x, y, z, 1);

			vec4 tp = obj0Inv*v;

			if ((fabs(tp.x) < .5) && (fabs(tp.y) < .5) &&(fabs(tp.z) < .5))
			{
				//printf("found point!\n");
				fprintf(objects[i]->fp, "%f %f %f %d %d %d\n", x, y, z, r, g, b);
			}

			index++;
			if (index%100000 == 0)
				printf("read %d points\n", index);
		}

		fclose(pointFile);
		fclose(objects[i]->fp);
	}

	return 0;
}

