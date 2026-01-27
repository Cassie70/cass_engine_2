#pragma once

#include <iostream>
#include <cmath>

namespace cass
{
    template <typename T>
    class Vector2
    {
    public:
        T x, y;
        Vector2() : x(0), y(0) {}
        Vector2(T x, T y) : x(x), y(y) {}

        Vector2 operator+(const Vector2 &v) const { return Vector2(x + v.x, y + v.y); }
        Vector2 operator+(const T &f) const { return Vector2(x + f, y + f); }
		Vector2 operator-() const { return Vector2(-x, -y); }
        Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
        Vector2 operator-(const T& f) const { return Vector2(x - f, y - f); }
        Vector2 operator*(const T& f) const { return Vector2(x * f, y * f); }

        Vector2 operator+=(const Vector2 &v)
        {
            x += v.x;
            y += v.y;
            return *this;
		}
        Vector2 operator-= (const Vector2& v)
        {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        T dot(const Vector2 &v) const { return x * v.x + y * v.y; }
        T magnitude() const { return sqrt(x * x + y * y); }

        Vector2 normalize() const
        {
            T inv_m = 1 / magnitude();
            return Vector2(x * inv_m, y * inv_m);
        }

        friend std::ostream &operator<<(std::ostream &os, const Vector2 &v)
        {
            os << "(" << v.x << ", " << v.y << ")";
            return os;
        }
    };

    template <typename T>
    class Vector3
    {
    public:
        T x, y, z;
        Vector3() : x(0), y(0), z(0) {}
        Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
        Vector3(const Vector2<T> &v, T z) : x(v.x), y(v.y), z(z) {}

        Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
        Vector3 operator+(const T& f) const { return Vector3(x + f, y + f, z + f); }
        Vector3 operator-() const { return Vector3(-x, -y, -z); }
        Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
        Vector3 operator-(const T& f) const { return Vector3(x - f, y - f, z - f); }

        Vector3 operator+= (const Vector3 & v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }
        Vector3 operator-= (const Vector3 & v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
		}

        T dot(const Vector3 &v) const { return x * v.x + y * v.y + z * v.z; }
        Vector3 cross(const Vector3 &v) const { return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
        T magnitude() const { return sqrt(x * x + y * y + z * z); }

        Vector3 normalize() const
        {
            T inv_m = 1 / magnitude();
            return Vector3(x * inv_m, y * inv_m, z * inv_m);
        }

        friend std::ostream &operator<<(std::ostream &os, const Vector3 &v)
        {
            os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
            return os;
        }
    };

    template <typename T>
    class Vector4
    {
    public:
        T x, y, z, t;

        Vector4() : x(0), y(0), z(0), t(0) {}
        Vector4(T x, T y, T z, T t) : x(x), y(y), z(z), t(t) {}
        Vector4(const Vector3<T> &v, T t) : x(v.x), y(v.y), z(v.z), t(t) {}

        Vector4 operator+(const Vector4 &v) const { return Vector4(x + v.x, y + v.y, z + v.z, t + v.t); }
        Vector4 operator+(const T &f) const { return Vector4(x + f, y + f, z + f, t + f); }
		Vector4 operator-() const { return Vector4(-x, -y, -z, -t); }
        Vector4 operator-(const Vector4 &v) const { return Vector4(x - v.x, y - v.y, z - v.z, t - v.t); }
        Vector4 operator-(const T &f) const { return Vector4(x - f, y - f, z - f, t - f); }

        Vector4 operator+=(const Vector4& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            t += v.t;
            return *this;
        }
        Vector4 operator-=(const Vector4& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            t -= v.t;
            return *this;
		}

        T dot(const Vector4 &v) const { return x * v.x + y * v.y + z * v.z + t * v.t; }
        T magnitude() const { return sqrt(x * x + y * y + z * z + t * t); }

        Vector4 normalize() const
        {
            T inv_m = 1 / magnitude();
            return Vector4(x * inv_m, y * inv_m, z * inv_m, t * inv_m);
        }

        friend std::ostream &operator<<(std::ostream &os, const Vector4 &v)
        {
            os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.t << ")";
            return os;
        }
    };

    template <typename T>
    class Matrix2
    {
    public:
        T m[2][2] = {{1, 0}, {0, 1}};

        Matrix2() {}

        Matrix2 operator-() const
        {
            Matrix2 result;
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    result.m[i][j] = -m[i][j];
            return result;
        }

        Matrix2 operator+(const Matrix2& mat) const
        {
            Matrix2 result;
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    result.m[i][j] = m[i][j] + mat.m[i][j];
            return result;
        }

        Matrix2 operator-(const Matrix2& mat) const
        {
            Matrix2 result;
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    result.m[i][j] = m[i][j] - mat.m[i][j];
            return result;
		}

        Matrix2 operator*(const Matrix2 &mat) const
        {
            Matrix2 result;
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    result.m[i][j] = 0;
                    for (int k = 0; k < 2; k++)
                    {
                        result.m[i][j] += m[i][k] * mat.m[k][j];
                    }
                }
            }
            return result;
        }

