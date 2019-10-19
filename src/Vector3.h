//
// Created by skalimat on 11.10.19.
//

#ifndef AVT7_VECTOR3_H
#define AVT7_VECTOR3_H

class Vector3 {
public:
    float x;
    float y;
    float z;

    Vector3(float v = 0) {
        this->x = v;
        this->y = v;
        this->z = v;
    }

    Vector3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    inline bool operator==(const Vector3& vec) const {
        return ((vec.x == this->x) && (vec.y == this->y) && (vec.z == this->z));
    }

    inline Vector3 operator+(const Vector3& vec) const {
        return {this->x + vec.x, this->y + vec.y, this->z + vec.z};
    }

    inline Vector3 operator-(const Vector3& vec) const {
        return {this->x - vec.x, this->y - vec.y, this->z - vec.z};
    }

    inline Vector3 operator*(float scalar) const {
        return {this->x * scalar, this->y * scalar, this->z * scalar};
    }

    inline Vector3 operator/(float scalar) const {
        return {this->x / scalar, this->y / scalar, this->z / scalar};
    }

    inline bool operator>(const Vector3& vec) const {
        return (this->x > vec.x) && (this->y > vec.y) && (this->z > vec.z);
    }


    inline void operator=(const Vector3& vec) {
        x = vec.x;
        y = vec.y;
        z = vec.z;
    }

    inline float distance(const Vector3& vec) const{
        Vector3 delta = Vector3(this->x - vec.x, this->y - vec.y, this->z - vec.z);
        return sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    }
};

#endif //AVT7_VECTOR3_H
