#pragma once

#include <cmath>
#include <format>
#include <iostream>
#include <string>

namespace cass::linear {

inline constexpr double PI = 3.141592653589793;

/// Concept for types that support addition.
template <typename T>
concept Addable = requires(T a, T b) {
  { a + b } -> std::convertible_to<T>;
};

/// Concept for types that support subtraction.
template <typename T>
concept Subtractable = requires(T a, T b) {
  { a - b } -> std::convertible_to<T>;
};

/// Concept for types that support unary negation.
template <typename T>
concept Negatable = requires(T a) {
  { -a } -> std::convertible_to<T>;
};

/// Concept for types that support multiplication.
template <typename T>
concept Multipliable = requires(T a, T b) {
  { a * b } -> std::convertible_to<T>;
};

/// Concept for types that support division.
template <typename T>
concept Divisible = requires(T a, T b) {
  { a / b } -> std::convertible_to<T>;
};

/// Concept for types that support addition assignment.
template <typename T>
concept AddAssignable = requires(T a, T b) {
  { a += b } -> std::convertible_to<T>;
};

/// Concept for types that support subtraction assignment.
template <typename T>
concept SubtractAssignable = requires(T a, T b) {
  { a -= b } -> std::convertible_to<T>;
};

inline double radians(double degrees) { return degrees * PI / 180; }
inline double degrees(double degrees) { return degrees * 180 / PI; }

/// A two-dimensional vector containing values of type T.
///
/// @tparam T The type of each vector component.
template <typename T> class Vector2 {
public:
  T x, y;
  /// Constructs a zero 2D vector.
  Vector2() : x(0), y(0) {}

  /// Constructs a 2D vector from two components.
  ///
  /// @param x The X component.
  /// @param y The Y component.
  Vector2(T x, T y) : x(x), y(y) {}

  /// Constructs a 2D vector from another 2D vector of a different type.
  ///
  /// @tparam U The type of the other vector's components.
  /// @param other The other vector.
  template <typename U>
  Vector2(const Vector2<U> &other)
      : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

  /// Adds two 2D vectors together.
  ///
  /// @param v The vector to add.
  /// @return The resulting vector.
  Vector2 operator+(const Vector2 &v) const
    requires Addable<T>
  {
    return Vector2(x + v.x, y + v.y);
  }

  /// Adds a scalar value to each component of the 2D vector.
  ///
  /// @param f The scalar value to add.
  /// @return The resulting vector.
  Vector2 operator+(const T &f) const
    requires Addable<T>
  {
    return Vector2(x + f, y + f);
  }

  /// Negates the 2D vector.
  ///
  /// @return The negated vector.
  Vector2 operator-() const
    requires Negatable<T>
  {
    return Vector2(-x, -y);
  }

  /// Subtracts a 2D vector from this 2D vector.
  ///
  /// @param v The 2D vector to subtract.
  /// @return The resulting 2D vector.
  Vector2 operator-(const Vector2 &v) const
    requires Subtractable<T>
  {
    return Vector2(x - v.x, y - v.y);
  }

  /// Subtracts a scalar value from each component of the 2D vector.
  ///
  /// @param f The scalar value to subtract.
  /// @return The resulting 2D vector.
  Vector2 operator-(const T &f) const
    requires Subtractable<T>
  {
    return Vector2(x - f, y - f);
  }

  /// Multiplies each component of the 2D vector by a scalar value.
  ///
  /// @param f The scalar value to multiply by.
  /// @return The resulting 2D vector.
  Vector2 operator*(const T &f) const
    requires Multipliable<T>
  {
    return Vector2(x * f, y * f);
  }

  /// Multiplies a scalar value by a 2D vector.
  ///
  /// @param f The scalar value to multiply by.
  /// @param v The 2D vector to multiply.
  /// @return The resulting 2D vector.
  friend Vector2 operator*(const T &f, const Vector2 &v)
    requires Multipliable<T>
  {
    return Vector2(v.x * f, v.y * f);
  }

  /// Divides each component of the 2D vector by a scalar value.
  ///
  /// @param f The scalar value to divide by.
  /// @return The resulting 2D vector.
  Vector2 operator/(const T &f) const
    requires Divisible<T>
  {
    return Vector2(x / f, y / f);
  }

  /// Adds another 2D vector to this 2D vector in place.
  ///
  /// @param v The 2D vector to add.
  /// @return A reference to this 2D vector.
  Vector2 operator+=(const Vector2 &v)
    requires AddAssignable<T>
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  /// Subtracts another 2D vector from this 2D vector in place.
  ///
  /// @param v The 2D vector to subtract.
  /// @return A reference to this 2D vector.
  Vector2 operator-=(const Vector2 &v)
    requires SubtractAssignable<T>
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  /// Computes the dot product of this 2D vector and another 2D vector.
  ///
  /// @param v The other 2D vector.
  /// @return The dot product scalar result.
  T dot(const Vector2 &v) const
    requires Multipliable<T> && Addable<T>
  {
    return x * v.x + y * v.y;
  }

  /// Computes the magnitude (length) of the 2D vector.
  ///
  /// @return The magnitude of the 2D vector scalar result.
  T magnitude() const
    requires Multipliable<T> && Addable<T>
  {
    return sqrt(x * x + y * y);
  }

  /// Normalizes the 2D vector to have a magnitude of 1.
  ///
  /// @note If the 2Dvector has a magnitude of 0, the original vector is
  /// returned.
  /// @return The normalized 2D vector.
  Vector2 normalize() const
    requires Divisible<T> && Multipliable<T>
  {
    T mag = magnitude();

    if (mag != T(0)) {
      T inv_m = T(1) / mag;
      return Vector2(x * inv_m, y * inv_m);
    }

    return *this;
  }

  /// Outputs the 2D vector to an output stream.
  ///
  /// @param os The output stream.
  /// @param v The 2D vector to output.
  /// @return The output stream.
  friend std::ostream &operator<<(std::ostream &os, const Vector2 &v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
  }

  /// Converts the 2D vector to a string representation.
  ///
  /// @return A string representation of the 2D vector.
  std::string toString() const { return std::format("({}, {})", x, y); }
};

/// A three-dimensional vector containing values of type T.
///
/// @tparam T The type of each vector component.
template <typename T> class Vector3 {
public:
  T x, y, z;
  /// Constructs a zero 3D vector.
  Vector3() : x(0), y(0), z(0) {}
  /// Constructs a 3D vector from three components.
  ///
  /// @param x The X component.
  /// @param y The Y component.
  /// @param z The Z component.
  Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

  /// Constructs a 3D vector from a 2D vector and a z-component.
  ///
  /// @param v The 2D vector containing X and Y components.
  /// @param z The Z component.
  Vector3(const Vector2<T> &v, T z) : x(v.x), y(v.y), z(z) {}

  /// Constructs a 3D vector from another 3D vector of a different type.
  ///
  /// @tparam U The type of the other vector's components.
  /// @param other The other vector.
  /// @note This constructor allows for implicit conversion between different
  /// scalar types, such as from Vector3<int> to Vector3<float>.
  template <typename U>
  Vector3(const Vector3<U> &other)
      : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)),
        z(static_cast<T>(other.z)) {}

  /// Adds two 3D vectors together.
  ///
  /// @param v The other 3D vector.
  /// @return The resulting 3D vector.
  Vector3 operator+(const Vector3 &v) const
    requires Addable<T>
  {
    return Vector3(x + v.x, y + v.y, z + v.z);
  }

  /// Adds a scalar value to each component of this 3D vector.
  ///
  /// @param f The scalar value to add.
  /// @return The resulting 3D vector.
  Vector3 operator+(const T &f) const
    requires Addable<T>
  {
    return Vector3(x + f, y + f, z + f);
  }

  /// Negates the 3D vector.
  ///
  /// @return The negated 3D vector.
  Vector3 operator-() const
    requires Negatable<T>
  {
    return Vector3(-x, -y, -z);
  }

  /// Subtracts a 3D vector from this 3D vector.
  ///
  /// @param v The 3D vector to subtract.
  /// @return The resulting 3D vector.
  Vector3 operator-(const Vector3 &v) const
    requires Subtractable<T>
  {
    return Vector3(x - v.x, y - v.y, z - v.z);
  }

  /// Subtracts a scalar value from each component of this 3D vector.
  ///
  /// @param f The scalar value to subtract.
  /// @return The resulting 3D vector.
  Vector3 operator-(const T &f) const
    requires Subtractable<T>
  {
    return Vector3(x - f, y - f, z - f);
  }

  /// Multiplies each component of the 3D vector by a scalar.
  ///
  /// @param f The scalar value to multiply by.
  /// @return The resulting 3D vector.
  Vector3 operator*(const T &f) const
    requires Multipliable<T>
  {
    return Vector3(x * f, y * f, z * f);
  }

  /// Multiplies a scalar value by a 3D vector.
  ///
  /// @param f The scalar value to multiply by.
  /// @param v The 3D vector to multiply.
  /// @return The resulting 3D vector.
  friend Vector3 operator*(const T &f, const Vector3 &v)
    requires Multipliable<T>
  {
    return Vector3(v.x * f, v.y * f, v.z * f);
  }

  /// Divides each component of the 3D vector by a scalar.
  ///
  /// @param f The scalar value to divide by.
  /// @return The resulting 3D vector.
  Vector3 operator/(const T &f) const
    requires Divisible<T>
  {
    return Vector3(x / f, y / f, z / f);
  }

  /// Adds another 3D vector to this 3D vector in place.
  ///
  /// @param v The 3D vector to add.
  /// @return A reference to this 3D vector.
  Vector3 operator+=(const Vector3 &v)
    requires AddAssignable<T>
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  /// Subtracts another 3D vector from this 3D vector in place.
  ///
  /// @param v The 3D vector to subtract.
  /// @return A reference to this 3D vector.
  Vector3 operator-=(const Vector3 &v)
    requires SubtractAssignable<T>
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  /// Computes the dot product of the 3D vector with another 3D vector.
  ///
  /// @param v The other 3D vector.
  /// @return The dot product of the two vectors.
  T dot(const Vector3 &v) const
    requires Multipliable<T> && Addable<T>
  {
    return x * v.x + y * v.y + z * v.z;
  }

  /// Computes the cross product of the 3D vector with another 3D vector.
  ///
  /// @param v The other 3D vector.
  /// @return The cross product of the two vectors.
  Vector3 cross(const Vector3 &v) const
    requires Multipliable<T> && Subtractable<T>
  {
    return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
  }

  /// Computes the magnitude (length) of the 3D vector.
  ///
  /// @return The magnitude of the 3D vector.
  T magnitude() const
    requires Multipliable<T> && Addable<T>
  {
    return sqrt(x * x + y * y + z * z);
  }

  /// Normalizes the 3D vector to have a magnitude of 1.
  ///
  /// @note If the 3D vector has a magnitude of 0, the original vector is
  /// returned.
  /// @return The normalized 3D vector.
  Vector3 normalize() const
    requires Divisible<T> && Multipliable<T>
  {
    T mag = magnitude();

    if (mag != T(0)) {
      T inv_m = T(1) / mag;
      return Vector3(x * inv_m, y * inv_m, z * inv_m);
    }

    return *this;
  }
  /// Outputs the 3D vector to an output stream.
  ///
  /// @param os The output stream.
  /// @param v The 3D vector to output.
  /// @return The output stream.
  friend std::ostream &operator<<(std::ostream &os, const Vector3 &v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
  }
};