        Matrix2& operator*=(const Matrix2 &mat)
        {
            *this = (*this) * mat;
            return *this;
        }
        
        Matrix2& scale(const Vector2<T> &v)
        {
            Matrix2 scaleMatrix;
            scaleMatrix.m[0][0] = v.x;
            scaleMatrix.m[1][1] = v.y;

            *this *= scaleMatrix;
            return *this;
        }

        Matrix2& scale(T value){return scale(Vector2<T>(value, value));}
        Matrix2& scale(T x, T y){return scale(Vector2<T>(x, y));}

        Matrix2& rotate(T angle)
        {
            Matrix2 rotationMatrix;
            rotationMatrix.m[0][0] = cos(angle);
            rotationMatrix.m[0][1] = -sin(angle);
            rotationMatrix.m[1][0] = sin(angle);
            rotationMatrix.m[1][1] = cos(angle);

            *this *= rotationMatrix;
            return *this;
        }

        Vector2<T> operator*(const Vector2<T> &v) const
        {
            return Vector2(m[0][0] * v.x + m[0][1] * v.y, m[1][0] * v.x + m[1][1] * v.y);
        }

        friend std::ostream &operator<<(std::ostream &os, const Matrix2 &mat)
        {
            os << "[" << mat.m[0][0] << ", " << mat.m[0][1] << "]" << std::endl;
            os << "[" << mat.m[1][0] << ", " << mat.m[1][1] << "]" << std::endl;
            return os;
        }
    };

    template <typename T>
    class Matrix3
    {
    public:
        T m[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

        Matrix3() {}

        Matrix3 operator-() const
        {
            Matrix3 result;
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    result.m[i][j] = -m[i][j];
            return result;
        }

        Matrix3 operator+(const Matrix3 &mat) const
        {
            Matrix3 result;
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    result.m[i][j] = m[i][j] + mat.m[i][j];
            return result;
		}

        Matrix3 operator-(const Matrix3& mat) const
        {
            Matrix3 result;
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    result.m[i][j] = m[i][j] - mat.m[i][j];
            return result;
		}

        Matrix3 operator*(const Matrix3 &mat) const
        {
            Matrix3 result;
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    result.m[i][j] = 0;
                    for (int k = 0; k < 3; k++)
                    {
                        result.m[i][j] += m[i][k] * mat.m[k][j];
                    }
                }
            }
            return result;
        }

        Vector3<T> operator*(const Vector3<T> &v) const
        {
            return Vector3(m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
                           m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
                           m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
        }

        Matrix3& operator*=(const Matrix3 &mat)
        {
            *this = (*this) * mat;
            return *this;
        }

        Matrix3& translate(const Vector2<T> &v)
        {
            Matrix3 translationMatrix;
            translationMatrix.m[0][2] = v.x;
            translationMatrix.m[1][2] = v.y;

            *this *= translationMatrix;
            return *this;
        }

        Matrix3& scale(const Vector2<T> &v)
        {
            Matrix3 scaleMatrix;
            scaleMatrix.m[0][0] = v.x;
            scaleMatrix.m[1][1] = v.y;

            *this *= scaleMatrix;
            return *this;
        }

        Matrix3& scale(T value){return scale(Vector2<T>(value, value));}
        Matrix3& scale(T x, T y){return scale(Vector2<T>(x, y));}

        Matrix3& rotate(const T &angle)
        {
            Matrix3 rotationMatrix;
            rotationMatrix.m[0][0] = cos(angle);
            rotationMatrix.m[0][1] = -sin(angle);
            rotationMatrix.m[1][0] = sin(angle);
            rotationMatrix.m[1][1] = cos(angle);

            *this *= rotationMatrix;
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &os, const Matrix3 &mat)
        {
            os << "[" << mat.m[0][0] << ", " << mat.m[0][1] << ", " << mat.m[0][2] << "]" << std::endl;
            os << "[" << mat.m[1][0] << ", " << mat.m[1][1] << ", " << mat.m[1][2] << "]" << std::endl;
            os << "[" << mat.m[2][0] << ", " << mat.m[2][1] << ", " << mat.m[2][2] << "]" << std::endl;
            return os;
        }
    };

    template <typename T>
    class Matrix4
    {
    public:
        T m[4][4];

        Matrix4()
        {
            setIdentity();
        }

        static Matrix4 zero()
        {
            Matrix4 mat;
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    mat.m[i][j] = 0.0f;
            return mat;
        }

        static Matrix4 identity()
        {
            Matrix4 mat;
            mat.setIdentity();
            return mat;
        }

        static Matrix4 diagonal(T value)
        {
            Matrix4 mat = zero();
            for (int i = 0; i < 4; i++)
                mat.m[i][i] = value;
            return mat;
        }

        Matrix4(std::initializer_list<T> values)
        {
            auto it = values.begin();
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    m[i][j] = (it != values.end()) ? *it++ : 0.0f;
        }

