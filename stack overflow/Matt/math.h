#pragma once

#define M_RADPI 57.295779513082f
#define M_PI 3.14159265358979323846
#define M_PI_F		((float)(M_PI))

#define SQUARE( a ) a*a
#define DEG2RAD( x  )  ( (float)(x) * (float)( M_PI_F / 180.f ) )
#define RAD2DEG( x  )  ( (float)(x) * (float)( 180.f/M_PI_F ) )

#define RAND_INT(hi, lo) (hi - lo + 1) + lo

double inline __declspec ( naked ) __fastcall FastSQRT( double n )
{
	_asm fld qword ptr[ esp + 4 ]
	_asm fsqrt
	_asm ret 8
}

float inline RAND_FLOAT(float hi, float lo) 
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = hi - lo;
	float r = random * diff;
	return lo + r;
}

class Vector
{
public:
	float x, y, z;

	Vector()
	{
		x = y = z = 0.0f;
	}

	Vector( float X, float Y, float Z )
	{
		x = X; y = Y; z = Z;
	}

	Vector( float XYZ )
	{
		x = XYZ; y = XYZ; z = XYZ;
	}

	Vector( float* v )
	{
		x = v[ 0 ]; y = v[ 1 ]; z = v[ 2 ];
	}

	Vector( const float* v )
	{
		x = v[ 0 ]; y = v[ 1 ]; z = v[ 2 ];
	}
	
	inline bool operator==(const Vector& v)
	{
		return (x == v.x && y == v.y && z == v.z);
	}
	
	inline Vector& operator=( const Vector& v )
	{
		x = v.x; y = v.y; z = v.z; return *this;
	}

	inline Vector& operator=( const float* v )
	{
		x = v[ 0 ]; y = v[ 1 ]; z = v[ 2 ]; return *this;
	}

	inline float& operator[]( int i )
	{
		return ( ( float* )this )[ i ];
	}

	inline float operator[]( int i ) const
	{
		return ( ( float* )this )[ i ];
	}

	inline Vector& operator+=( const Vector& v )
	{
		x += v.x; y += v.y; z += v.z; return *this;
	}

	inline Vector& operator-=( const Vector& v )
	{
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}

	inline Vector& operator*=( const Vector& v )
	{
		x *= v.x; y *= v.y; z *= v.z; return *this;
	}

	inline Vector& operator/=( const Vector& v )
	{
		x /= v.x; y /= v.y; z /= v.z; return *this;
	}

	inline Vector& operator+=( float v )
	{
		x += v; y += v; z += v; return *this;
	}

	inline Vector& operator-=( float v )
	{
		x -= v; y -= v; z -= v; return *this;
	}

	inline Vector& operator*=( float v )
	{
		x *= v; y *= v; z *= v; return *this;
	}

	inline Vector& operator/=( float v )
	{
		x /= v; y /= v; z /= v; return *this;
	}

	inline Vector operator-( ) const
	{
		return Vector( -x, -y, -z );
	}

	inline Vector operator+( const Vector& v ) const
	{
		return Vector( x + v.x, y + v.y, z + v.z );
	}

	inline Vector operator-( const Vector& v ) const
	{
		return Vector( x - v.x, y - v.y, z - v.z );
	}

	inline Vector operator*( const Vector& v ) const
	{
		return Vector( x * v.x, y * v.y, z * v.z );
	}

	inline Vector operator/( const Vector& v ) const
	{
		return Vector( x / v.x, y / v.y, z / v.z );
	}

	inline Vector operator+( float v ) const
	{
		return Vector( x + v, y + v, z + v );
	}

	inline Vector operator-( float v ) const
	{
		return Vector( x - v, y - v, z - v );
	}

	inline Vector operator*( float v ) const
	{
		return Vector( x * v, y * v, z * v );
	}

	inline Vector operator/( float v ) const
	{
		return Vector( x / v, y / v, z / v );
	}

	inline float Length() const
	{
		return sqrtf( x * x + y * y + z * z );
	}

	inline float LengthSqr() const
	{
		return ( x * x + y * y + z * z );
	}

	inline float LengthXY() const
	{
		return sqrtf( x * x + y * y );
	}

	inline float LengthXZ() const
	{
		return sqrtf( x * x + z * z );
	}

	inline float DistTo( const Vector& v ) const
	{
		return ( *this - v ).Length();
	}

	inline float Dot( const Vector& v ) const
	{
		return ( x * v.x + y * v.y + z * v.z );
	}

	inline Vector Cross( const Vector& v ) const
	{
		return Vector( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
	}

	inline bool IsZero() const
	{
		return ( x > -0.01f && x < 0.01f
				 &&	y > -0.01f && y < 0.01f
				 &&	z > -0.01f && z < 0.01f );
	}
	
	inline bool IsTrue() const
	{
		return ( x < 90.0f && x > -90.0f &&	y < 180.0f && y > -180.0f && z == 0.0f && std::isfinite( x ) && std::isfinite( y ) &&	std::isfinite( z ) );
	}
	
	inline void Normalize()
	{
		if (!std::isfinite(x)) x = 0;
		if (!std::isfinite(y)) y = 0;
		while (y < -180.0f) y += 360.0f;
		while (y > 180.0f) y -= 360.0f;
		if (x > 89.0f) x = 89.0f;
		if (x < -89.0f) x = -89.0f;
		z = 0;
	}
	
	inline float DotProduct(Vector v) const
	{
		return (x * v.x + y * v.y + z * v.z);
	}
	
	inline void PrintInfo() const
	{
		printf_s("%f | %f | %f", x, y, z);
	}
};

class Matrix4x4
{
public:
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};

