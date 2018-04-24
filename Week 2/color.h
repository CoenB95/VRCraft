#ifndef COLOR_H
#define COLOR_H

class Color4f
{
public:
	union
	{
		struct
		{
			float r, g, b, a;
		};
		float color4fv[4];
	};
	Color4f(float r, float g, float b);
	Color4f(float r, float g, float b, float a);

	static const Color4f BLUE;
	static const Color4f CYAN;
	static const Color4f GREEN;
	static const Color4f MAGENTA;
	static const Color4f RED;
	static const Color4f YELLOW;
};

#endif // !COLOR_H
