#include <iostream>
#include "Scene.h"
#include "Quad.h"
#include "ray_utils.h"
#include "Triangle.h"
#include "PhotonMap.h"
#include <GL/glut.h>
#include "Sphere.h"
#include <math.h>
#define _USE_MATH_DEFINES
#include "Presets.h"
#include <vector>
#include <random>
#include <thread>
#define SCENE_ID 3		// Scene Identifier from Scene.h file

// Image dimensions
#define WIDTH 512		
#define HEIGHT 384
#define far_Z 256

// Image Mode
#define DIRECT_ONLY 1
#define INDIRECT_ONLY 2
#define DIRECT_PLUS_INDIRECT 3

// Samples per pixel
#define SPP 1000

// Number of photons in map
#define NUM_PHOTONS 10000

// Filtering Mode
#define FILTERING_ON false
// SPP Intervals for Filtering
#define filter_intervals 50
// Kernel width
#define FILTER_WIDTH 2

// Number of Direct Light samples to take per iteration
#define DIRECT_LIGHT_SAMPLES 1

// Should Photon Map rendering (range queries) be performed during path tracing (ONLINE), or should the photon map be superimposed offline
// Setting this to false leads to faster rendering times, but can cause artefacts and inaccuracies that are missing in the online photon map computation
#define ONLINE_PHOTON_MAP false

// Depth of Path Tracer
#define MAX_DEPTH 4



int MODE = DIRECT_PLUS_INDIRECT;

int NUM_SAMPLES = 1;
int canvas_Z = 1;
Vec3f EYE(WIDTH /2, HEIGHT /2, -384);

using namespace std;

float diagonal = sqrtf(HEIGHT * HEIGHT + WIDTH * WIDTH + far_Z * far_Z);
vector<Objects*> objects;
vector<Objects*> lights;
vector<Objects*> caustics;
KDNode photonMap;
std::default_random_engine randGenerator;
std::uniform_real_distribution<float> distribution(0, 1);


int N;
Color buffer1[HEIGHT][WIDTH];
Color colors[HEIGHT][WIDTH];
Color filter_colors[HEIGHT][WIDTH];
Color photonPower[HEIGHT][WIDTH];
Vec3f normal_per_pixel[HEIGHT][WIDTH];
int object_per_pixel[HEIGHT][WIDTH];

inline
float clamp(const float& lo, const float& hi, const float& v)
{
	return std::max(lo, std::min(hi, v));
}


void init_window()
/* Clear the image area, and set up the coordinate system */
{
	/* Clear the window */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glOrtho(0, WIDTH, 0, HEIGHT, -1.0, 1.0);
}

void write_pixel(int x, int y, Color intensity)
/* Turn on the pixel found at x,y */
{
	glColor3f(intensity.r, intensity.g, intensity.b);
	glBegin(GL_POINTS);
	glVertex3i(x, y, 0);
	glEnd();
}

void display(void)   // Create The Display Function
{
	// Just displaying the Color array
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	      // Clear Screen 
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			write_pixel(j, i, colors[i][j]);
		}
	}
	glutSwapBuffers();
}

void init_scene(int scene_id) {
	// Retrieve Scene information
	Scene scene;
	scene.getScene(scene_id, WIDTH, HEIGHT, far_Z, objects, lights, caustics);
	N = objects.size();
}

