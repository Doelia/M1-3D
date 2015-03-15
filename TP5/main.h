#ifndef TOOLGL_H
#define TOOLGL_H

#include <stdio.h>      
#include <stdlib.h>     
#include <functional>
#include "../lib/Include.h"
#include "Sphere.h"
#include "Voxel.h"

void goAlgo(Sphere& s, Voxel v, float resolution) {
	if (s.isOn(v)) {
		if (resolution == 1) {
			v.draw(2);
		} else {
			Voxel* tab = v.decoupe();
			for (int i = 0; i < 8; ++i) {
				goAlgo(s, tab[i], resolution-1);
			}
		}
	} else {
		if (s.isIn(v))
			v.draw(1);
		else
			v.draw(3);
	}
}

void drawShpereAdaptatif(Point center, float r, float resolution) {
	Voxel v(&center, r*2);
	Sphere s(center, r);
	goAlgo(s, v, resolution);

}

void drawShpere(Point center, float r, float resolution) {
	int sizeRepere = r;

	int minX = -sizeRepere - center.getX();
	int minY = -sizeRepere - center.getY();
	int minZ = -sizeRepere - center.getZ();
	int maxX = sizeRepere + center.getX();
	int maxY = sizeRepere + center.getY();
	int maxZ = sizeRepere + center.getZ();

	float sizeVoxel = r*2.0f / resolution;
	//cout << "sizeVoxel = " << sizeVoxel << endl;

	Sphere s(center, r);

	for (float i = minX; i <= maxX; i += sizeVoxel) {
		for (float j = minY; j <= maxY; j += sizeVoxel) {
			for (float k = minZ; k <= maxZ; k += sizeVoxel) {
				//cout << i << ", " << j << ", " << k << endl;
				Point p(i,j,k);
				Voxel v(&p,1);
				if (s.isOn(v)) {
					Voxel v(&p,sizeVoxel);
					v.draw(1);
				}
			}
		}
	}
}




#endif
