#include <iostream>

namespace sim
{

template <class T>
class Point2D
{

	public:
		T x, y;

		Point2D() { this->x = 0; this->y = 0; }
		Point2D(const T x, const T y) { this->x = x; this->y = y; }

		void set(const T x, const T y) { this->x = x; this->y = y; }

		Point2D operator +(const Point2D &v) const
		{
			return Point2D(x + v.x, y + v.y);
		}

		friend std::ostream & operator<< (std::ostream &stream, Point2D &v)
		{
			stream << "[" << v.x << ", " << v.y << "]";
			return stream;
		}
	
};

typedef sim::Point2D <int> Point2Di;

} /* namespace sim */