        static Matrix4 ortho(T left, T right, T bottom, T top, T near, T far)
        {
            Matrix4 mat = zero();
            mat.m[0][0] = 2.0f / (right - left);
            mat.m[1][1] = 2.0f / (top - bottom);
            mat.m[2][2] = -2.0f / (far - near);
            mat.m[3][3] = 1.0f;

            mat.m[0][3] = -(right + left) / (right - left);
            mat.m[1][3] = -(top + bottom) / (top - bottom);
            mat.m[2][3] = -(far + near) / (far - near);

            return mat;
        }

        static Matrix4 perspective(T fov, T aspect, T near, T far)
        {
            Matrix4 mat = zero();
            T tanHalfFov = tan(fov / 2.0f);
            mat.m[0][0] = 1.0f / (aspect * tanHalfFov);
            mat.m[1][1] = 1.0f / tanHalfFov;
            mat.m[2][2] = -(far + near) / (far - near);
            mat.m[2][3] = -1.0f;
            mat.m[3][2] = -(2.0f * far * near) / (far - near);
            return mat;
        }

        void setIdentity()
        {
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    m[i][j] = (i == j) ? 1.0f : 0.0f;
        }

        Matrix4 operator-() const
        {
            Matrix4 result;
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    result.m[i][j] = -m[i][j];
            return result;
		}

        Matrix4 operator+(const Matrix4 &mat) const
        {
            Matrix4 result;
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    result.m[i][j] = m[i][j] + mat.m[i][j];
            return result;
		}

        Matrix4 operator-(const Matrix4& mat) const
        {
            Matrix4 result;
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    result.m[i][j] = m[i][j] - mat.m[i][j];
            return result;
        }


        Matrix4 operator*(const Matrix4 &mat) const
        {
            Matrix4 result;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    result.m[i][j] = 0;
                    for (int k = 0; k < 4; k++)
                    {
                        result.m[i][j] += m[i][k] * mat.m[k][j];
                    }
                }
            }
            return result;
        }

        Matrix4& operator*=(const Matrix4 &mat)
        {
            *this = (*this) * mat;
            return *this;
        }

        Vector4<T> operator*(const Vector4<T> &v) const
        {
            return Vector4(m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.t,
                           m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.t,
                           m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.t,
                           m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.t);
        }

        Matrix4& translate(const Vector3<T> &v)
        {
            Matrix4 translationMatrix;
            translationMatrix.m[0][3] = v.x;
            translationMatrix.m[1][3] = v.y;
            translationMatrix.m[2][3] = v.z;

            *this *= translationMatrix;
            return *this;
        }

        Matrix4& translate(const Vector2<T>& v) { return translate(Vector3<T>(v, 0));}
        Matrix4& translate(T x, T y, T z){return translate(Vector3<T>(x, y, z));}

        Matrix4& scale(const Vector3<T> &v)
        {
            Matrix4 scaleMatrix;
            scaleMatrix.m[0][0] = v.x;
            scaleMatrix.m[1][1] = v.y;
            scaleMatrix.m[2][2] = v.z;

            *this *= scaleMatrix;
            return *this;
        }

        Matrix4& scale(const Vector2<T>& v) { return scale(Vector3<T>(v, 0)); }
        Matrix4& scale(T x, T y, T z){return scale(Vector3<T>(x, y, z));}
        Matrix4& scale(T value){return scale(Vector3<T>(value, value, value));}
        

        Matrix4& rotateX(const T &angle)
        {
            Matrix4 rotationMatrix;
            rotationMatrix.m[1][1] = cos(angle);
            rotationMatrix.m[1][2] = -sin(angle);
            rotationMatrix.m[2][1] = sin(angle);
            rotationMatrix.m[2][2] = cos(angle);

            *this *= rotationMatrix;
            return *this;
        }

        Matrix4& rotateY(const T &angle)
        {
            Matrix4 rotationMatrix;
            rotationMatrix.m[0][0] = cos(angle);
            rotationMatrix.m[0][2] = sin(angle);
            rotationMatrix.m[2][0] = -sin(angle);
            rotationMatrix.m[2][2] = cos(angle);

            *this *= rotationMatrix;
            return *this;
        }

        Matrix4& rotateZ(const T &angle)
        {
            Matrix4 rotationMatrix;
            rotationMatrix.m[0][0] = cos(angle);
            rotationMatrix.m[0][1] = -sin(angle);
            rotationMatrix.m[1][0] = sin(angle);
            rotationMatrix.m[1][1] = cos(angle);

            *this *= rotationMatrix;
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &os, const Matrix4 &mat)
        {
            os << "[" << mat.m[0][0] << ", " << mat.m[0][1] << ", " << mat.m[0][2] << ", " << mat.m[0][3] << "]" << std::endl;
            os << "[" << mat.m[1][0] << ", " << mat.m[1][1] << ", " << mat.m[1][2] << ", " << mat.m[1][3] << "]" << std::endl;
            os << "[" << mat.m[2][0] << ", " << mat.m[2][1] << ", " << mat.m[2][2] << ", " << mat.m[2][3] << "]" << std::endl;
            os << "[" << mat.m[3][0] << ", " << mat.m[3][1] << ", " << mat.m[3][2] << ", " << mat.m[3][3] << "]" << std::endl;
            return os;
        }
    };
}