Color traceRay(Ray& ray, int depth, bool mirrorRay) {
	// Return the irradiance/color retrieved by the ray

	float russian_roulette = distribution(randGenerator);
	bool inShadow = false;
	float MIN_DISTANCE = INFINITY;
	int object_hit = -1;
	for (int k = 0; k < N; k++) {
		// Loop through all objects to find intersections
		float hit_t = objects[k]->hit(ray, false);

		if (hit_t > 0) {
			// if hit_t > 0, the object is "in front" of the ray's origin point
			if (hit_t < MIN_DISTANCE) {
				// Keep track of the closest object to the ray's origin point
				MIN_DISTANCE = hit_t;
				object_hit = k;
			}
		}
	}
	if (object_hit == -1) {
		return BLACK;
	}

	// Compute the point of intersection
	Vec3f hitPoint = ray.point + ray.direction * MIN_DISTANCE;


	Color directIllumination, indirectIllumination, emission, photonPower, specular;
	photonPower = BLACK;
	indirectIllumination = BLACK;
	directIllumination = BLACK;
	specular = BLACK;

	// Get the material of the hit object
	Material mat = objects[object_hit]->getMaterial();
	emission = mat.emission;

	/* 
	* Since I am using DIRECT + INDIRECT lighting, I don't want to return the emission of an object
	* if an "indirect ray" hits the light source.
	* This is to avoid calculating the light energy twice 
	* Thus, at depth < MAX_DEPTH, I will return the emission of the object
	* In case of a mirror, I will return the emission anyway (to capture the reflection of light sources on the mirror)
	*/
	if (depth == 0) return (mirrorRay) ? emission : BLACK;
	if (emission.r > 0 || emission.g > 0 || emission.b > 0) {
		if (mirrorRay) return emission;
		if (MODE == DIRECT_PLUS_INDIRECT)
			return (depth == MAX_DEPTH) ? emission * mat.K_emission_intensity : BLACK;	 // Avoiding calculating light twice for Direct+Indirect
		else
			return emission * mat.K_emission_intensity;
	}

	// Compute the normal at the hitpoint
	Vec3f normal = objects[object_hit]->normal(hitPoint);

	// In case the normal is facing the same direction as the ray, flip the normal
	if (normal.dot(ray.direction) > 0)
		normal = -normal;

	if (MODE == DIRECT_ONLY || MODE == DIRECT_PLUS_INDIRECT) {
		//if (russian_roulette < (mat.K_refract / (mat.K_diffuse + mat.K_specular + mat.K_refract + mat.K_absorb)))
		if (mat.type == REFRACTIVE)
		{
			// https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel
			// If the object is refractive
			Vec3f n = normal;
			Vec3f L = ray.direction;
			float cosi = clamp(-1, 1, L.dot(n));
			//float cosi = L.dot(n);
			float etai = 1, etat = mat.ior;
			if (cosi < 0) { cosi = -cosi; }
			else { std::swap(etai, etat); n = -normal; }
			float eta = etai / etat;
			float k = 1 - eta * eta * (1 - cosi * cosi);
			if (k > 0) {
				Vec3f transmitted = (L * eta) + (n * (eta * cosi - sqrtf(k)));
				Vec3f o = (hitPoint + transmitted * 0.01);
				Ray transmitted_ray(o, transmitted, false);
				directIllumination = traceRay(transmitted_ray, depth - 1, false) * mat.K_refract;
			}

			// Calculate the irradiance along the Reflective direction
			Vec3f dir = (ray.direction);
			Vec3f temp(normal * 2 * (ray.direction).dot(normal));
			dir = (dir - temp).normalize();
			Ray reflectedRay(hitPoint, dir, false);

			directIllumination = directIllumination + traceRay(reflectedRay, 0, true);
		}
		else {
			for (int l = 0; l < DIRECT_LIGHT_SAMPLES; l++) {

				for (auto light : lights) {
					inShadow = false;
					// Sample random point on the area light
					float rand1 = distribution(randGenerator);
					float rand2 = distribution(randGenerator);
					float rand3 = distribution(randGenerator);
					
					Vec3f areaLightPoint = light->samplePointLight(rand1, rand2, rand3);
					float distance = areaLightPoint.dist2(hitPoint);
					
					// Shadow Ray to the direction of the light from the hitpoint
					Ray shadowRay(hitPoint, areaLightPoint, true);

					if (normal.dot(shadowRay.direction) > 0) {
					// Check if the object is facing the light	
						for (int k = 0; k < N; k++) {
							if (k == object_hit || light == objects[k])
								continue;
							float t = objects[k]->hit(shadowRay, true);
							if (t > 0) {
								if (t * t < distance && objects[k]->getMaterial().type != REFRACTIVE && objects[k]->getMaterial().type != EMISSIVE) {
								// If there is an object lying closer than the light in the +ve direction of the shadow ray,
								// then the object is in shadow
									inShadow = true;
									break;
								}
							}
						}

						if (!inShadow) {
							/* If the object is not in shadow, I calculate the direct Illumination
							* Otherwise Direct Illumination will remain zero (BLACK)
							*/

							// Calculate the dot product between the shadowRay direction and normal to the object
							float dotProduct = ((areaLightPoint - hitPoint).normalize()).dot(objects[object_hit]->normal(hitPoint));
													   
							// In case the dotProduct is less than 0 (object facing other way), make it 0
							if (dotProduct < 0) dotProduct = 0;

							/*
							* Compute the direct Illumination
							* Light_Incident * cos(theta)
							* This value will get scaled by K_diffuse of the material after the loop finishes executing
							*/

							directIllumination = directIllumination + (light->getMaterial().emission * light->getMaterial().K_emission_intensity) * dotProduct;

							//if (russian_roulette < ((mat.K_refract + mat.K_specular) / (mat.K_diffuse + mat.K_specular + mat.K_refract + mat.K_absorb))) {
							if (mat.type == GLOSSY) {
								// Calculate the specular component for reflective/specular materials
								Vec3f L = (hitPoint - areaLightPoint);
								Vec3f R = normal * (2 * normal.dot(L));
								R = (L - R).normalize();
								float d = (-ray.direction).dot(R);
								if (d < 0) d = 0;
								specular = specular + (light->getMaterial().emission);
							}
							else {

							}

						}
					}
					directIllumination = directIllumination + specular * mat.K_specular * 2;

				}

			}

			// Multiply the direct illumination with the material's diffuse color 
			directIllumination = directIllumination * mat.diffuse_albedo * mat.K_diffuse;

			directIllumination = directIllumination / (float)DIRECT_LIGHT_SAMPLES;

		}
	}

	if (MODE == INDIRECT_ONLY || MODE == DIRECT_PLUS_INDIRECT) {

	
		if (mat.type != MIRROR) {

			if (russian_roulette < mat.K_absorb / (mat.K_absorb + mat.K_diffuse + mat.K_specular)) {
				// The probability of the light getting absorbed using russian roulette scheme
				indirectIllumination = BLACK;
			}
			else {
				// For the diffuse case, the pdf is a constant
				float pdf = 1 / (2 * M_PI);
				// Store the tangent and bit-tangent to the normal
				Vec3f N_t, N_b;
				if (fabs(normal.x) > fabs(normal.y)) {
					N_t = Vec3f(normal.z, 0, -normal.x).normalize();
				}
				else {
					N_t = Vec3f(0, -normal.z, normal.y).normalize();
				}
				N_b = normal.cross(N_t).normalize();

				// generate a random value for cos-theta (where theta is between 0 and 90)
				float u1 = distribution(randGenerator);

				// Generate a random phi angle (between 0 and 2*PI)
				float u2 = distribution(randGenerator);
				float phi = 2 * M_PI * u2;

				float sinTheta;
				Vec3f dir;
				if (russian_roulette < (mat.K_absorb + mat.K_diffuse) / (mat.K_absorb + mat.K_diffuse + mat.K_specular)) {
					sinTheta = sqrtf(1 - u1 * u1);
						
				}
				else {
					sinTheta = pow((1 - u1 * u1), 1 / mat.K_specular);
				}

				// Cosine importance sampling 
				float x = sinTheta * cosf(phi);
				float z = sinTheta * sinf(phi);

				// A random vector is generated in the global co-ordinate frame using Cosine importance sampling
				Vec3f sample(x, u1, z);


				// Rotating the vector so that the UP-axis of the global frame points at the direction of the local normal
				dir = Vec3f(
					sample.x * N_b.x + sample.y * normal.x + sample.z * N_t.x,
					sample.x * N_b.y + sample.y * normal.y + sample.z * N_t.y,
					sample.x * N_b.z + sample.y * normal.z + sample.z * N_t.z);

				dir = dir.normalize();
				Ray randomRay(hitPoint, dir, false);
				Color shade = traceRay(randomRay, (depth - 1), false);

				// Clipping cos-theta to discount normals facing in the opposite direction
				if (u1 < 0) u1 = 0;

				if (russian_roulette < (mat.K_absorb + mat.K_diffuse) / (mat.K_absorb + mat.K_diffuse + mat.K_specular)) {

					// The rendering equation
					// light_incident * cos-theta * material_colour / (probability of the sample)
					indirectIllumination = indirectIllumination + shade * u1 / pdf * mat.diffuse_albedo * mat.K_diffuse;

					}
					else {
						// Count as specular reflection if the russian roulette value determines specular reflection
						indirectIllumination = indirectIllumination + shade * u1 / pdf * mat.diffuse_albedo * mat.K_specular;
					}


			}
		}

		else {
			// Mirror Ray
			Vec3f R = (ray.direction);
			Vec3f temp(normal * 2 * (ray.direction).dot(normal));
			R = (R - temp).normalize();
			Ray reflectedRay(hitPoint, R, false);
			Color shade = traceRay(reflectedRay, depth - 1, true); // The mirror-ray boolean is set to True here
			indirectIllumination = indirectIllumination + shade * mat.K_mirror_brightness;
			Vec3f hitPoint = ray.point + ray.direction * MIN_DISTANCE;
		}

	}

	if (ONLINE_PHOTON_MAP) {
		if (depth == MAX_DEPTH) {
			// Determines the photon intensity using the photon map
			vector<Photon> map;

			// Arbitrary Query window
			Vec3f q1(20, 20, 20);
			float cone_filter_parameter = q1.dot(q1) * 2;
			Vec3f query1 = hitPoint - q1;
			Vec3f query2 = hitPoint + q1;
			// Searches the photon map for photons collected in the neighborhood of the hitpoint and stores the photon in a vector
			photonMap.rangeSearch(query1, query2, map, NUM_PHOTONS / 5);
			int s = map.size();
			if (s > 1) {
				Vec3f normal = objects[object_hit]->normal(hitPoint);

				if (normal.dot(ray.direction) > 0)
					normal = -normal;
				Color power = BLACK;
				for (auto m : map) {
					/* Computes the total power due to that photon
					* A cone-type filter is used to favour photons that lie in the vicinity of the hit point
					* compared to those that lie farther away
					*/
					power = power + m.power * normal.dot(m.direction) * (1 - m.position.dist2(hitPoint) / cone_filter_parameter);
				}
				// Scale the accumulated photon power
				photonPower = mat.diffuse_albedo * power;
			}
		}
	}
	
	
	// Return the total irradiance due to direct light, indirect light, and caustic map
	return (directIllumination + indirectIllumination + photonPower);

}