		float m[ 4 ][ 4 ];
		float mm[ 16 ];

		struct
		{
			__m128 m1, m2, m3, m4;
		};
	};

	inline void Transpose()
	{
		for( int i = 0; i < 4; i++ )
		{
			for( int j = 0; j < 4; j++ )
			{
				m[ i ][ j ] = m[ j ][ i ];
			}
		}
	}

	inline Vector& GetAxis( int i )
	{
		return *( Vector* )&m[ i ][ 0 ];
	}
};

class Matrix3x4
{
public:
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
		};

		float m[ 3 ][ 4 ];
		float mm[ 12 ];
	};

	inline Vector& GetAxis( int i )
	{
		return *( Vector* )&m[ i ][ 0 ];
	}
};

class Math
{
public:

	Math()
	{
		srand( time( NULL ) );
	}

	void inline SinCos( float radians, float *sine, float *cosine )
	{
		_asm
		{
			fld		DWORD PTR[ radians ]
				fsincos

				mov edx, DWORD PTR[ cosine ]
				mov eax, DWORD PTR[ sine ]

				fstp DWORD PTR[ edx ]
				fstp DWORD PTR[ eax ]
		}

		*sine = sin( radians );
		*cosine = cos( radians );
	}
[junk_enable /]	

	void AngleVectors( const Vector &angles, Vector *forward )
	{

		float sr, sp, sy, cr, cp, cy;

		SinCos( DEG2RAD( angles[ 0 ] ), &sy, &cy );
		SinCos( DEG2RAD( angles[ 1 ] ), &sp, &cp );
		SinCos( DEG2RAD( angles[ 2 ] ), &sr, &cr );

		if( forward )
		{
			forward->x = cp*cy;
			forward->y = cp*sy;
			forward->z = -sp;
		}
	}

	void AngleVectors( const Vector &angles, Vector *forward, Vector *right, Vector *up )
	{

		float sr, sp, sy, cr, cp, cy;

		SinCos( DEG2RAD( angles[ 0 ] ), &sy, &cy );
		SinCos( DEG2RAD( angles[ 1 ] ), &sp, &cp );
		SinCos( DEG2RAD( angles[ 2 ] ), &sr, &cr );

		if( forward )
		{
			forward->x = cp*cy;
			forward->y = cp*sy;
			forward->z = -sp;
		}

		if( right )
		{
			right->x = ( -1 * sr*sp*cy + -1 * cr*-sy );
			right->y = ( -1 * sr*sp*sy + -1 * cr*cy );
			right->z = -1 * sr*cp;
		}

		if( up )
		{
			up->x = ( cr*sp*cy + -sr*-sy );
			up->y = ( cr*sp*sy + -sr*cy );
			up->z = cr*cp;
		}
	}

	float DotProduct( Vector &v1, float* v2 )
	{
		return v1.x*v2[ 0 ] + v1.y*v2[ 1 ] + v1.z*v2[ 2 ];
	}

	float Dot( const Vector &v1, Vector &v2 )
	{
		return v1[ 0 ] * v2[ 0 ] + v1[ 1 ] * v2[ 1 ] + v1[ 2 ] * v2[ 2 ];
	}

	void VectorTransform( Vector &in1, Matrix3x4& in2, Vector &out )
	{
		out.x = DotProduct( in1, in2.m[ 0 ] ) + in2.m[ 0 ][ 3 ];
		out.y = DotProduct( in1, in2.m[ 1 ] ) + in2.m[ 1 ][ 3 ];
		out.z = DotProduct( in1, in2.m[ 2 ] ) + in2.m[ 2 ][ 3 ];
	}

	float VecLength( Vector& vec )
	{
		return FastSQRT( vec.x *  vec.x + vec.y *  vec.y + vec.z *  vec.z );
	}

	float VecDist( Vector& fVec1, Vector& fVec2 )
	{
		return FastSQRT( pow( fVec1.x - fVec2.x, 2 ) + pow( fVec1.y - fVec2.y, 2 ) + pow( fVec1.z - fVec2.z, 2 ) );
	}

	float GetFov( Vector angle, Vector src, Vector dst )
	{
		Vector ang, aim;
		ang = CalcAngle( src, dst );
		MakeVector( angle, aim );
		MakeVector( ang,   ang );

		float mag = FastSQRT( pow( aim.x, 2 ) + pow( aim.y, 2 ) + pow( aim.z, 2 ) );
		float u_dot_v = Dot( aim, ang );

		return RAD2DEG( acos( u_dot_v / ( pow( mag, 2 ) ) ) );
	}

