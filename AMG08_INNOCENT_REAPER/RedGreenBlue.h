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
		RedGreenBlue(double red, double green, double blue);
		~RedGreenBlue() = default;

		inline int Red() { return  static_cast<int>(_red); }
		inline int Green() { return static_cast<int>(_green); }
		inline int Blue() { return static_cast<int>(_blue); }

		void Update(bool type, double value);
		void Update(bool type, double rvalue, double gvalue, double bvalue);

		void SetBlend(double* value);
	private:
		double _red;
		double _green;
		double _blue;
	};
}

