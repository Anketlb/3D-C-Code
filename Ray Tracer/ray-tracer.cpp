/// Include the image write library and enable it
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

/// Uncomment these lines if you want to debug using couts
#include <iostream>
#include <vector>
#include <algorithm>
#include "Vector3.hpp"
#include "Triangle.hpp"
#include "file_util.h"
using namespace std;

/// Width of the final image
const int w = 640;

/// Height of the final image
const int h = 640;

/// The 1d array for storing the pixel we have generated
unsigned char pixels[w * h * 3];

// left wall
// Triangle triangle0f = Triangle{face[0],face[1],face[2]};
// Triangle triangle0f = Triangle{[ index[1], texture[1], normal[1] ], [ index[2], texture[2], normal[1] ], [ index[3], texture[3], normal[1] ]};
// Triangle triangle0f = Triangle{[ index[i-1], texture[i-1], normal[i-1] ], [ index[i-1], texture[i-1], normal[i-1] ], [ index[i-1], texture[i-1], normal[i-1] ]};

// Triangle triangle01 = Triangle{Vector3{-1, 1, -2}, Vector3{-1, 1, -1}, Vector3{-1, -1, -2}};
vector<Triangle> triangles = {};

Vector3 background_color = Vector3{255, 255, 255};

float trace_ray_distance(Vector3 origin, Vector3 direction)
{
  float closest_t = INFINITY;

  for (const Triangle &triangle : triangles)
  {

    // Find where we hit the plane
    Vector3 edge_one = triangle.one - triangle.two;
    Vector3 edge_two = triangle.three - triangle.two;
    Vector3 p = edge_two.cross(edge_one);
    Vector3 abc = p.normalize();
    float d = -abc.dot(triangle.one);

    float t = -(abc.dot(origin) + d) / abc.dot(direction);

    if (t > closest_t || t <= 0)
      continue;

    Vector3 collision_point = direction * t + origin;

    Vector3 minus_one = collision_point - triangle.one;
    Vector3 minus_two = collision_point - triangle.two;
    Vector3 minus_three = collision_point - triangle.three;

    float triangle_area = p.length() / 2;
    float area_one = minus_two.cross(minus_three).length() / 2;
    float area_two = minus_three.cross(minus_one).length() / 2;
    float area_three = minus_one.cross(minus_two).length() / 2;

    float u = area_one / triangle_area;
    float v = area_two / triangle_area;
    float a = area_three / triangle_area;

    float sum = u + v + a;

    float diff = abs(1 - sum);

    if (diff > .00001 || isnan(sum))
    {
      // Im not in the triangle
    }
    else
    { /// try to implement a shader inside this
      closest_t = t;
    }
  }
  return closest_t;
}

Vector3 trace_ray(Vector3 origin, Vector3 direction, int bounces = 2)
{
  Vector3 color = Vector3{background_color.x, background_color.y, background_color.z};
  if (bounces <= 0)
    return background_color;

  float closest_t = 1000000;

  for (const Triangle &triangle : triangles)
  {

    // Find where we hit the plane
    Vector3 edge_one = triangle.one - triangle.two;
    Vector3 edge_two = triangle.three - triangle.two;
    Vector3 p = edge_two.cross(edge_one);
    Vector3 abc = p.normalize();
    float d = -abc.dot(triangle.one);

    float t = -(abc.dot(origin) + d) / abc.dot(direction);

    if (t > closest_t || t <= 0)
      continue;

    Vector3 collision_point = direction * t + origin;

    Vector3 minus_one = collision_point - triangle.one;
    Vector3 minus_two = collision_point - triangle.two;
    Vector3 minus_three = collision_point - triangle.three;

    float triangle_area = p.length() / 2;
    float area_one = minus_two.cross(minus_three).length() / 2;
    float area_two = minus_three.cross(minus_one).length() / 2;
    float area_three = minus_one.cross(minus_two).length() / 2;

    float u = area_one / triangle_area;
    float v = area_two / triangle_area;
    float a = area_three / triangle_area;

    float sum = u + v + a;

    float diff = abs(1 - sum);

    if (diff > .00001 || isnan(sum))
    {
      // Im not in the triangle
    }
    else
    { /// try to implement a shader inside this
      // i am in triangle
      // u is up left, v is bot left, and a is bot right
      // The terms U,V, and A are positional only
      // 0 * u + 0 * v + 0 * a
      closest_t = t;

      Vector3 base_color = Vector3{255, 255, 255};

      // 1st is x Right+ to Left- , 2nd is y goes up+ or down-, 3rd is z Forward or Backward
      Vector3 light_direction = Vector3{.5, 1, .5}.normalize();

      Vector3 ambient = base_color * .1;
      Vector3 normal = p.normalize();

      // float strength = max(0.f, light_direction.dot(normal));
      float strength = abs(light_direction.dot(normal));

      Vector3 diffuse = base_color * strength;
      if (trace_ray_distance(collision_point + light_direction * .00001, light_direction) != INFINITY)
      {
        diffuse = Vector3{0, 0, 0};
      }

      Vector3 reflective = Vector3{0, 0, 0};

      if (true)
      {
        Vector3 reflective_direction = direction - normal * 2 * normal.dot(direction);
        reflective = trace_ray(collision_point + reflective_direction * .00001, reflective_direction, bounces - 1) * .5;
        diffuse *= .5;
        ambient *= .5;
      }

      Vector3 _color = ambient + diffuse + reflective;
      if(_color.x > 255.0){
        _color.x = 255.0;
      }
      if(_color.y > 255.0){
        _color.y = 255.0;
      }
      if(_color.z > 255.0){
        _color.z = 255.0;
      }
      color = _color;
    }
  }
  return color;
}