void visualize_photon_map(int start_i, int end_i, int start_j, int end_j) {
	/* 
	* Used to visualize the photon map	
	* in the rectangular pixel space between (start_i, start_j) and (end_i, end_j)
	*/

	for (int i = start_i; i < end_i; i++) {
		for (int j = start_j; j < end_j; j++) {
			Vec3f canvas(j, i, canvas_Z);
			Vec3f canvasPart(canvas.x, canvas.y, canvas_Z);
			Ray ray(EYE, canvasPart, true);
			float MIN_DISTANCE = INFINITY;
			int object_hit = -1;

			// Compute hit point of the camera ray
			for (int k = 0; k < N - 1; k++) {
				float hit_t = objects[k]->hit(ray, false);

				if (hit_t > 0) {
					if (hit_t < MIN_DISTANCE) {
						MIN_DISTANCE = hit_t;
						object_hit = k;
					}
				}
			}
			if (object_hit == -1) {
				return;
			}
			Vec3f hitPoint = ray.point + ray.direction * MIN_DISTANCE;

			// Range search the photons in the vicinity of the hit point
			vector<Photon> map;
			Vec3f q1(10, 10, 10);
			float cone_filter_parameter = q1.dot(q1) * 2;

			Vec3f query1 = hitPoint - q1;
			Vec3f query2 = hitPoint + q1;
			photonMap.rangeSearch(query1, query2, map, NUM_PHOTONS/5);


			int s = map.size();
			if (s > 1) {
				Vec3f normal = objects[object_hit]->normal(hitPoint);

				if (normal.dot(ray.direction) > 0)
					normal = -normal;
				Color power = BLACK;
				for (auto m : map) {
					//power = power + m.power * normal.dot(m.direction) * ( 1 - m.position.dist2(hitPoint)/ cone_filter_parameter);
					power = power + m.power * normal.dot(m.direction) * (1 - m.position.dist2(hitPoint) / cone_filter_parameter);
				}
				photonPower[i][j] = objects[object_hit]->getMaterial().diffuse_albedo * power;
			}
			else {
				photonPower[i][j] = { 0, 0, 0 };
				
			}
		}
	}
}

