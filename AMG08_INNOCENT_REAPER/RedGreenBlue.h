#pragma once

namespace inr {

	namespace rgb {
		constexpr auto ADD = true;
		constexpr auto SUB = false;

		constexpr auto MIN_BLEND = 0;
		constexpr auto MAX_BLEND = 255;
	}

	class RedGreenBlue {
	public:
		RedGreenBlue();
		RedGreenBlue(double red, double green, double blue);
		~RedGreenBlue() = default;

		inline int Red() { return  static_cast<int>(_red); }
		inline int Green() { return static_cast<int>(_green); }
		inline int Blue() { return static_cast<int>(_blue); }

		void Min();
		void Max();

		void Update(bool type, double value);
		void Update(bool type, double rvalue, double gvalue, double bvalue);
		// 上限を超えているか・加減を下回っているか（フラグに応じて返す値を切り替え）
		/*bool IsMaxValue(bool type);*/
	private:
		double _red;
		double _green;
		double _blue;

		void SetBlend(double* value);
	};
}

