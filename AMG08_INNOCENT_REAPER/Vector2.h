/*****************************************************************//**
 * @file   Vector2.h
 * @brief  2次元ベクトルクラス
 * 
 * @author アミューズメントメディア総合学院
 * @date   October 2021
 *********************************************************************/
#pragma once
#include <cmath>
/** 2次元ベクトル */
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
  // ベクトルの加算（x, y）
  friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs) {
    return Vector2{ lhs.x + rhs.x, lhs.y + rhs.y };
  }
  // ベクトルの減算（x, y）
  friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs) {
    return { lhs.x - rhs.x, lhs.y - rhs.y };
  }
  // ベクトルのスカラー倍（x, y）
  friend Vector2 operator*(const Vector2& lhs, double scalar) {
    return { lhs.x * scalar, lhs.y * scalar };
  }
private:
  double x, y; //!< 成分
  inline double Length() { return std::sqrt(x * x + y * y); }
};