int main()
{
  // how to ray trace??
  Vector3 camera_origin = Vector3{0, 0, 1}; // x, y, z
  Vector3 camera_look_at = Vector3{0, 0, 0};
  Vector3 camera_up = Vector3{0, 1, 0};
  vector<string> book = readLines("cubemove.obj");
  vector<Vector3> index = {};
  vector<Vector3> texture = {};
  vector<Vector3> normal = {};
  vector<vector<vector<int>>> face = {};
  
  // iterators are neat: we can go through file
  for (auto scan = book.begin(); scan != book.end(); ++scan){// scan through the book one line at a time
    // vector<string> word = split(*scan, ' '); // split what you see into single words by spaces
    stringstream peasant(*scan);
    string torch;
    peasant >> torch;
    if (torch == "v"){
      Vector3 chest; // set up temp vector to shove into real vector
      peasant >> chest.x >> chest.y >> chest.z;
      index.push_back(chest); // push Temp Vector into actual Vector
    }
    if (torch == "vn")
    {                            // find every word that starts with vn
      Vector3 chest; // set up temp vector to shove into real vector
      peasant >> chest.x >> chest.y >> chest.z;
      normal.push_back(chest); // push Temp Vector into actual Vector
    }
    if (torch == "vt"){  // find every word that starts with vt
      Vector3 chest; // set up temp vector to shove into real vector
      peasant >> chest.x >> chest.y >> chest.z;
      texture.push_back(chest); // push Temp Vector into actual Vector
    }
    if (torch == "f"){
      vector<vector<int>> chest;
      string lit;
      for(int i = 0; i < 3; i++){
        vector<int> hand;
        peasant >> lit;
        vector<string> ash = split(lit,'/');
        int vkey = stoi(ash[0]) - 1;
        int vtkey = stoi(ash[1]) - 1;
        int vnkey = stoi(ash[2]) - 1;
        hand.push_back(vkey);
        hand.push_back(vtkey);
        hand.push_back(vnkey);
        chest.push_back(hand);
      }
      face.push_back(chest);
    }
  }

  for (int t = 0; t < face.size(); t++){
        vector<Vector3> corner_1 = {};
        vector<Vector3> corner_2 = {};
        vector<Vector3> corner_3 = {}; // face[0] = the entire line
    for (int c = 0; c < face[t].size(); c++){ // face[0][0] = first corner
      for (int v = 0; v < face[t][c].size(); v++){ // face[0][0][0] = first value
        int value = face[t][c][v];
        if(c == 0){ // Corner 1
          //Index key
          if(v == 0){
            corner_1.push_back(index[value]);
          }
          //Texture key
          if(v == 1){
            corner_1.push_back(texture[value]);
          }
          //Normal key
          if(v == 2){
            corner_1.push_back(normal[value]);
          }
        }
        if(c == 1){ // Corner 2
         //Index key
          if(v == 0){
            corner_2.push_back(index[value]);
          }
          //Texture key
          if(v == 1){
            corner_2.push_back(texture[value]);
          }
          //Normal key
          if(v == 2){
            corner_2.push_back(normal[value]);
          }
        }
        if(c == 2){ // Corner 3
           //Index key
          if(v == 0){
            corner_3.push_back(index[value]);
          }
          //Texture key
          if(v == 1){
            corner_3.push_back(texture[value]);
          }
          //Normal key
          if(v == 2){
            corner_3.push_back(normal[value]);
          }
        }
        // Triangle triangle01 = Triangle{Vector3{-1, 1, -2}, Vector3{-1, 1, -1}, Vector3{-1, -1, -2}};
      }
    }
    Triangle triangle = Triangle{Vector3{corner_1[0].x,corner_1[0].y,corner_1[0].z}, Vector3{corner_2[0].x,corner_2[0].y,corner_2[0].z}, Vector3{corner_3[0].x,corner_3[0].y,corner_3[0].z}};
    triangles.push_back(triangle);
  }



  /// Loop over each line doing down
  for (int y = 0; y < h; y++)
  {
    clog << "\rScanlines Remaining: " << (h - y) << ' ' << flush;
    /// While going down, loop over every pixel going right
    for (int x = 0; x < w; x++)
    {
      // if(x == 226 && y == 504){
      //   cout << "Problem";
      // }
      // else{
      //   continue;
      // }
      /// Figure out our index in the 1d array of pixels
      int index = y * w * 3 + x * 3;

      /// The value of the red channel (0-255)
      unsigned char r = background_color.x;

      /// The value of the green channel (0-255)
      unsigned char g = background_color.y;

      /// The value of the blue channel (0-255)
      unsigned char b = background_color.z;

      Vector3 target{
          x / (float)w * 2 - 1, -(y / (float)h * 2 - 1), 0};

      Vector3 ray = (target - camera_origin).normalize();
      Vector3 color = trace_ray(camera_origin, ray);

      r = color.x;
      g = color.y;
      b = color.z;

      /// Assign the r, g, and b values to the pixel at index
      pixels[index] = r;
      pixels[index + 1] = g;
      pixels[index + 2] = b;
    }
  }
  clog << "\rDone.                          \n";


  

  /// Write the final image as a png
  stbi_write_png("image.png", w, h, 3, pixels, w * 3);
  return 0;
}