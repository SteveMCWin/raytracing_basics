#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "bvh.h"
#include "material.h"
#include "sphere.h"
#include "texture.h"
#include "vec3.h"

#include <iostream>
#include <memory>

void random_spheres(){
  

  hittable_list world;

  auto checker = make_shared<checker_texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

  //nesto

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          auto center2 = center + vec3(0, random_double(0, 0.5), 0);
          world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  world = hittable_list(make_shared<bvh_node>(world));
  //Pravimo novu hittable listu koja jedini parametar ima world kao shared_ptr<bvh_node>
  //kada se pozove make_shared<bvh_node> pozove se i bvh_node(world) sto pravi celu
  //strukturu. Posto nova world lista ima samo jedan objekat u sebi a to je bvh pointer
  //kada se pozove hittable_list::hit time se poziva i bvh_node::hit 


  camera cam;

  cam.aspect_ratio      = 16.0 / 9.0;
  cam.image_width       = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth         = 20;

  cam.vfov     = 20;
  cam.lookfrom = point3(13,2,3);
  cam.lookat   = point3(0,0,0);
  cam.vup      = vec3(0,1,0);

  cam.defocus_angle = 0.6;
  cam.focus_dist    = 10.0;

  cam.render(world);

}

void two_spheres(){

  hittable_list world;

  auto checker = make_shared<checker_texture>(0.8, color(.2, .3, .1), color(.9, .9, .9));

  world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
  world.add(make_shared<sphere>(point3(0,  10, 0), 10, make_shared<lambertian>(checker)));

  camera cam;

  cam.aspect_ratio      = 16.0 / 9.0;
  cam.image_width       = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth         = 20;

  cam.vfov     = 20;
  cam.lookfrom = point3(13,2,3);
  cam.lookat   = point3(0,0,0);
  cam.vup      = vec3(0,1,0);

  cam.defocus_angle = 0.6;
  cam.focus_dist    = 10.0;

  cam.render(world);

}

void earth(){
  auto earth_texture = make_shared<image_texture>("earthmap.jpg");
  auto earth_surface = make_shared<lambertian>(earth_texture);
  auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

  camera cam;

  cam.aspect_ratio      = 16.0 / 9.0;
  cam.image_width       = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth         = 20;

  cam.vfov     = 20;
  cam.lookfrom = point3(0, 0, 12);
  cam.lookat   = point3(0,0,0);
  cam.vup      = vec3(0,1,0);

  cam.defocus_angle = 0.0;
  cam.focus_dist    = 10.0;

  cam.render(hittable_list(globe));

}

int main() {

  switch(3) {
    case 1: random_spheres(); break;
    case 2: two_spheres();    break;
    case 3: earth();          break;
  }
}
