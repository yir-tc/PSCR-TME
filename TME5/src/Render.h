#pragma once

#include "Scene.h"
#include "Color.h"
#include "Sphere.h"
#include "Vec3D.h"
#include "Rayon.h"

namespace pr {

int findClosestInter(const Scene & scene, const Rayon & ray);
Color computeColor(const Sphere & obj, const Rayon & ray, const Vec3D & camera, std::vector<Vec3D> & lights);

}