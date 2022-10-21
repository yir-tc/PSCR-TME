#pragma once

#include "Job.h"
#include "Color.h"
#include "Scene.h"
#include "Render.h"
#include "Barrier.h"

namespace pr {

class RenderJob : public Job {
    const Scene& scene;
	std::vector<Vec3D>& lights;

    Color *pixels;
    int x;
    int y;

    Barrier& barrier;

public:
    RenderJob(Scene& scene, std::vector<Vec3D>& lights, Color *pixels, 
            int x, int y,
            Barrier& barrier)
    : scene(scene), lights(lights), pixels(pixels), x(x), y(y), barrier(barrier) {}

    void run() {
	    // les points de l'ecran, en coordonnées 3D, au sein de la Scene.
    	// on tire un rayon de l'observateur vers chacun de ces points
    	const Scene::screen_t & screen = scene.getScreenPoints();
        
        for (int x = 0; x < scene.getHeight(); x++) {
            // le point de l'ecran par lequel passe ce rayon
            auto & screenPoint = screen[y][x];
            
            // le rayon a inspecter
            Rayon  ray(scene.getCameraPos(), screenPoint);
            int targetSphere = findClosestInter(scene, ray);

            if (targetSphere != -1) {
                const Sphere & obj = *(scene.begin() + targetSphere);
                // pixel prend la couleur de l'objet
                Color finalcolor = computeColor(obj, ray, scene.getCameraPos(), lights);
                // mettre a jour la couleur du pixel dans l'image finale.
                pixels[y*scene.getHeight() + x] = finalcolor;
            }
        }
        barrier.done(); 
    }
};

}