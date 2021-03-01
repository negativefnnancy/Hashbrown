/* display test
 *
 * this is meant to be a low level test of the vector display!!!
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <SDL2/SDL.h>

#include <hashbrown/display/display.h>

#define WIDTH 480
#define HEIGHT 360

#define REALTIME false
#define FPS 60 /* non real time */
#define EXPORT true /* export all the frames to "images/xxxx.png" */

#define LINE_SPEED 100000 /* normalized device units per second */

/* TODO consolodate this demo with the display.c demo */

typedef struct app_t {

    /* SDL stuff */
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    /* the simulated display */
    display_t display;

    /* time of init */
    double time_init;

    /* frame counter */
    unsigned long int frame;

    /* scanlines */
    double x, y, dx, dy;

} app_t;

typedef struct material_t {

    double reflectance;
    double emittance;
    double scattering;

} material_t;

material_t material_make (double reflectance, double emittance, double scattering) {

    material_t material;
    material.reflectance = reflectance;
    material.emittance   = emittance;
    material.scattering  = scattering;
    return material;
}

typedef struct vec3_t {

    double x, y, z;

} vec3_t;

vec3_t vec3_make (double x, double y, double z) {

    vec3_t vector;
    vector.x = x;
    vector.y = y;
    vector.z = z;
    return vector;
}

typedef struct vec4_t {

    double x, y, z, w;

} vec4_t;

vec4_t vec4_make (double x, double y, double z, double w) {

    vec4_t vector;
    vector.x = x;
    vector.y = y;
    vector.z = z;
    vector.w = w;
    return vector;
}

