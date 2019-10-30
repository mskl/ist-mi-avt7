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

    explicit Vector3(float val = 0) {
        this->x = val;
        this->y = val;
        this->z = val;
    }

    Vector3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    inline bool operator==(const Vector3& vec) const {
        return ((vec.x == this->x) && (vec.y == this->y) && (vec.z == this->z));
    }

    inline bool operator!=(const Vector3& vec) const {
        return ((vec.x != this->x) || (vec.y != this->y) || (vec.z != this->z));
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

    Vector3 operator-() const {
        return {-this->x, -this->y, -this->z};
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

    friend ostream& operator<< (ostream& os, const Vector3& vec) {
        os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return os;
    }

};

#endif //AVT7_VECTOR3_H