void filter(int start_i, int end_i, int start_j, int end_j, int n, int K) {

	/*
	* Denoise the rendered image between (start_i, start_j) and (end_i, end_j)
	* K = number of iterations the filter loop should run. Higher K results in blurrier images
	* n = Number of SPP used to produce the original image
	*/
	for (int samples = 1; samples <= K; samples++) {
		for (int i = start_i; i < end_i; i++) {
			for (int j = start_j; j < end_j; j++) {
				Color filter = BLACK;
				int count = 0;
				int obj_idx = object_per_pixel[i][j];
				if (obj_idx == -1) {
					filter_colors[i][j] = BLACK;
					continue;
				}
				int kernel = FILTER_WIDTH;

				/*// Does not blur the image if the pixels contains the edges of an object

				if (!(object_per_pixel[i - 1][j - 1] == object_per_pixel[i + 1][j + 1] &&
					object_per_pixel[i - 1][j + 1] == object_per_pixel[i + 1][j - 1])) {
					filter_colors[i][j] = colors[i][j];
					continue;
				}
				*/
				for (int k = -kernel; k < kernel; k++) {
					for (int l = -kernel; l < kernel; l++) {
						if ((k + i) < 0 || (k + i) >= WIDTH) {
							continue;
						}
						if ((l + j) < 0 || (l + j) >= HEIGHT) {
							continue;
						}
						
						if (obj_idx == object_per_pixel[k + i][l + j]) {
							//int max_dis = (abs(i) > abs(j)) ? abs(i) : abs(j);
							//int weight = 1; //(FILTER_WIDTH - max_dis) + 1;
							int weight = (2 * FILTER_WIDTH - abs(k) - abs(l));
							float dif_r = abs(colors[i][j].r - colors[i + k][j + l].r);
							float dif_g = abs(colors[i][j].g - colors[i + k][j + l].g);
							float dif_b = abs(colors[i][j].b - colors[i + k][j + l].b);

							if (objects[obj_idx]->getMaterial().type == REFRACTIVE || objects[obj_idx]->getMaterial().type == MIRROR) {

								if (dif_r < 0.5 && dif_g < 0.5 && dif_b < 0.5) {
									filter = filter + colors[k + i][l + j] * weight;
									count = count + weight;
								}
							}
							else {
								
								if (dif_r < 0.8 && dif_g < 0.8 && dif_b < 0.8) {
									filter = filter + colors[k + i][l + j] * weight;
									count = count + weight;
								}
							}

						}
					}

				}
				if (count != 0) 
					filter_colors[i][j] = (filter) / count;
				else
					filter_colors[i][j] = colors[i][j];
			}
		}

		for (int i = start_i; i < end_i; i++) {
			for (int j = start_j; j < end_j; j++) {

				colors[i][j] = filter_colors[i][j];
			}
		}
		/*
		// Removes "fireflies" and outlier pixels
		for (int i = start_i; i < end_i; i++) {
			for (int j = start_j; j < end_j; j++) {
				float mean_r = 0, mean_g = 0, mean_b = 0;
				int count = 0;
				int obj_idx = object_per_pixel[i][j];
				if (obj_idx == -1) {
					filter_colors[i][j] = BLACK;
					continue;
				}

				if (objects[obj_idx]->getMaterial().type == REFRACTIVE || objects[obj_idx]->getMaterial().type == MIRROR) {
					filter_colors[i][j] = colors[i][j];
					continue;
				}
				for (int k = -5; k <= 5; k++) {
					for (int l = -5; l <= 5; l++) {
						if (k == 0 && l == 0) continue;
						if (i + k >= HEIGHT || i - k < 0 || j + l>=WIDTH || j + l < 0) continue;
						if (obj_idx != object_per_pixel[k + i][l + j]) continue;

						mean_r += colors[i + k][j + l].r;
						mean_g += colors[i + k][j + l].g;
						mean_b += colors[i + k][j + l].b;
						count++;
					}
				}
				float dif_r = abs(colors[i][j].r - mean_r / count);
				float dif_g = abs(colors[i][j].g - mean_g / count);
				float dif_b = abs(colors[i][j].b - mean_b / count);
				if (dif_r > 0.6 || dif_g > 0.6 || dif_b > 0.6)
					colors[i][j] = { mean_r / count, mean_g / count, mean_b / count };
				
			}

		}
		*/
		
	}
	
}

