#ifndef TOOLGL_H
#define TOOLGL_H

#include <stdio.h>      
#include <stdlib.h>     
#include <functional>
#include "../lib/Include.h"
#include "Figures.h"
#include "Voxel.h"
#include <Vector>

void drawVoxels(vector<Voxel> tab) {
	for (auto v : tab) {
		v.draw(v.flag);
	}
}

void goAlgo(std::function<int(Voxel)> f, Voxel v, float resolution, vector<Voxel>* voxels) {
	if (f(v) == 1) { // IS ON
		if (resolution == 1) {
			v.flag = 1;
			voxels->push_back(v);
		} else {
			Voxel* tab = v.decoupe();
			for (int i = 0; i < 8; ++i) {
				goAlgo(f, tab[i], resolution-1, voxels);
			}
		}
	} else {
		if (f(v) == 2) // IS IN
			v.flag = 2;
		else // IS OUT
			v.flag = 3;
		voxels->push_back(v);
	}
}

std::function<int(Voxel)> generateFunctionUnique(Figure* s) {
	auto prototype = [] (Figure* c) -> std::function<int(Voxel)>
	{ return ([=] (Voxel v) {
		if (c->isOn(v))
			return 1;
		else if (c->isIn(v))
			return 2;
		else if (c->isOut(v))
			return 3;
		return 0;
	}); };
	auto f = prototype(s);
	return f;
}

std::function<int(Voxel)> generateFunctionIntersect(Figure* s1, Figure* s2) {
	auto prototype = [] (Figure* c1, Figure* c2) -> std::function<int(Voxel)>
	{ return ([=] (Voxel v) {
		if (c1->isOn(v) && c2->isOn(v))
			return 1;
		else if (c1->isIn(v) && c2->isIn(v))
			return 2;
		else if (c1->isOut(v) && c2->isOut(v))
			return 3;
		return 0;
	}); };
	return prototype(s1, s2);
}

std::function<int(Voxel)> generateFunctionUnion(Figure* s1, Figure* s2) {
	auto prototype = [] (Figure* c1, Figure* c2) -> std::function<int(Voxel)>
	{ return ([=] (Voxel v) {
		if (c1->isOn(v) || c2->isOn(v))
			return 1;
		else if (c1->isIn(v) || c2->isIn(v))
			return 2;
		else if (c1->isOut(v) || c2->isOut(v))
			return 3;
		return 0;
	}); };
	return prototype(s1, s2);
}

std::function<int(Voxel)> generateFunctionMinus(Figure* s1, Figure* s2) {
	auto prototype = [] (Figure* c1, Figure* c2) -> std::function<int(Voxel)>
	{ return ([=] (Voxel v) {
		if (c1->isOn(v) && c2->isOn(v))
			return 1;
		else if (c1->isIn(v) && c2->isIn(v))
			return 2;
		else if (c1->isOut(v) && c2->isOut(v))
			return 3;
		return 0;
	}); };
	return prototype(s1, s2);
}


vector<Voxel>* octreeMethod(std::function<int(Voxel)> f, float resolution, Voxel v) {
	vector<Voxel>* voxels = new vector<Voxel>();
	goAlgo(f, v, resolution, voxels);
	return voxels;
}

void intesectSphereCilynder(Point center, float rSphere, float rCilyndre, Vector axe, float resolution) {
	int sizeVoxel = (rSphere > rCilyndre) ? rSphere : rCilyndre;

	Voxel v(&center, sizeVoxel*3);
	Sphere s1(center, rSphere);
	Cilynder s2(center, axe, rCilyndre);
	vector<Voxel>* voxels = octreeMethod(generateFunctionMinus(&s2, &s1), resolution, v);
	//vector<Voxel>* voxels = octreeMethod(generateFunctionIntersect(&s1, &s2), resolution, v);
	//vector<Voxel>* voxels = octreeMethod(generateFunctionUnion(&s1, &s2), resolution, v);
	drawVoxels(*voxels);
	delete(voxels);
}

void drawShpereAdaptatif(Point center, float r, float resolution) {
	Voxel v(&center, r*2);
	Sphere s(center, r);
	vector<Voxel>* voxels = octreeMethod(generateFunctionUnique(&s), resolution, v);
	drawVoxels(*voxels);
	delete(voxels);
}

void drawCilrindreAdaptatif(Point center, float r, Vector axe, float resolution) {
	Voxel v(&center, r*2);
	Cilynder s(center, axe, r);
	vector<Voxel>* voxels = octreeMethod(generateFunctionUnique(&s), resolution, v);
	drawVoxels(*voxels);
	delete(voxels);
}

#endif
