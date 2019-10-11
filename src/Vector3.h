//
// Created by skalimat on 11.10.19.
//

#ifndef AVT7_VECTOR3_H
#define AVT7_VECTOR3_H

class Vector3 {
public:
    Vector3(float _x, float _y, float _z) {
        this->set(_x, _y, _z);
    }

    float getX() const {
        return this->x;
    }

    float getY() const {
        return this->y;
    }

    float getZ() const {
        return this->z;
    }

    void set(float _x, float _y, float _z) {
        this->x = _x;
        this->y = _y;
        this->z = _z;
    }

    inline bool operator==(const Vector3& vec) const {
        return ((vec.x == this->x) && (vec.y == this->y) && (vec.z == this->z));
    }

    inline Vector3 operator+(const Vector3& vec) const {
        return {this->x + vec.getX(), this->y + vec.getY(), this->z + vec.getZ()};
    }

    inline Vector3 operator-(const Vector3& vec) const {
        return {this->x - vec.getX(), this->y - vec.getY(), this->z - vec.getZ()};
    }

    inline Vector3 operator*(float scalar) const {
        return {this->x * scalar, this->y * scalar, this->z * scalar};
    }

    inline void operator=(const Vector3& vec) {
        x = vec.getX();
        y = vec.getY();
        z = vec.getZ();
    }

private:
    float x;
    float y;
    float z;
};

#endif //AVT7_VECTOR3_H