void collect_filter_data() {
	// Collects the object_per_pixel as a pre-pass to the rendering step => G-Buffer
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			object_per_pixel[i][j] = -1;
			float MIN_DISTANCE = INFINITY;
			Vec3f canvas(j, i, canvas_Z);
			Ray ray(EYE, canvas, true);
			for (int k = 0; k < N; k++) {
				float hit_t = objects[k]->hit(ray, false);

				if (hit_t > 0) {
					if (hit_t < MIN_DISTANCE) {

						MIN_DISTANCE = hit_t;
						object_per_pixel[i][j] = k;
					}
				}

			}
		}
	}

}

bool estimate_discrepancy(int i, int j, int samples) {
	if (colors[i][j].r < 0.2 || colors[i][j].g < 0.2 || colors[i][j].b < 0.2) {
		return true;
	}
	Color average = BLACK;
	int num_adj = 0;
	int count = 0;
	int kernel_width = 2;
	for (int ii = -kernel_width; ii <= kernel_width; ii++) {
		for (int jj = -kernel_width; jj <= kernel_width; jj++) {
			if (ii < 0 || jj < 0 || ii > HEIGHT - 1 || jj > WIDTH - 1)
				continue;
			num_adj++;
			if (abs(colors[i][j].r - colors[ii][jj].r) > 0.2f || abs(colors[i][j].g - colors[ii][jj].g) > 0.2f || abs(colors[i][j].b - colors[ii][jj].b) > 0.2f) {
				return true;
			}
		}
	}
	return false;
}