/// A four-dimensional vector containing values of type T.
///
/// @tparam T The type of each vector component.
template <typename T> class Vector4 {
public:
  T x, y, z, t;

  /// Constructs a zero 4D vector.
  Vector4() : x(0), y(0), z(0), t(0) {}

  /// Constructs a 4D vector from four components.
  ///
  /// @param x The X component.
  /// @param y The Y component.
  /// @param z The Z component.
  /// @param t The T component.
  Vector4(T x, T y, T z, T t) : x(x), y(y), z(z), t(t) {}

  /// Constructs a 4D vector from a 3D vector and a t-component.
  ///
  /// @param v The 3D vector containing X, Y, and Z components.
  /// @param t The T component.
  Vector4(const Vector3<T> &v, T t) : x(v.x), y(v.y), z(v.z), t(t) {}

  /// Constructs a 4D vector from another 4D vector of a different type.
  ///
  /// @tparam U The type of the other vector's components.
  /// @param other The other vector.
  template <typename U>
  Vector4(const Vector4<U> &other)
      : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)),
        z(static_cast<T>(other.z)), t(static_cast<T>(other.t)) {}

  /// Adds two 4D vectors together.
  ///
  /// @param v The other 4D vector.
  /// @return The resulting 4D vector.
  Vector4 operator+(const Vector4 &v) const
    requires Addable<T>
  {
    return Vector4(x + v.x, y + v.y, z + v.z, t + v.t);
  }

  /// Adds a scalar value to each component of this 4D vector.
  ///
  /// @param f The scalar value to add.
  /// @return The resulting 4D vector.
  Vector4 operator+(const T &f) const
    requires Addable<T>
  {
    return Vector4(x + f, y + f, z + f, t + f);
  }

  /// Negates the 4D vector.
  ///
  /// @return The negated 4D vector.
  Vector4 operator-() const
    requires Negatable<T>
  {
    return Vector4(-x, -y, -z, -t);
  }

  /// Subtracts a 4D vector from this 4D vector.
  ///
  /// @param v The 4D vector to subtract.
  /// @return The resulting 4D vector.
  Vector4 operator-(const Vector4 &v) const
    requires Subtractable<T>
  {
    return Vector4(x - v.x, y - v.y, z - v.z, t - v.t);
  }

  /// Subtracts a scalar value from each component of this 4D vector.
  ///
  /// @param f The scalar value to subtract.
  /// @return The resulting 4D vector.
  Vector4 operator-(const T &f) const
    requires Subtractable<T>
  {
    return Vector4(x - f, y - f, z - f, t - f);
  }

  /// Multiplies each component of the 4D vector by a scalar value.
  ///
  /// @param f The scalar value to multiply by.
  /// @return The resulting 4D vector.
  Vector4 operator*(const T &f) const
    requires Multipliable<T>
  {
    return Vector4(x * f, y * f, z * f, t * f);
  }

  /// Multiplies a scalar value by a 4D vector.
  ///
  /// @param f The scalar value to multiply by.
  /// @param v The 4D vector to multiply.
  /// @return The resulting 4D vector.
  friend Vector4 operator*(const T &f, const Vector4 &v)
    requires Multipliable<T>
  {
    return Vector4(v.x * f, v.y * f, v.z * f, v.t * f);
  }

  /// Adds another 4D vector to this 4D vector in place.
  ///
  /// @param v The 4D vector to add.
  /// @return A reference to this 4D vector.
  Vector4 operator+=(const Vector4 &v)
    requires AddAssignable<T>
  {
    x += v.x;
    y += v.y;
    z += v.z;
    t += v.t;
    return *this;
  }

  /// Subtracts another 4D vector from this 4D vector in place.
  ///
  /// @param v The 4D vector to subtract.
  /// @return A reference to this 4D vector.
  Vector4 operator-=(const Vector4 &v)
    requires SubtractAssignable<T>
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    t -= v.t;
    return *this;
  }

  /// Computes the dot product of this 4D vector and another 4D vector.
  ///
  /// @param v The other 4D vector.
  /// @return The dot product of the two vectors.
  T dot(const Vector4 &v) const
    requires Multipliable<T> && Addable<T>
  {
    return x * v.x + y * v.y + z * v.z + t * v.t;
  }

  /// Computes the magnitude (length) of the 4D vector.
  ///
  /// @return The magnitude of the 4D vector.
  T magnitude() const
    requires Multipliable<T> && Addable<T>
  {
    return sqrt(x * x + y * y + z * z + t * t);
  }

  /// Normalizes the 4D vector to have a magnitude of 1.
  ///
  /// @note If the 4D vector has a magnitude of 0, the original vector is
  ///       returned unchanged.
  /// @return The normalized 4D vector.
  Vector4 normalize() const
    requires Divisible<T> && Multipliable<T>
  {
    T mag = magnitude();

    if (mag != T(0)) {
      T inv_m = T(1) / mag;
      return Vector4(x * inv_m, y * inv_m, z * inv_m, t * inv_m);
    }

    return *this;
  }

  /// Outputs the 4D vector to an output stream.
  ///
  /// @param os The output stream.
  /// @param v The 4D vector to output.
  /// @return The output stream.
  friend std::ostream &operator<<(std::ostream &os, const Vector4 &v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.t << ")";
    return os;
  }
};

