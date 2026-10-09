/// Include the image write library and enable it
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

/// Uncomment these lines if you want to debug using couts
#include <iostream>
using namespace std;

class Vector3
{
public:
  float x, y, z;

  Vector3 &operator+=(const Vector3 &rhs)
  {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }
  Vector3 &operator-=(const Vector3 &rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }

  Vector3 &operator*=(const float rhs)
  {
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
  }

  float dot(const Vector3 &rhs)
  {
    return x * rhs.x + y * rhs.y + z * rhs.z;
  }

  Vector3 &cross(const Vector3 &rhs)
  {
    Vector3 temp = Vector3{
        y * rhs.z - z * rhs.y,
        z * rhs.x - x * rhs.z,
        x * rhs.y - y * rhs.x};
    return temp;
  }

  float length()
  {
    return sqrt(x * x + y * y + z * z);
  }

  Vector3 &normalize()
  {
    float l = length();
    Vector3 temp = Vector3{x / l, y / l, z / l};
    return temp;
  }
};

inline Vector3 &operator+(Vector3 lhs, const Vector3 &rhs)
{
  return lhs += rhs;
}

inline Vector3 &operator-(Vector3 lhs, const Vector3 &rhs)
{
  return lhs -= rhs;
}

inline Vector3 &operator*(Vector3 lhs, const float rhs)
{
  return lhs *= rhs;
}

/// Uncomment these lines if you happen to add a Vector3 and want to debug :)
inline std::ostream &operator<<(std::ostream &os, const Vector3 &v)
{
  return os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}

int main()
{
  /// Example of how to debug with couts.
  /// You must uncomment the include and namespace lines above
  // cout << "Hello, world" << endl;

  Vector3 temp = Vector3{0, 0, 0};
  temp += Vector3{1, 1, 1};

  /// Width of the final image
  const int w = 640;

  /// Height of the final image
  const int h = 640;

  /// The 1d array for storing the pixel we have generated
  unsigned char pixels[w * h * 3];

  /// Loop over each line doing down
  for (int y = 0; y < h; y++)
  {
    /// While going down, loop over every pixel going right
    for (int x = 0; x < w; x++)
    {

      // how to ray trace??
      Vector3 camera_origin = Vector3{0, 0, -1}; // x, y, z
      Vector3 camera_look_at = Vector3{0, 0, 0};
      Vector3 camera_up = Vector3{0, 1, 0};

      Vector3 background_color = Vector3{255, 0, 255};

      Vector3 target{
          x / w * 2 - 1, -(y / h * 2 - 1), 0};

      Vector3 ray = (target - camera_origin).normalize();

      /// Figure out our index in the 1d array of pixels
      int index = y * w * 3 + x * 3;

      /// The value of the red channel (0-255)
      unsigned char r = background_color.x;

      /// The value of the green channel (0-255)
      unsigned char g = background_color.y;

      /// The value of the blue channel (0-255)
      unsigned char b = background_color.z;

      /// Assign the r, g, and b values to the pixel at index
      pixels[index] = r;
      pixels[index + 1] = g;
      pixels[index + 2] = b;
    }
  }

  /// Write the final image as a png
  stbi_write_png("image.png", w, h, 3, pixels, w * 3);
  return 0;
}
