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

ray3_t ray3_make (vec3_t origin, vec3_t target) {

    ray3_t ray;
    ray.origin    = origin;
    ray.direction = vec3_direction (origin, target);
    return ray;
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

bool ray_intersect_sphere (ray3_t ray, sphere_t sphere, double *distance, vec3_t *point, vec3_t *normal) {

    /* find intersection points */
    vec3_t relative_origin = vec3_subtract (ray.origin, sphere.origin);
    double a = vec3_dot (ray.direction,   ray.direction);
    double b = vec3_dot (relative_origin, ray.direction) * 2;
    double c = vec3_dot (relative_origin, relative_origin) - sphere.radius * sphere.radius;
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
        *normal = vec3_direction (sphere.origin, *point);

        /* we hit the sphereeeeeeeee */
        return true;

    } else
        /* we hit the skyyyy */
        return false;
}

#define N_SPHERES 3

bool ray_intersect_scene (ray3_t ray, vec3_t *point, vec3_t *normal, material_t *material, double time) {

    size_t i;

    /* the spheres in the scene */
    sphere_t spheres[N_SPHERES] = {
        sphere_make (vec3_make (1 * cos (0.25 * time * M_PI * 2), -0.5, 1 * sin (0.25 * time * M_PI * 2) + 1), 0.1),
        sphere_make (vec3_make (1 * cos (0.25 * time * M_PI * 2 + M_PI), 0.5, 1 * sin (0.25 * time * M_PI * 2 + M_PI) + 1), 0.1),
        sphere_make (vec3_make (0, 0, 1), 1)
    };

    /* materials */
    material_t materials[N_SPHERES] = {
        material_make (0, (sin (time * M_PI * 4) + 1) / 2, 0),
        material_make (0, 1, 0),
        material_make (1, 0, 0.5)
    };

    /* the intersections and normals */
    double distances[N_SPHERES];
    vec3_t    points[N_SPHERES];
    vec3_t   normals[N_SPHERES];
    bool     results[N_SPHERES];

    for (i = 0; i < N_SPHERES; i++) {
        results[i] = ray_intersect_sphere (ray, spheres[i], &distances[i], &points[i], &normals[i]);
    }

    /* calculate closest */
    bool result = false;
    double distance;
    for (i = 0; i < N_SPHERES; i++) {

        if (results[i]) {

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

bool ray_cast (ray3_t ray, double time) {

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

            ray3_t reflection = ray3_make (point,
                                           reflect (normal,
                                                    ray.direction,
                                                    material.scattering));
            return ray_cast (reflection, time) && rand_bool (material.reflectance);
            /* TODO max recursion */

        } else /* TODO proper combine probabilities */
            return rand_bool (material.emittance);

    } else
        return false;
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

    /* randomly sample a point on the screen */
    const double eye_z = -1;
    double ratio = app->display.width / (double) app->display.height;

    /* normalized device coordinates */
    double x = (noise () * 2 - 1) * ratio;
    double y = noise () * 2 - 1;
    
    /* cast a ray from the eye to the point on screen */
    ray3_t ray = ray3_make (vec3_make (0, 0, eye_z), vec3_make (x, y, 0));
    app->display.gun_enabled = ray_cast (ray, (display->i_electron + i_electron) * SECONDS_PER_ELECTRON);

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
                                            SDL_PIXELFORMAT_RGBA8888,
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

int main (int argc, char **argv) {

    app_t app;
    SDL_Event event;

    memset (&app, 0, sizeof (app));

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
        app.frame++;

        /* copy the rendered image to texture memory */
        SDL_UpdateTexture (app.texture,
                           NULL,
                           app.display.buffer_output,
                           app.display.width * sizeof (uint32_t));

        /* present the image on screen */
        SDL_SetRenderTarget (app.renderer, NULL);
        SDL_RenderCopy (app.renderer, app.texture, NULL, NULL);
        SDL_RenderPresent (app.renderer);
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