/// A 2x2 matrix containing values of type T.
///
/// @tparam T The type of each matrix element.
template <typename T> class Matrix2 {
public:
  /// The 2x2 array storing matrix elements.
  T m[2][2] = {{1, 0}, {0, 1}};

  /// Constructs an identity 2x2 matrix.
  Matrix2() {}

  /// Negates each element of the 2x2 matrix.
  ///
  /// @return The negated matrix.
  Matrix2 operator-() const
    requires Negatable<T>
  {
    Matrix2 result;
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        result.m[i][j] = -m[i][j];
    return result;
  }

  /// Adds two 2x2 matrices element-wise.
  ///
  /// @param mat The other 2x2 matrix.
  /// @return The resulting 2x2 matrix.
  Matrix2 operator+(const Matrix2 &mat) const
    requires Addable<T>
  {
    Matrix2 result;
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        result.m[i][j] = m[i][j] + mat.m[i][j];
    return result;
  }

  /// Subtracts another 2x2 matrix from this matrix element-wise.
  ///
  /// @param mat The 2x2 matrix to subtract.
  /// @return The resulting 2x2 matrix.
  Matrix2 operator-(const Matrix2 &mat) const
    requires Subtractable<T>
  {
    Matrix2 result;
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        result.m[i][j] = m[i][j] - mat.m[i][j];
    return result;
  }

  /// Multiplies two 2x2 matrices.
  ///
  /// @param mat The other 2x2 matrix.
  /// @return The resulting 2x2 matrix.
  Matrix2 operator*(const Matrix2 &mat) const
    requires Multipliable<T>
  {
    Matrix2 result;
    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        result.m[i][j] = 0;
        for (int k = 0; k < 2; k++) {
          result.m[i][j] += m[i][k] * mat.m[k][j];
        }
      }
    }
    return result;
  }

  /// Multiplies this matrix by another 2x2 matrix in place.
  ///
  /// @param mat The 2x2 matrix to multiply by.
  /// @return A reference to this 2x2 matrix.
  Matrix2 &operator*=(const Matrix2 &mat)
    requires Multipliable<T>
  {
    *this = (*this) * mat;
    return *this;
  }

  /// Applies a scaling transformation to this matrix.
  ///
  /// @param v A 2D vector containing scale factors for X and Y axes.
  /// @return A reference to this 2x2 matrix.
  Matrix2 &scale(const Vector2<T> &v)
    requires Multipliable<T>
  {
    Matrix2 scaleMatrix;
    scaleMatrix.m[0][0] = v.x;
    scaleMatrix.m[1][1] = v.y;

    *this *= scaleMatrix;
    return *this;
  }

  /// Applies a uniform scaling transformation to this matrix.
  ///
  /// @param value The scale factor for both X and Y axes.
  /// @return A reference to this 2x2 matrix.
  Matrix2 &scale(T value)
    requires Multipliable<T>
  {
    return scale(Vector2<T>(value, value));
  }

  /// Applies a non-uniform scaling transformation to this matrix.
  ///
  /// @param x The scale factor for the X axis.
  /// @param y The scale factor for the Y axis.
  /// @return A reference to this 2x2 matrix.
  Matrix2 &scale(T x, T y)
    requires Multipliable<T>
  {
    return scale(Vector2<T>(x, y));
  }

  /// Applies a rotation transformation to this matrix.
  ///
  /// @param angle The rotation angle in radians.
  /// @return A reference to this 2x2 matrix.
  Matrix2 &rotate(T angle)
    requires Multipliable<T>
  {
    Matrix2 rotationMatrix;
    rotationMatrix.m[0][0] = cos(angle);
    rotationMatrix.m[0][1] = -sin(angle);
    rotationMatrix.m[1][0] = sin(angle);
    rotationMatrix.m[1][1] = cos(angle);

    *this *= rotationMatrix;
    return *this;
  }

  /// Multiplies this 2x2 matrix by a 2D vector.
  ///
  /// @param v The 2D vector to multiply.
  /// @return The resulting 2D vector.
  Vector2<T> operator*(const Vector2<T> &v) const
    requires Multipliable<T> && Addable<T>
  {
    return Vector2(
        m[0][0] * v.x + m[0][1] * v.y, m[1][0] * v.x + m[1][1] * v.y
    );
  }

  /// Outputs the 2x2 matrix to an output stream.
  ///
  /// @param os The output stream.
  /// @param mat The 2x2 matrix to output.
  /// @return The output stream.
  friend std::ostream &operator<<(std::ostream &os, const Matrix2 &mat) {
    os << "[" << mat.m[0][0] << ", " << mat.m[0][1] << "]" << std::endl;
    os << "[" << mat.m[1][0] << ", " << mat.m[1][1] << "]" << std::endl;
    return os;
  }
};