	Vector CalcAngle( Vector PlayerPos, Vector EnemyPos )
	{
		Vector AimAngles;
		Vector delta = PlayerPos - EnemyPos;
		float hyp = FastSQRT( ( delta.x * delta.x ) + ( delta.y * delta.y ) ); //SUPER SECRET IMPROVEMENT CODE NAME DONUT STEEL
		AimAngles.x = atanf( delta.z / hyp ) * M_RADPI;
		AimAngles.y = atanf( delta.y / delta.x ) * M_RADPI;
		AimAngles.z = 0.0f;
		if( delta.x >= 0.0 )
			AimAngles.y += 180.0f;

		return AimAngles;
	}

	void VectorAngles( const Vector& dir, Vector &angles )
	{
		float hyp = FastSQRT( ( dir.x * dir.x ) + ( dir.y * dir.y ) ); //SUPER SECRET IMPROVEMENT CODE NAME DONUT STEEL
		angles.x = atanf( dir.z / hyp ) * M_RADPI;
		angles.y = atanf( dir.y / dir.x ) * M_RADPI;
		angles.z = 0.0f;
		if( dir.x >= 0.0 )
			angles.y += 180.0f;
	}

	void ClampAngle( Vector& angles )
	{
		if( angles.x < -89.0f )
			angles.x = 89.0f;
		if( angles.x >  89.0f )
			angles.x = 89.0f;
		if( angles.y < -180.0f )
			angles.y += 360.0f;
		if( angles.y >  180.0f )
			angles.y -= 360.0f;
		if( angles.z != 0.0f ) 
			angles.z = 0.0f;
	}

	void VectorNormalize( Vector& v )
	{
		float l = VecLength( v );
		if( l != 0.0f )
		{
			v /= l;
		}
		else
		{
			v.x = v.y = 0.0f; v.z = 1.0f;
		}
	}

	void SmoothAngle( Vector& ViewAngle, Vector& DestAngles, float smooth )
	{
		Vector vecDelta = ViewAngle - DestAngles;
		ClampAngle( vecDelta );
		DestAngles = ViewAngle - vecDelta / 100.0f * smooth; // 50.0f is ur smooth value
	}

	void MakeVector( Vector angle, Vector& vector )
	{
		float pitch = float( angle[ 0 ] * M_PI / 180 );
		float yaw = float( angle[ 1 ] * M_PI / 180 );
		float tmp = float( cos( pitch ) );
		vector[ 0 ] = float( -tmp * -cos( yaw ) );
		vector[ 1 ] = float( sin( yaw )*tmp );
		vector[ 2 ] = float( -sin( pitch ) );
	}

	Vector AngleToDirection( Vector angle )
	{
		// Convert angle to radians 
		angle.x = ( float )DEG2RAD( angle.x );
		angle.y = ( float )DEG2RAD( angle.y );

		float sinYaw = sin( angle.y );
		float cosYaw = cos( angle.y );

		float sinPitch = sin( angle.x );
		float cosPitch = cos( angle.x );

		Vector direction;
		direction.x = cosPitch * cosYaw;
		direction.y = cosPitch * sinYaw;
		direction.z = -sinPitch;

		return direction;
	}

	void VectorITransform( Vector& in1, const Matrix3x4& in2, Vector& out )
	{
		float in1t[ 3 ];

		in1t[ 0 ] = in1.x - in2.m[ 0 ][ 3 ];
		in1t[ 1 ] = in1.y - in2.m[ 1 ][ 3 ];
		in1t[ 2 ] = in1.z - in2.m[ 2 ][ 3 ];

		out.x = in1t[ 0 ] * in2.m[ 0 ][ 0 ] + in1t[ 1 ] * in2.m[ 1 ][ 0 ] + in1t[ 2 ] * in2.m[ 2 ][ 0 ];
		out.y = in1t[ 0 ] * in2.m[ 0 ][ 1 ] + in1t[ 1 ] * in2.m[ 1 ][ 1 ] + in1t[ 2 ] * in2.m[ 2 ][ 1 ];
		out.z = in1t[ 0 ] * in2.m[ 0 ][ 2 ] + in1t[ 1 ] * in2.m[ 1 ][ 2 ] + in1t[ 2 ] * in2.m[ 2 ][ 2 ];
	}

	void VectorIRotate( Vector& in1, const Matrix3x4& in2, Vector& out )
	{
		out.x = in1.x*in2.m[ 0 ][ 0 ] + in1.y*in2.m[ 1 ][ 0 ] + in1.z*in2.m[ 2 ][ 0 ];
		out.y = in1.x*in2.m[ 0 ][ 1 ] + in1.y*in2.m[ 1 ][ 1 ] + in1.z*in2.m[ 2 ][ 1 ];
		out.z = in1.x*in2.m[ 0 ][ 2 ] + in1.y*in2.m[ 1 ][ 2 ] + in1.z*in2.m[ 2 ][ 2 ];
	}
[junk_disable /]
};

extern Math* g_pMath;
