/*****************************************************************//**
 * @file   Vector2.h
 * @brief  2�����x�N�g���N���X
 * 
 * @author �A�~���[�Y�����g���f�B�A�����w�@
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <cmath>
/** 2�����x�N�g�� */
class Vector2 {
public:
  Vector2() : x(0), y(0){}
  Vector2(double ax, double ay) :x(ax), y(ay){}

  inline double GetX() { return x; }
  inline double GetY() { return y; }
  inline double& GetPX() { return x; }
  inline double& GetPY() { return y; }
  inline int IntX() { return static_cast<int>(x); }
  inline int IntY() { return static_cast<int>(y); }

  void Normalize();
  // �x�N�g���̉��Z�ix, y�j
  friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs) {
    return Vector2{ lhs.x + rhs.x, lhs.y + rhs.y };
  }
  // �x�N�g���̌��Z�ix, y�j
  friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs) {
    return { lhs.x - rhs.x, lhs.y - rhs.y };
  }
  // �x�N�g���̃X�J���[�{�ix, y�j
  friend Vector2 operator*(const Vector2& lhs, double scalar) {
    return { lhs.x * scalar, lhs.y * scalar };
  }
private:
  double x, y; //!< ����
  inline double Length() { return std::sqrt(x * x + y * y); }
};