double vec4_dot (vec4_t a, vec4_t b) {

    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

vec4_t vec4_multiply (vec4_t a, vec4_t b) {

    return vec4_make (a.x * b.x,
                      a.y * b.y,
                      a.z * b.z,
                      a.w * b.w);
}

typedef struct mat4_t {

    vec4_t x, y, z, w;

} mat4_t;

mat4_t mat4_make (vec4_t x, vec4_t y, vec4_t z, vec4_t w) {

    mat4_t matrix;
    matrix.x = x;
    matrix.y = y;
    matrix.z = z;
    matrix.w = w;
    return matrix;
}

mat4_t mat4_transpose (mat4_t matrix) {

    vec4_t x = vec4_make (matrix.x.x, matrix.y.x, matrix.z.x, matrix.w.x);
    vec4_t y = vec4_make (matrix.x.y, matrix.y.y, matrix.z.y, matrix.w.y);
    vec4_t z = vec4_make (matrix.x.z, matrix.y.z, matrix.z.z, matrix.w.z);
    vec4_t w = vec4_make (matrix.x.w, matrix.y.w, matrix.z.z, matrix.w.w);
    return mat4_make (x, y, z, w);
}

mat4_t mat4_multiply (mat4_t a, mat4_t b) {

    mat4_t b_transpose = mat4_transpose (b);
    return mat4_make (vec4_multiply (a.x, b_transpose.x),
                      vec4_multiply (a.y, b_transpose.y),
                      vec4_multiply (a.z, b_transpose.z),
                      vec4_multiply (a.w, b_transpose.w));
}

vec4_t mat4_multiply_vec4 (mat4_t matrix, vec4_t vector) {

    return vec4_make (vec4_dot (matrix.x, vector),
                      vec4_dot (matrix.y, vector),
                      vec4_dot (matrix.z, vector),
                      vec4_dot (matrix.w, vector));
}

mat4_t mat4_identity () {

    return mat4_make (vec4_make (1, 0, 0, 0),
                      vec4_make (0, 1, 0, 0),
                      vec4_make (0, 0, 1, 0),
                      vec4_make (0, 0, 0, 1));
}

mat4_t make_rotation_x (double degrees) {

    return mat4_make (vec4_make (0, 0, 0, 0),
                      vec4_make (0, cos (degrees), -sin (degrees), 0),
                      vec4_make (0, sin (degrees), cos (degrees), 0),
                      vec4_make (0, 0, 0, 1));
}

mat4_t make_rotation_y (double degrees) {

    return mat4_make (vec4_make (cos (degrees), 0, sin (degrees), 0),
                      vec4_make (0, 0, 0, 0),
                      vec4_make (-sin (degrees), 0, cos (degrees), 0),
                      vec4_make (0, 0, 0, 1));
}

mat4_t make_rotation_z (double degrees) {

    return mat4_make (vec4_make (cos (degrees), -sin (degrees), 0, 0),
                      vec4_make (sin (degrees), cos (degrees), 0, 0),
                      vec4_make (0, 0, 1, 0),
                      vec4_make (0, 0, 0, 1));
}

typedef struct sphere_t {

    vec3_t origin;
    double radius;

} sphere_t;

sphere_t sphere_make (vec3_t origin, double radius) {

    sphere_t sphere;
    sphere.origin = origin;
    sphere.radius = radius;
    return sphere;
}

typedef struct plane_t {

    vec3_t origin;
    vec3_t direction;

} plane_t;

plane_t plane_make (vec3_t origin, vec3_t direction) {

    plane_t plane;
    plane.origin = origin;
    plane.direction = direction;
    return plane;
}

typedef struct box_t {

    vec3_t min;
    vec3_t max;
    mat4_t transform;

} box_t;

box_t box_make (vec3_t min, vec3_t max, mat4_t transform) {

    box_t box;
    box.min = min;
    box.max = max;
    box.transform = transform;
    return box;
}

void box_rotate (box_t *box, double x, double y, double z) {

    mat4_t rotation_x = make_rotation_x (x);
    mat4_t rotation_y = make_rotation_y (y);
    mat4_t rotation_z = make_rotation_z (z);

    box->transform = mat4_multiply (box->transform, rotation_x);
    box->transform = mat4_multiply (box->transform, rotation_y);
    box->transform = mat4_multiply (box->transform, rotation_z);
}

typedef struct ray3_t {

    vec3_t origin;
    vec3_t direction; /* unit vector */

} ray3_t;

double vec3_dot (vec3_t a, vec3_t b) {

    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double vec3_length (vec3_t vector) {

    return sqrt (vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

extern double linear_interpolate (double a, double b, double amount);

vec3_t vec3_interpolate (vec3_t a, vec3_t b, double amount) {

    return vec3_make (linear_interpolate (a.x, b.x, amount),
                      linear_interpolate (a.y, b.y, amount),
                      linear_interpolate (a.z, b.z, amount));
}

vec3_t vec3_add (vec3_t a, vec3_t b) {

    return vec3_make (a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3_t vec3_subtract (vec3_t a, vec3_t b) {

    return vec3_make (a.x - b.x, a.y - b.y, a.z - b.z);
}

vec3_t vec3_multiply (vec3_t a, vec3_t b) {

    return vec3_make (a.x * b.x, a.y * b.y, a.z * b.z);
}

vec3_t vec3_divide (vec3_t a, vec3_t b) {

    return vec3_make (a.x / b.x, a.y / b.y, a.z / b.z);
}

vec3_t vec3_multiply_scalar (vec3_t vector, double scalar) {

    return vec3_make (vector.x * scalar, vector.y * scalar, vector.z * scalar);
}

vec3_t vec3_divide_scalar (vec3_t vector, double scalar) {

    return vec3_make (vector.x / scalar, vector.y / scalar, vector.z / scalar);
}

vec3_t vec3_subtract_scalar (vec3_t vector, double scalar) {

    return vec3_make (vector.x - scalar, vector.y - scalar, vector.z - scalar);
}

vec3_t vec3_unit (vec3_t vector) {

    return vec3_divide_scalar (vector, vec3_length (vector));
}

vec3_t vec3_direction (vec3_t origin, vec3_t target) {

    return vec3_unit (vec3_subtract (target, origin));
}

ray3_t ray3_make_direction (vec3_t origin, vec3_t direction) {

    ray3_t ray;
    ray.origin    = origin;
    ray.direction = direction;
    return ray;
}

ray3_t ray3_make (vec3_t origin, vec3_t target) {

    return ray3_make_direction (origin, vec3_direction (origin, target));
}

extern double noise ();

bool rand_bool (double probability) {

    return noise () < probability;
}

bool solve_quadratic (double a, double b, double c, double *x1, double *x2) {

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return false;
    double sqrt_discriminant = sqrt (discriminant);
    *x1 = -0.5 * (b + sqrt_discriminant) / a;
    *x2 = -0.5 * (b - sqrt_discriminant) / a;
    return true;
}

bool ray_intersect_sphere (ray3_t ray, void *data, double *distance, vec3_t *point, vec3_t *normal) {

    sphere_t *sphere = (sphere_t *) data;

    /* find intersection points */
    vec3_t relative_origin = vec3_subtract (ray.origin, sphere->origin);
    double a = vec3_dot (ray.direction,   ray.direction);
    double b = vec3_dot (relative_origin, ray.direction) * 2;
    double c = vec3_dot (relative_origin, relative_origin) - sphere->radius * sphere->radius;
    double x1, x2;
    if (solve_quadratic (a, b, c, &x1, &x2)) {

        /* hit the sphere */

        /* get the nearest intersection */
        *distance = x1 < x2 ? x1 : x2;
        if (*distance <= 0.01)
            return false; /* it was hit from behind!! :scream: */

        /* get the point of intersection */
        *point = vec3_add (ray.origin, vec3_multiply_scalar (ray.direction, *distance));

        /* get the surface normal */
        *normal = vec3_direction (sphere->origin, *point);

        /* we hit the sphereeeeeeeee */
        return true;

    } else
        /* we hit the skyyyy */
        return false;
}

bool ray_intersect_plane (ray3_t ray, void *data, double *distance, vec3_t *point, vec3_t *normal) {

    plane_t *plane = (plane_t *) data;

    double denominator = vec3_dot (ray.direction, plane->direction);
    if (denominator > 0)
        return false;

    *distance = vec3_dot (vec3_subtract (plane->origin, ray.direction), plane->direction) / denominator;

    if (*distance < 0)
        return false;

    *point = vec3_add (ray.origin, vec3_multiply_scalar (ray.direction, *distance));
    *normal = plane->direction;

    return true;
}

bool ray_intersect_box (ray3_t ray, void *data, double *distance, vec3_t *point, vec3_t *normal) {

    box_t *box = (box_t *) data;

    vec3_t invdir = vec3_divide (vec3_make (1, 1, 1), ray.direction);
    int sign[3];
    sign[0] = (invdir.x < 0);
    sign[1] = (invdir.y < 0);
    sign[2] = (invdir.z < 0);

    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    vec3_t bounds[2];
    bounds[0] = box->min;
    bounds[1] = box->max;

    tmin = (bounds[sign[0]].x - ray.origin.x) * invdir.x;
    tmax = (bounds[1-sign[0]].x - ray.origin.x) * invdir.x;
    tymin = (bounds[sign[1]].y - ray.origin.y) * invdir.y;
    tymax = (bounds[1-sign[1]].y - ray.origin.y) * invdir.y;

    if ((tmin > tymax) || (tymin > tmax))
        return false;
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    tzmin = (bounds[sign[2]].z - ray.origin.z) * invdir.z;
    tzmax = (bounds[1-sign[2]].z - ray.origin.z) * invdir.z;

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;
    if (tzmin > tmin)
        tmin = tzmin;
    if (tzmax < tmax)
        tmax = tzmax;

    *distance = tmin < tmax ? tmin : tmax;
    *point = vec3_add (ray.origin, vec3_multiply_scalar (ray.direction, *distance));

    return true;
}

#define N_OBJECTS 4

typedef bool intersect_function_t (ray3_t, void *, double *, vec3_t *, vec3_t *);

typedef struct object_t {

    intersect_function_t *intersect;
    void *data;

} object_t;

object_t object_make (intersect_function_t *intersect, void *data) {

    object_t object;
    object.intersect = intersect;
    object.data = data;
    return object;
}

bool ray_intersect_scene (ray3_t ray, vec3_t *point, vec3_t *normal, material_t *material, double time) {

    size_t i;

    /* the spheres in the scene */
    sphere_t spheres[3];
    spheres[0] = sphere_make (vec3_make (1 * cos (0.25 * time * M_PI * 2 + M_PI * 1.0/4), -0.5, 1 * sin (0.25 * time * M_PI * 2 + M_PI * 1.0/4) + 1), 0.1);
    spheres[1] = sphere_make (vec3_make (1 * cos (0.25 * 3.0 / 2 * time * M_PI * 2 + M_PI * 5.0/4), 0.5, 1 * sin (0.25 * 3.0 / 2 * time * M_PI * 2 + M_PI * 5.0/4) + 1), 0.1);
    spheres[2] = sphere_make (vec3_make (0, 0, 1), 1);

//    mat4_t transform = make_rotation_y (sin (time * M_PI * 2 * 0.25));
//    box_t box = box_make (vec3_make (-0.7, -0.7, 0.3), vec3_make (0.7, 0.7, 1.7), transform);

    /* floor plane */
    plane_t plane = plane_make (vec3_make (0, 1, 0), vec3_make (0, -1, 0));

    /* objects on screen */
    object_t objects[N_OBJECTS];
    objects[0] = object_make (&ray_intersect_sphere, &spheres[0]);
    objects[1] = object_make (&ray_intersect_sphere, &spheres[1]);
    objects[2] = object_make (&ray_intersect_sphere, &spheres[2]);
    objects[3] = object_make (&ray_intersect_plane,  &plane);

    /* materials */
    material_t materials[N_OBJECTS] = {
        //material_make (0, (sin (time * M_PI * 4) + 1) / 2, 0),
        material_make (0, 1, 0),
        material_make (0, 1, 0),
        material_make (0.75, 0, 0.125),
        material_make (0.5, 0, 0.75),
    };

    /* the intersections and normals */
    double distances[N_OBJECTS];
    vec3_t    points[N_OBJECTS];
    vec3_t   normals[N_OBJECTS];
    bool     results[N_OBJECTS];

    for (i = 0; i < N_OBJECTS; i++) {
        results[i] = objects[i].intersect (ray, objects[i].data, &distances[i], &points[i], &normals[i]);
    }

    /* calculate closest */
    bool result = false;
    double distance;
    for (i = 0; i < N_OBJECTS; i++) {

        if (results[i]) {

            /* dirty hak */
            if (i == 3) {

                vec3_t p = points[i];
                int x = p.x * 2;
                int y = p.z * 2;
                int s = x + y;
                while (s < 0)
                    s += 2;
                s %= 2;
                int f = 0;
                if (p.x < 0) f = !f;
                if (p.y < 0) f = !f;
                materials[i].reflectance *= s == f;
            }

            if (result) {

                if (distances[i] < distance) {

                    distance  = distances[i];
                    *point    = points[i];
                    *normal   = normals[i];
                    *material = materials[i];
                }

            } else {

                distance  = distances[i];
                *point    = points[i];
                *normal   = normals[i];
                *material = materials[i];
                result = true;
            }
        }
    }
    return result;
}

void print_vector (vec3_t vector) {

    printf ("%lf, %lf, %lf\n", vector.x, vector.y, vector.z);
}

vec3_t reflect (vec3_t normal, vec3_t incoming_direction, double scattering) {

    double dot_product = vec3_dot (incoming_direction, normal);
    vec3_t reflection_specular = vec3_subtract (incoming_direction,
                                                vec3_multiply_scalar (normal,
                                                                      dot_product * 2));
    vec3_t reflection_diffuse
        = vec3_unit (vec3_add (normal,
                               vec3_make (noise () * 2 - 1,
                                          noise () * 2 - 1,
                                          noise () * 2 - 1)));
    /*
    printf ("%lf\n", dot_product);
    print_vector (normal); 
    print_vector (incoming_direction); 
    print_vector (reflection_specular); 
    print_vector (reflection_diffuse); 
    puts ("");
    */
    return vec3_interpolate (reflection_specular, reflection_diffuse, scattering);
}

double ray_cast (ray3_t ray, double time) {

    /*
    reflect (vec3_unit (vec3_make (0, -1, 0)),
             vec3_unit (vec3_make (1, 0, 0)),
             0);
    exit (1);
    */

    /* intersect with the scene */
    vec3_t point, normal;
    material_t material;
    if (ray_intersect_scene (ray, &point, &normal, &material, time)) {

        if (material.reflectance) {

            ray3_t reflection = ray3_make_direction (point,
                                                     reflect (normal,
                                                              ray.direction,
                                                              material.scattering));
            return ray_cast (reflection, time) * material.reflectance;
            //return material.reflectance * vec3_dot (reflection.direction, vec3_make (1, 0, 0));
            /* TODO max recursion */

        } else /* TODO proper combine probabilities */
            return material.emittance;

    } else
        return vec3_dot (ray.direction, vec3_unit (vec3_make (1, -1, 0))) * 0.125 + 0.125;
}

void sample_path (app_t *app, double time, double *x, double *y) {

    double ratio = app->display.width / (double) app->display.height;

    ///* randomly sample a point on the screen */
    ///* normalized device coordinates */
    //*x = (noise () * 2 - 1) * ratio;
    //*y = noise () * 2 - 1;

    /* bouncing scanlines */
    /*
    *x = app->x;
    *y = app->y;
    app->x += app->dx * SECONDS_PER_ELECTRON;
    app->y += app->dy * SECONDS_PER_ELECTRON;

    while (true) {

        const double eps = 0.000001;

        if (app->x < -ratio - eps) {

            app->x -= 2 * (app->x + ratio);
            app->dx *= -1;
        }
        else if (app->y < -1 - eps) {

            app->y -= 2 * (app->y + 1);
            app->dy *= -1;
        }
        else if (app->x > ratio + eps) {

            app->x += 2 * (ratio - app->x);
            app->dx *= -1;
        }
        else if (app->y > 1 + eps) {

            app->y += 2 * (1 - app->y);
            app->dy *= -1;
        }
        else break;
    }
    */

    /* standard scanlines */
    /*
    *x = app->x;
    *y = app->y;
    app->x += app->dx * SECONDS_PER_ELECTRON;
    app->y += app->dy * SECONDS_PER_ELECTRON;

    while (true) {

        const double eps = 0.000001;

        if (app->x > ratio + eps) {

            app->x -= 2 * ratio;
            app->y += 0.01;
        }
        else if (app->y > 1 + eps) {

            app->y -= 2;
        }
        else break;
    }
    */

    /* random bounce */
    *x = app->x;
    *y = app->y;
    app->x += app->dx * SECONDS_PER_ELECTRON;
    app->y += app->dy * SECONDS_PER_ELECTRON;

    while (true) {

        const double eps = 0.000001;

        if (app->x < -ratio - eps) {

            app->x -= 2 * (app->x + ratio);
            app->dx = noise () * 2;
            app->dy = noise () * 2 - 1;
            double l = sqrt (app->dx * app->dx + app->dy * app->dy);
            app->dx *= LINE_SPEED / l;
            app->dy *= LINE_SPEED / l;
        }
        else if (app->y < -1 - eps) {

            app->y -= 2 * (app->y + 1);
            app->dx = noise () * 2 - 1;
            app->dy = noise () * 2;
            double l = sqrt (app->dx * app->dx + app->dy * app->dy);
            app->dx *= LINE_SPEED / l;
            app->dy *= LINE_SPEED / l;
        }
        else if (app->x > ratio + eps) {

            app->x += 2 * (ratio - app->x);
            app->dx = noise () * 2 - 2;
            app->dy = noise () * 2 - 1;
            double l = sqrt (app->dx * app->dx + app->dy * app->dy);
            app->dx *= LINE_SPEED / l;
            app->dy *= LINE_SPEED / l;
        }
        else if (app->y > 1 + eps) {

            app->y += 2 * (1 - app->y);
            app->dx = noise () * 2 - 1;
            app->dy = noise () * 2 - 2;
            double l = sqrt (app->dx * app->dx + app->dy * app->dy);
            app->dx *= LINE_SPEED / l;
            app->dy *= LINE_SPEED / l;
        }
        else break;
    }
}

void callback (display_t *display,
               unsigned long int i_electron,
               unsigned long int delta_electrons,
               void *data) {

    app_t *app = (app_t *) data;

    /* circle */
    /*
    double angle = noise () * M_PI * 2;
    double radius = app->display.width * sqrt (noise ()) / 4;
    app->display.x = cos (angle) * radius + app->display.width / 2;
    app->display.y = sin (angle) * radius + app->display.height / 2;
    app->display.gun_enabled = true;
    */

    /* path tracing */
    double time = (display->i_electron + i_electron) * SECONDS_PER_ELECTRON;

    /* point on screen to sample */
    double eye_z = 2 * sin (time * M_PI * 2 / 8) - 3;
    double x, y;
    sample_path (app, time, &x, &y);

    /* cast a ray from the eye to the point on screen */
    ray3_t ray = ray3_make (vec3_make (0, 0, eye_z), vec3_make (x, y, 0));
    app->display.intensity = ray_cast (ray, time);

    /* convert to screen coordinates */
    app->display.y = (y * app->display.height + app->display.height) / 2;
    app->display.x = (x * app->display.height + app->display.width)  / 2;
}

int update_size (app_t *app) {

    int width, height;
    SDL_GetWindowSize (app->window, &width, &height);

    if (app->texture)
        SDL_DestroyTexture (app->texture);

    if (!(app->texture = SDL_CreateTexture (app->renderer,
                                            SDL_PIXELFORMAT_ABGR8888,
                                            SDL_TEXTUREACCESS_STREAMING,
                                            width,
                                            height))) {

        fprintf (stderr, "Could not create texture: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    if (app->display.buffer_output)
        display_deinit (&app->display);

    display_init (&app->display, width, height, callback, app);

    app->time_init = SDL_GetTicks () / 1000.0;
    app->frame = 0;

    return EXIT_FAILURE;
}

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void export_frame (app_t *app) {

    char path[256];
    sprintf (path, "images/%04d.png", app->frame);
    printf ("Writing image to %s (t = %lf)\n", path, (double) app->frame / FPS);
    if (stbi_write_png (path,
                        app->display.width,
                        app->display.height,
                        4,
                        app->display.buffer_output,
                        4 * app->display.width) == 0) {

        fprintf (stderr, "Failed to write image to %s\n", path);
        exit (EXIT_FAILURE);
    }
}

int main (int argc, char **argv) {

    app_t app;
    SDL_Event event;

    memset (&app, 0, sizeof (app));
    app.dx = noise () * 2 - 1;
    app.dy = noise () * 2 - 1;
    double l = sqrt (app.dx * app.dx + app.dy * app.dy);
    app.dx *= LINE_SPEED / l;
    app.dy *= LINE_SPEED / l;
    //app.dx = LINE_SPEED;
    //app.dy = 0;

    /* initialize SDL */
    if (SDL_Init (SDL_INIT_VIDEO) < 0) {

        fprintf (stderr, "Could not initialize SDL: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    /* create the window */
    if (!(app.window = SDL_CreateWindow ("Hashbrown Raster Test",
                                         SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         WIDTH,
                                         HEIGHT,
                                         SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE))) {

        fprintf (stderr, "Could not create SDL window: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    /* create the renderer */
    if (!(app.renderer = SDL_CreateRenderer (app.window,
                                             -1,
                                             SDL_RENDERER_ACCELERATED |
                                             SDL_RENDERER_PRESENTVSYNC))) {

        fprintf (stderr, "Could not create SDL renderer: %s\n", SDL_GetError ());
        return EXIT_FAILURE;
    }

    /* create the render texture and init the display */
    update_size (&app);

    /* run loop */
    while (true) {

        /* handle events */
        while (SDL_PollEvent (&event) != 0)
            switch (event.type) {

                case SDL_WINDOWEVENT:

                    switch (event.window.event) {

                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            update_size (&app);
                            break;
                    }

                    break;

                case SDL_QUIT:
                    goto quit;

                case SDL_KEYDOWN:

                    switch (event.key.keysym.sym) {

                        case SDLK_ESCAPE:
                            goto quit;

                        case SDLK_TAB:
                            app.display.power_supply_in = !app.display.power_supply_in;
                    }

                    break;
            }

        /* render the next image */
        display_process (&app.display, REALTIME ? SDL_GetTicks () / 1000.0 - app.time_init
                                                : (double) app.frame / FPS);

        /* export the frame */
        if (EXPORT)
            export_frame (&app);

        /* copy the rendered image to texture memory */
        SDL_UpdateTexture (app.texture,
                           NULL,
                           app.display.buffer_output,
                           app.display.width * sizeof (uint32_t));

        /* present the image on screen */
        SDL_SetRenderTarget (app.renderer, NULL);
        SDL_RenderCopy (app.renderer, app.texture, NULL, NULL);
        SDL_RenderPresent (app.renderer);

        /* keep track of the frame we are on */
        app.frame++;
    }
quit:

    /* free the display */
    display_deinit (&app.display);

    /* cleanup SDL stuff */
    SDL_DestroyTexture (app.texture);
    SDL_DestroyRenderer (app.renderer);
    SDL_DestroyWindow (app.window);
    SDL_Quit ();

    /* exit */
    return EXIT_SUCCESS;
}