/// A 3x3 matrix containing values of type T.
///
/// @tparam T The type of each matrix element.
template <typename T> class Matrix3 {
public:
  /// The 3x3 array storing matrix elements.
  T m[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

  /// Constructs an identity 3x3 matrix.
  Matrix3() {}

  /// Negates each element of the 3x3 matrix.
  ///
  /// @return The negated matrix.
  Matrix3 operator-() const
    requires Negatable<T>
  {
    Matrix3 result;
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        result.m[i][j] = -m[i][j];
    return result;
  }

  /// Adds two 3x3 matrices element-wise.
  ///
  /// @param mat The other 3x3 matrix.
  /// @return The resulting 3x3 matrix.
  Matrix3 operator+(const Matrix3 &mat) const
    requires Addable<T>
  {
    Matrix3 result;
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        result.m[i][j] = m[i][j] + mat.m[i][j];
    return result;
  }

  /// Subtracts another 3x3 matrix from this matrix element-wise.
  ///
  /// @param mat The 3x3 matrix to subtract.
  /// @return The resulting 3x3 matrix.
  Matrix3 operator-(const Matrix3 &mat) const
    requires Subtractable<T>
  {
    Matrix3 result;
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        result.m[i][j] = m[i][j] - mat.m[i][j];
    return result;
  }

  /// Multiplies two 3x3 matrices.
  ///
  /// @param mat The other 3x3 matrix.
  /// @return The resulting 3x3 matrix.
  Matrix3 operator*(const Matrix3 &mat) const
    requires Multipliable<T>
  {
    Matrix3 result;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        result.m[i][j] = 0;
        for (int k = 0; k < 3; k++) {
          result.m[i][j] += m[i][k] * mat.m[k][j];
        }
      }
    }
    return result;
  }

  /// Multiplies this 3x3 matrix by a 3D vector.
  ///
  /// @param v The 3D vector to multiply.
  /// @return The resulting 3D vector.
  Vector3<T> operator*(const Vector3<T> &v) const
    requires Multipliable<T> && Addable<T>
  {
    return Vector3(
        m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
        m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
        m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
    );
  }

  /// Multiplies this matrix by another 3x3 matrix in place.
  ///
  /// @param mat The 3x3 matrix to multiply by.
  /// @return A reference to this 3x3 matrix.
  Matrix3 &operator*=(const Matrix3 &mat)
    requires Multipliable<T>
  {
    *this = (*this) * mat;
    return *this;
  }

  /// Inverts the matrix in-place.
  ///
  /// If the matrix is singular (determinant is 0), the matrix remains
  /// unmodified.
  ///
  /// @return A reference to this inverted 3x3 matrix.
  Matrix3 &inverse()
    requires Multipliable<T> && Addable<T> && Subtractable<T> && Divisible<T>
  {
    Matrix3 inv;

    T *a = &m[0][0];
    T *invOut = &inv.m[0][0];

    invOut[0] = a[4] * a[8] - a[5] * a[7];
    invOut[1] = -(a[1] * a[8] - a[2] * a[7]);
    invOut[2] = a[1] * a[5] - a[2] * a[4];

    invOut[3] = -(a[3] * a[8] - a[5] * a[6]);
    invOut[4] = a[0] * a[8] - a[2] * a[6];
    invOut[5] = -(a[0] * a[5] - a[2] * a[3]);

    invOut[6] = a[3] * a[7] - a[4] * a[6];
    invOut[7] = -(a[0] * a[7] - a[1] * a[6]);
    invOut[8] = a[0] * a[4] - a[1] * a[3];

    T det = a[0] * invOut[0] + a[1] * invOut[3] + a[2] * invOut[6];

    if (det == T(0)) {
      return *this; // no invertible
    }

    det = T(1) / det;

    for (int i = 0; i < 9; i++)
      invOut[i] *= det;

    *this = inv;
    return *this;
  }

  /// Applies a translation transformation using a 2D vector.
  ///
  /// @param v The 2D translation vector.
  /// @return A reference to this 3x3 matrix.
  Matrix3 &translate(const Vector2<T> &v)
    requires Multipliable<T>
  {
    Matrix3 translationMatrix;
    translationMatrix.m[0][2] = v.x;
    translationMatrix.m[1][2] = v.y;

    *this *= translationMatrix;
    return *this;
  }

  /// Applies a scaling transformation using a 2D vector.
  ///
  /// @param v A 2D vector containing scale factors for X and Y axes.
  /// @return A reference to this 3x3 matrix.
  Matrix3 &scale(const Vector2<T> &v)
    requires Multipliable<T>
  {
    Matrix3 scaleMatrix;
    scaleMatrix.m[0][0] = v.x;
    scaleMatrix.m[1][1] = v.y;

    *this *= scaleMatrix;
    return *this;
  }

  /// Applies a uniform scaling transformation to this matrix.
  ///
  /// @param value The scale factor for both X and Y axes.
  /// @return A reference to this 3x3 matrix.
  Matrix3 &scale(T value)
    requires Multipliable<T>
  {
    return scale(Vector2<T>(value, value));
  }

  /// Applies a non-uniform scaling transformation to this matrix.
  ///
  /// @param x The scale factor for the X axis.
  /// @param y The scale factor for the Y axis.
  /// @return A reference to this 3x3 matrix.
  Matrix3 &scale(T x, T y)
    requires Multipliable<T>
  {
    return scale(Vector2<T>(x, y));
  }

  /// Applies a rotation transformation to this matrix around the Z axis.
  ///
  /// @param angle The rotation angle in radians.
  /// @return A reference to this 3x3 matrix.
  Matrix3 &rotate(const T &angle)
    requires Multipliable<T>
  {
    Matrix3 rotationMatrix;
    rotationMatrix.m[0][0] = cos(angle);
    rotationMatrix.m[0][1] = -sin(angle);
    rotationMatrix.m[1][0] = sin(angle);
    rotationMatrix.m[1][1] = cos(angle);

    *this *= rotationMatrix;
    return *this;
  }

  /// Outputs the 3x3 matrix to an output stream.
  ///
  /// @param os The output stream.
  /// @param mat The 3x3 matrix to output.
  /// @return The output stream.
  friend std::ostream &operator<<(std::ostream &os, const Matrix3 &mat) {
    os << "[" << mat.m[0][0] << ", " << mat.m[0][1] << ", " << mat.m[0][2]
       << "]" << std::endl;
    os << "[" << mat.m[1][0] << ", " << mat.m[1][1] << ", " << mat.m[1][2]
       << "]" << std::endl;
    os << "[" << mat.m[2][0] << ", " << mat.m[2][1] << ", " << mat.m[2][2]
       << "]" << std::endl;
    return os;
  }
};

