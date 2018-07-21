#pragma once

class ColorESP
{
public:
	// constructors
	ColorESP()
	{
		*((int *)this) = 0;
	}
	ColorESP(int color32)
	{
		*((int *)this) = color32;
	}
	ColorESP(int _r, int _g, int _b)
	{
		SetColor(_r, _g, _b, 255);
	}
	ColorESP(int _r, int _g, int _b, int _a)
	{
		SetColor(_r, _g, _b, _a);
	}

	// set the color
	// r - red component (0-255)
	// g - green component (0-255)
	// b - blue component (0-255)
	// a - alpha component, controls transparency (0 - transparent, 255 - opaque);
	void SetColor(int _r, int _g, int _b, int _a = 255)
	{
		_color[0] = (unsigned char)_r;
		_color[1] = (unsigned char)_g;
		_color[2] = (unsigned char)_b;
		_color[3] = (unsigned char)_a;
	}

	void GetColor(int &_r, int &_g, int &_b, int &_a) const
	{
		_r = _color[0];
		_g = _color[1];
		_b = _color[2];
		_a = _color[3];
	}

	void SetRawColor(int color32)
	{
		*((int*)this) = color32;
	}

	int GetRawColor() const
	{
		return *((int*)this);
	}

	int GetD3DColor() const
	{
		return ((int)((((_color[3]) & 0xff) << 24) | (((_color[0]) & 0xff) << 16) | (((_color[1]) & 0xff) << 8) | ((_color[2]) & 0xff)));
	}

	inline int r() const	{ return _color[0]; }
	inline int g() const	{ return _color[1]; }
	inline int b() const	{ return _color[2]; }
	inline int a() const	{ return _color[3]; }

	unsigned char &operator[](int index)
	{
		return _color[index];
	}

	const unsigned char &operator[](int index) const
	{
		return _color[index];
	}

	bool operator == (const ColorESP &rhs) const
	{
		return (*((int *)this) == *((int *)&rhs));
	}

	bool operator != (const ColorESP &rhs) const
	{
		return !(operator==(rhs));
	}

	ColorESP &operator=(const ColorESP &rhs)
	{
		SetRawColor(rhs.GetRawColor());
		return *this;
	}

	float* Base()
	{
		float clr[3];

		clr[0] = _color[0] / 255.0f;
		clr[1] = _color[1] / 255.0f;
		clr[2] = _color[2] / 255.0f;

		return &clr[0];
	}

	float Hue() const
	{
		if (_color[0] == _color[1] && _color[1] == _color[2])
		{
			return 0.0f;
		}

		float r = _color[0] / 255.0f;
		float g = _color[1] / 255.0f;
		float b = _color[2] / 255.0f;

		float max = r > g ? r : g > b ? g : b,
			min = r < g ? r : g < b ? g : b;
		float delta = max - min;
		float hue = 0.0f;

		if (r == max)
		{
			hue = (g - b) / delta;
		}
		else if (g == max)
		{
			hue = 2 + (b - r) / delta;
		}
		else if (b == max)
		{
			hue = 4 + (r - g) / delta;
		}
		hue *= 60;

		if (hue < 0.0f)
		{
			hue += 360.0f;
		}
		return hue;
	}

	float Saturation() const
	{
		float r = _color[0] / 255.0f;
		float g = _color[1] / 255.0f;
		float b = _color[2] / 255.0f;

		float max = r > g ? r : g > b ? g : b,
			min = r < g ? r : g < b ? g : b;
		float l, s = 0;

		if (max != min)
		{
			l = (max + min) / 2;
			if (l <= 0.5f)
				s = (max - min) / (max + min);
			else
				s = (max - min) / (2 - max - min);
		}
		return s;
	}

	float Brightness() const
	{
		float r = _color[0] / 255.0f;
		float g = _color[1] / 255.0f;
		float b = _color[2] / 255.0f;

		float max = r > g ? r : g > b ? g : b,
			min = r < g ? r : g < b ? g : b;
		return (max + min) / 2;
	}

	ColorESP FromHSB(float hue, float saturation, float brightness)
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return ColorESP(
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255),
				(unsigned char)(p * 255)
				);
		}
		else if (h < 2)
		{
			return ColorESP(
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255)
				);
		}
		else if (h < 3)
		{
			return ColorESP(
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255)
				);
		}
		else if (h < 4)
		{
			return ColorESP(
				(unsigned char)(p * 255),
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255)
				);
		}
		else if (h < 5)
		{
			return ColorESP(
				(unsigned char)(t * 255),
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255)
				);
		}
		else
		{
			return ColorESP(
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255),
				(unsigned char)(q * 255)
				);
		}
	}

	static ColorESP Red()			{ return ColorESP(255, 0, 0); }
	static ColorESP Green()		{ return ColorESP(0, 255, 0); }
	static ColorESP LightGreen()	{ return ColorESP(128, 255, 128); }
	static ColorESP Blue()			{ return ColorESP(0, 0, 255); }
	static ColorESP LightBlue()	{ return ColorESP(100, 100, 255); }
	static ColorESP Cyan()			{ return ColorESP(0, 255, 255); }
	static ColorESP LightGrey()	{ return ColorESP(128, 128, 128); }
	static ColorESP Grey()			{ return ColorESP(84, 84, 84); }
	static ColorESP DarkGrey()		{ return ColorESP(45, 45, 45); }
	static ColorESP Black()		{ return ColorESP(0, 0, 0); }
	static ColorESP White()		{ return ColorESP(255, 255, 255); }
	static ColorESP Yellow()		{ return ColorESP(255, 255, 0); }
	static ColorESP Purple()		{ return ColorESP(220, 0, 220); }
	static ColorESP Orange()		{ return ColorESP(255, 102, 0); }
	static ColorESP Pink()         { return ColorESP(255, 20, 255); }

private:
	unsigned char _color[4];
};