void fillColorArray(int start_i, int end_i, int start_j, int end_j) {
	// Fill the color values of each pixel between (start_i, start_j) and (end_i, end_j)
	int depth_limit = MAX_DEPTH;

	for (int n = 1; n <= SPP; n++) {
		/*
		* alpha = Learning rate
		* Used to keep a running average between the current rendered image (without denoising) and the denoised image
		* As n increases, I am decreasing the learning rate to slowly direct the image towards converging towards the denoised image
		* Decreasing alpha too soon will mean that most samples will not influence the final image, may result in blocky patches in the final image due to inefficient sampling
		* Decreasing alpha too slow will only hurt the running time of the render
		*/

		
		float alpha = 1;
		if (FILTERING_ON) alpha = 0.5;
		for (int i = start_i; i < end_i; i++) {
			for (int j = start_j; j < end_j; j++) {
				Vec3f canvas(j, i, canvas_Z);

				// Anti-aliasing by sampling the subpixels i.e. Distributed Ray Tracing
				float subPixelX = distribution(randGenerator);
				float subPixelY = distribution(randGenerator);
				Vec3f canvasPart(canvas.x + subPixelX, canvas.y + subPixelY, canvas_Z);

				// Generate ray
				Ray ray(EYE, canvasPart, true);

				// Cast ray
				if (n < 30) {
					buffer1[i][j] = buffer1[i][j] + traceRay(ray, depth_limit, false);
				}
				else {


					if (estimate_discrepancy(i, j, n))
						buffer1[i][j] = buffer1[i][j] + traceRay(ray, depth_limit, false);
					else
						buffer1[i][j] = buffer1[i][j] / (n - 1) * n;
				}
				if (n == 1) {
					colors[i][j] = (buffer1[i][j]) / (n);
				}
				else {
					/*
					* The buffer[][] matrix represents the true samples collected from the light transport computations
					* The colors[][] matrix will go through the denoise filter every 10 iterations
					* The final colors[][] matrix is a weighted average of the denoised image as well as the true estimates obtained through sampling
					*/
					colors[i][j] =  colors[i][j] * (1 - alpha) + (buffer1[i][j]) / n * alpha;
					//colors[i][j] = (buffer1[i][j]) / n;
				}
				if (!ONLINE_PHOTON_MAP)
					colors[i][j] = colors[i][j] + photonPower[i][j];
				
			}

		}
		if (n % filter_intervals == 0) {
			if (FILTERING_ON)
				filter(start_i, end_i, start_j, end_j, n, 2);
			cout << "SPP: " << n << " " << endl;
		}
	}
	if (FILTERING_ON)
		filter(start_i, end_i, start_j, end_j, SPP, 2);
}