/// A 4x4 matrix containing values of type T.
///
/// @tparam T The type of each matrix element.
template <typename T> class Matrix4 {
public:
  /// The 4x4 array storing matrix elements.
  T m[4][4];

  /// Constructs a 4x4 identity matrix.
  Matrix4() { setIdentity(); }

  /// Creates a zero 4x4 matrix.
  ///
  /// @return A 4x4 matrix with all elements set to 0.
  static Matrix4 zero() {
    Matrix4 mat;
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        mat.m[i][j] = T(0);
    return mat;
  }

  /// Creates an identity 4x4 matrix.
  ///
  /// @return A 4x4 identity matrix.
  static Matrix4 identity() {
    Matrix4 mat;
    mat.setIdentity();
    return mat;
  }

  /// Creates a diagonal 4x4 matrix with the specified value on the main
  /// diagonal.
  ///
  /// @param value The value to place on the diagonal elements.
  /// @return The diagonal matrix.
  static Matrix4 diagonal(T value) {
    Matrix4 mat = zero();
    for (int i = 0; i < 4; i++)
      mat.m[i][i] = value;
    return mat;
  }

  /// Constructs a 4x4 matrix from an initializer list.
  ///
  /// @param values The initializer list of 16 values in row-major order.
  Matrix4(std::initializer_list<T> values) {
    auto it = values.begin();
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        m[i][j] = (it != values.end()) ? *it++ : T(0);
  }

  /// Creates an orthographic projection matrix.
  ///
  /// @param left Coordinate for the left vertical clipping plane.
  /// @param right Coordinate for the right vertical clipping plane.
  /// @param bottom Coordinate for the bottom horizontal clipping plane.
  /// @param top Coordinate for the top horizontal clipping plane.
  /// @param near Distance to the near depth clipping plane.
  /// @param far Distance to the far depth clipping plane.
  /// @return The orthographic projection matrix.
  static Matrix4 ortho(T left, T right, T bottom, T top, T near, T far)
    requires Divisible<T> && Subtractable<T>
  {
    Matrix4 mat = zero();
    mat.m[0][0] = T(2) / (right - left);
    mat.m[1][1] = T(2) / (top - bottom);
    mat.m[2][2] = -T(2) / (far - near);
    mat.m[3][3] = T(1);

    mat.m[0][3] = -(right + left) / (right - left);
    mat.m[1][3] = -(top + bottom) / (top - bottom);
    mat.m[2][3] = -(far + near) / (far - near);

    return mat;
  }

  /// Creates a perspective projection matrix.
  ///
  /// @param fov The field of view angle in radians.
  /// @param aspect The aspect ratio (width divided by height).
  /// @param near Distance to the near depth clipping plane.
  /// @param far Distance to the far depth clipping plane.
  /// @return The perspective projection matrix.
  static Matrix4 perspective(T fov, T aspect, T near, T far)
    requires Divisible<T> && Subtractable<T> && Multipliable<T>
  {
    Matrix4 mat = zero();
    T tanHalfFov = tan(fov / T(2));
    mat.m[0][0] = T(1) / (aspect * tanHalfFov);
    mat.m[1][1] = T(1) / tanHalfFov;
    mat.m[2][2] = -(far + near) / (far - near);
    mat.m[2][3] = -(T(2) * far * near) / (far - near);
    mat.m[3][2] = -T(1);
    return mat;
  }

  /// Applies a look-at transformation to this matrix.
  ///
  /// @brief This transformation positions and orients the camera in 3D space.
  /// @param eye The position of the camera.
  /// @param center The point the camera is looking at.
  /// @param up The up direction vector.
  /// @return A reference to this 4x4 matrix.
  static Matrix4
  lookAt(const Vector3<T> &eye, const Vector3<T> &center, const Vector3<T> &up)
    requires Multipliable<T> && Addable<T> && Subtractable<T> && Divisible<T>
  {
    Vector3<T> f = (center - eye).normalize();
    Vector3<T> s = f.cross(up).normalize();
    Vector3<T> u = s.cross(f);

    Matrix4 result = identity();

    // Row 0: Vector Right (s)
    result.m[0][0] = s.x;
    result.m[0][1] = s.y;
    result.m[0][2] = s.z;

    // Row 1: Vector Up (u)
    result.m[1][0] = u.x;
    result.m[1][1] = u.y;
    result.m[1][2] = u.z;

    // Row 2: Vector Forward inverted (-f)
    result.m[2][0] = -f.x;
    result.m[2][1] = -f.y;
    result.m[2][2] = -f.z;

    result.translate(-eye);

    return result;
  }

  /// Sets this matrix to the identity matrix.
  void setIdentity() {
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        m[i][j] = (i == j) ? T(1) : T(0);
  }

  /// Negates each element of this 4x4 matrix.
  ///
  /// @return The negated matrix.
  Matrix4 operator-() const
    requires Negatable<T>
  {
    Matrix4 result;
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        result.m[i][j] = -m[i][j];
    return result;
  }

  /// Adds two 4x4 matrices element-wise.
  ///
  /// @param mat The other 4x4 matrix.
  /// @return The resulting 4x4 matrix.
  Matrix4 operator+(const Matrix4 &mat) const
    requires Addable<T>
  {
    Matrix4 result;
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        result.m[i][j] = m[i][j] + mat.m[i][j];
    return result;
  }

  /// Subtracts another 4x4 matrix from this matrix element-wise.
  ///
  /// @param mat The 4x4 matrix to subtract.
  /// @return The resulting 4x4 matrix.
  Matrix4 operator-(const Matrix4 &mat) const
    requires Subtractable<T>
  {
    Matrix4 result;
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        result.m[i][j] = m[i][j] - mat.m[i][j];
    return result;
  }

  /// Multiplies two 4x4 matrices.
  ///
  /// @param mat The other 4x4 matrix.
  /// @return The resulting 4x4 matrix.
  Matrix4 operator*(const Matrix4 &mat) const
    requires Multipliable<T>
  {
    Matrix4 result;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        result.m[i][j] = 0;
        for (int k = 0; k < 4; k++) {
          result.m[i][j] += m[i][k] * mat.m[k][j];
        }
      }
    }
    return result;
  }

  /// Multiplies this matrix by another 4x4 matrix in place.
  ///
  /// @param mat The 4x4 matrix to multiply by.
  /// @return A reference to this matrix.
  Matrix4 &operator*=(const Matrix4 &mat)
    requires Multipliable<T>
  {
    *this = (*this) * mat;
    return *this;
  }

  /// Multiplies this 4x4 matrix by a 4D vector.
  ///
  /// @param v The 4D vector to multiply.
  /// @return The resulting 4D vector.
  Vector4<T> operator*(const Vector4<T> &v) const
    requires Multipliable<T> && Addable<T>
  {
    return Vector4<T>(
        m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.t,
        m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.t,
        m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.t,
        m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.t
    );
  }

  /// Inverts the matrix in-place.
  ///
  /// If the matrix is singular (determinant is 0), the matrix remains
  /// unmodified.
  ///
  /// @return A reference to this inverted 4x4 matrix.
  Matrix4 &inverse()
    requires Multipliable<T> && Addable<T> && Subtractable<T> && Divisible<T>
  {
    Matrix4 inv;
    T *invOut = &inv.m[0][0];
    const T *a = &m[0][0];

    invOut[0] = a[5] * a[10] * a[15] - a[5] * a[11] * a[14] -
                a[9] * a[6] * a[15] + a[9] * a[7] * a[14] +
                a[13] * a[6] * a[11] - a[13] * a[7] * a[10];

    invOut[4] = -a[4] * a[10] * a[15] + a[4] * a[11] * a[14] +
                a[8] * a[6] * a[15] - a[8] * a[7] * a[14] -
                a[12] * a[6] * a[11] + a[12] * a[7] * a[10];

    invOut[8] = a[4] * a[9] * a[15] - a[4] * a[11] * a[13] -
                a[8] * a[5] * a[15] + a[8] * a[7] * a[13] +
                a[12] * a[5] * a[11] - a[12] * a[7] * a[9];

    invOut[12] = -a[4] * a[9] * a[14] + a[4] * a[10] * a[13] +
                 a[8] * a[5] * a[14] - a[8] * a[6] * a[13] -
                 a[12] * a[5] * a[10] + a[12] * a[6] * a[9];

    invOut[1] = -a[1] * a[10] * a[15] + a[1] * a[11] * a[14] +
                a[9] * a[2] * a[15] - a[9] * a[3] * a[14] -
                a[13] * a[2] * a[11] + a[13] * a[3] * a[10];

    invOut[5] = a[0] * a[10] * a[15] - a[0] * a[11] * a[14] -
                a[8] * a[2] * a[15] + a[8] * a[3] * a[14] +
                a[12] * a[2] * a[11] - a[12] * a[3] * a[10];

    invOut[9] = -a[0] * a[9] * a[15] + a[0] * a[11] * a[13] +
                a[8] * a[1] * a[15] - a[8] * a[3] * a[13] -
                a[12] * a[1] * a[11] + a[12] * a[3] * a[9];

    invOut[13] = a[0] * a[9] * a[14] - a[0] * a[10] * a[13] -
                 a[8] * a[1] * a[14] + a[8] * a[2] * a[13] +
                 a[12] * a[1] * a[10] - a[12] * a[2] * a[9];

    invOut[2] = a[1] * a[6] * a[15] - a[1] * a[7] * a[14] -
                a[5] * a[2] * a[15] + a[5] * a[3] * a[14] +
                a[13] * a[2] * a[7] - a[13] * a[3] * a[6];

    invOut[6] = -a[0] * a[6] * a[15] + a[0] * a[7] * a[14] +
                a[4] * a[2] * a[15] - a[4] * a[3] * a[14] -
                a[12] * a[2] * a[7] + a[12] * a[3] * a[6];

    invOut[10] = a[0] * a[5] * a[15] - a[0] * a[7] * a[13] -
                 a[4] * a[1] * a[15] + a[4] * a[3] * a[13] +
                 a[12] * a[1] * a[7] - a[12] * a[3] * a[5];

    invOut[14] = -a[0] * a[5] * a[14] + a[0] * a[6] * a[13] +
                 a[4] * a[1] * a[14] - a[4] * a[2] * a[13] -
                 a[12] * a[1] * a[6] + a[12] * a[2] * a[5];

    invOut[3] = -a[1] * a[6] * a[11] + a[1] * a[7] * a[10] +
                a[5] * a[2] * a[11] - a[5] * a[3] * a[10] - a[9] * a[2] * a[7] +
                a[9] * a[3] * a[6];

    invOut[7] = a[0] * a[6] * a[11] - a[0] * a[7] * a[10] -
                a[4] * a[2] * a[11] + a[4] * a[3] * a[10] + a[8] * a[2] * a[7] -
                a[8] * a[3] * a[6];

    invOut[11] = -a[0] * a[5] * a[11] + a[0] * a[7] * a[9] +
                 a[4] * a[1] * a[11] - a[4] * a[3] * a[9] - a[8] * a[1] * a[7] +
                 a[8] * a[3] * a[5];

    invOut[15] = a[0] * a[5] * a[10] - a[0] * a[6] * a[9] -
                 a[4] * a[1] * a[10] + a[4] * a[2] * a[9] + a[8] * a[1] * a[6] -
                 a[8] * a[2] * a[5];

    T det = a[0] * invOut[0] + a[1] * invOut[4] + a[2] * invOut[8] +
            a[3] * invOut[12];

    if (det == T(0)) {
      return *this;
    }

    det = T(1) / det;

    for (int i = 0; i < 16; i++)
      invOut[i] *= det;

    *this = inv;
    return *this;
  }

  /// Applies a translation transformation to this matrix using a 3D vector.
  ///
  /// @param v The 3D translation vector.
  /// @return A reference to this 4x4 matrix.
  Matrix4 &translate(const Vector3<T> &v)
    requires Multipliable<T>
  {
    Matrix4 translationMatrix;
    translationMatrix.m[0][3] = v.x;
    translationMatrix.m[1][3] = v.y;
    translationMatrix.m[2][3] = v.z;

    *this *= translationMatrix;
    return *this;
  }

  /// Applies a translation transformation to this matrix using a 2D vector.
  ///
  /// @param v The 2D translation vector.
  /// @return A reference to this 4x4 matrix.
  Matrix4 &translate(const Vector2<T> &v)
    requires Multipliable<T>
  {
    return translate(Vector3<T>(v, 0));
  }

  /// Applies a translation transformation to this matrix.
  ///
  /// @param x The translation component along the X axis.
  /// @param y The translation component along the Y axis.
  /// @param z The translation component along the Z axis.
  /// @return A reference to this 4x4 matrix.
  Matrix4 &translate(T x, T y, T z)
    requires Multipliable<T>
  {
    return translate(Vector3<T>(x, y, z));
  }

  /// Applies a scaling transformation using a 3D vector.
  ///
  /// @param v A 3D vector containing scale factors for X, Y, and Z axes.
  /// @return A reference to this 4x4 matrix.
  Matrix4 &scale(const Vector3<T> &v)
    requires Multipliable<T>
  {
    Matrix4 scaleMatrix;
    scaleMatrix.m[0][0] = v.x;
    scaleMatrix.m[1][1] = v.y;
    scaleMatrix.m[2][2] = v.z;

    *this *= scaleMatrix;
    return *this;
  }

  /// Applies a scaling transformation using a 2D vector (Z scaling remains 1).
  ///
  /// @param v A 2D vector containing scale factors for X and Y axes.
  /// @return A reference to this 4x4 matrix.
  Matrix4 &scale(const Vector2<T> &v)
    requires Multipliable<T>
  {
    return scale(Vector3<T>(v, 0));
  }

  /// Applies a non-uniform scaling transformation.
  ///
  /// @param x The scale factor for the X axis.
  /// @param y The scale factor for the Y axis.
  /// @param z The scale factor for the Z axis.
  /// @return A reference to this 4x4 matrix.
  Matrix4 &scale(T x, T y, T z)
    requires Multipliable<T>
  {
    return scale(Vector3<T>(x, y, z));
  }

  /// Applies a uniform scaling transformation to this matrix.
  ///
  /// @param value The scale factor for X, Y, and Z axes.
  /// @return A reference to this 4x4 matrix.
  Matrix4 &scale(T value)
    requires Multipliable<T>
  {
    return scale(Vector3<T>(value, value, value));
  }

  /// Applies a rotation transformation around the X axis.
  ///
  /// @param angle The rotation angle in radians.
  /// @return A reference to this 4x4 matrix.
  Matrix4 &rotateX(const T &angle)
    requires Multipliable<T>
  {
    Matrix4 rotationMatrix;
    rotationMatrix.m[1][1] = cos(angle);
    rotationMatrix.m[1][2] = -sin(angle);
    rotationMatrix.m[2][1] = sin(angle);
    rotationMatrix.m[2][2] = cos(angle);

    *this *= rotationMatrix;
    return *this;
  }

  /// Applies a rotation transformation around the Y axis.
  ///
  /// @param angle The rotation angle in radians.
  /// @return A reference to this 4x4 matrix.
  Matrix4 &rotateY(const T &angle)
    requires Multipliable<T>
  {
    Matrix4 rotationMatrix;
    rotationMatrix.m[0][0] = cos(angle);
    rotationMatrix.m[0][2] = sin(angle);
    rotationMatrix.m[2][0] = -sin(angle);
    rotationMatrix.m[2][2] = cos(angle);

    *this *= rotationMatrix;
    return *this;
  }

  /// Applies a rotation transformation around the Z axis.
  ///
  /// @param angle The rotation angle in radians.
  /// @return A reference to this 4x4 matrix.
  Matrix4 &rotateZ(const T &angle)
    requires Multipliable<T>
  {
    Matrix4 rotationMatrix;
    rotationMatrix.m[0][0] = cos(angle);
    rotationMatrix.m[0][1] = -sin(angle);
    rotationMatrix.m[1][0] = sin(angle);
    rotationMatrix.m[1][1] = cos(angle);

    *this *= rotationMatrix;
    return *this;
  }

  /// Outputs the 4x4 matrix to an output stream.
  ///
  /// @param os The output stream.
  /// @param mat The 4x4 matrix to output.
  /// @return The output stream.
  friend std::ostream &operator<<(std::ostream &os, const Matrix4 &mat) {
    os << "[" << mat.m[0][0] << ", " << mat.m[0][1] << ", " << mat.m[0][2]
       << ", " << mat.m[0][3] << "]" << std::endl;
    os << "[" << mat.m[1][0] << ", " << mat.m[1][1] << ", " << mat.m[1][2]
       << ", " << mat.m[1][3] << "]" << std::endl;
    os << "[" << mat.m[2][0] << ", " << mat.m[2][1] << ", " << mat.m[2][2]
       << ", " << mat.m[2][3] << "]" << std::endl;
    os << "[" << mat.m[3][0] << ", " << mat.m[3][1] << ", " << mat.m[3][2]
       << ", " << mat.m[3][3] << "]" << std::endl;
    return os;
  }
};
} // namespace cass::linear