void trace_caustic(Ray& ray, int depth, Color power) {
	/* 
	* Stores photon rays casted towards caustic objects in a photon map
	* ray = origin and direction of photon
	* depth = depth of recursion
	* orig_id = object id of the last-hit object (used to avoid ray intersections with the same object)
	* power = irradiance of the current photon
	*/
	if (depth == 0) return;
	float MIN_DISTANCE = INFINITY;
	int object_hit = -1;
	
	
	for (int k = 0; k < objects.size(); k++) {
		// Finds object intersection
		//if (k == orig_id) continue;

		float hit_t = objects[k]->hit(ray, false);

		if (hit_t > 0) {
			if (hit_t < MIN_DISTANCE) {
				MIN_DISTANCE = hit_t;
				object_hit = k;
			}
		}
	}

	if (object_hit == -1) {
		return;
	}

	Material mat = objects[object_hit]->getMaterial();
	Vec3f hitPoint = ray.point + ray.direction * MIN_DISTANCE;
	Vec3f normal = objects[object_hit]->normal(hitPoint);

	
	float russian_roulette = distribution(randGenerator); // Russian Roulette random variable to determine light-material interaction
	if (mat.type == REFRACTIVE || mat.type == MIRROR) {
		if (russian_roulette < mat.K_refract / (mat.K_refract + mat.K_mirror_brightness + mat.K_absorb)) {
			// Calculates the refracted direction
			Vec3f L = ray.direction;
			float cosi = clamp(-1, 1, L.dot(normal));
			float etai = 1, etat = mat.ior;
			Vec3f n = normal;
			if (cosi < 0) { cosi = -cosi; }

			else { std::swap(etai, etat); n = -normal; }
			float eta = etai / etat;
			float k = 1 - eta * eta * (1 - cosi * cosi);
			if (k > 0) {
				Vec3f transmitted = (L * eta) + (n * (eta * cosi - sqrtf(k)));
				Vec3f o = hitPoint + transmitted * 0.001;
				Ray transmitted_ray(o, transmitted, false);
				trace_caustic(transmitted_ray, depth - 1, power * mat.diffuse_albedo * mat.K_refract * (ray.direction).dot(normal));
				//trace_caustic(transmitted_ray, depth - 1, power);
			}
		}
		else {
			Vec3f R = (ray.direction);
			Vec3f temp(normal * 2 * (ray.direction).dot(normal));
			R = (R - temp).normalize();
			Ray reflectedRay(hitPoint, R, false);
			trace_caustic(reflectedRay, depth - 1, power * mat.K_mirror_brightness);
		}
	}
	else {
		if (depth == 3) {
			return;
		}
		else {
			Photon p(hitPoint, ray.direction, power);
			photonMap.insert(p);
			return;
		}
	}
}

int inverseCDFSample(vector<Objects*>& obj, float rand) {
	vector <float> area(obj.size());
	float sum = 0;
	for (int i = 0; i < obj.size(); i++) {
		sum = sum + obj[i]->computeArea();
		area[i] = sum;
	}
	
	// Compute normalized cdf
	for (int i = 0; i < obj.size(); i++) {
		area[i] = area[i]/sum;
		if (rand <= area[i]) {
			return i;
		}

	}
	return 1;
}

void create_photon_map() {
	for (auto light : lights) {
		Color  init_power = light->getMaterial().emission * light->getMaterial().K_emission_intensity / NUM_PHOTONS * 5;
		//float rand = distribution(randGenerator);
		//int idx = inverseCDFSample(caustics, rand);
		for (int idx = 0; idx < caustics.size(); idx++) {
			for (int i = 0; i < NUM_PHOTONS; i++) {
				Vec3f areaLightPoint = light->samplePointLight(distribution(randGenerator), distribution(randGenerator), distribution(randGenerator));
				Vec3f objectPoint = caustics[idx]->samplePointLight(distribution(randGenerator), distribution(randGenerator), distribution(randGenerator));
				Ray r(areaLightPoint, objectPoint, true);

				trace_caustic(r, 3, init_power);

			}
		}
	}
}

int main(int argc, char* argv[]) {


	init_scene(SCENE_ID);

	create_photon_map();
	int num_vectors = 16;
	vector<thread> threadPool;
	int steps = WIDTH / num_vectors;
	int i = 0;
	if (FILTERING_ON) collect_filter_data();
	for (int k = 0; k < num_vectors; k++) {
		threadPool.emplace_back(visualize_photon_map, 0, HEIGHT, i, i + steps);
		threadPool.emplace_back(fillColorArray, 0, HEIGHT, i, i + steps);
		i = i + steps;
	}
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Computer Graphics");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	init_window();				             //create_window
	glutMainLoop();

}