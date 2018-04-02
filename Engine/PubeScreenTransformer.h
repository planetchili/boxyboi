#pragma once
#include "Vec3.h"
#include "Graphics.h"

class PubeScreenTransformer
{
public:
	PubeScreenTransformer()
		:
		xFactor( float( Graphics::ScreenWidth ) / 2.0f ),
		yFactor( float( Graphics::ScreenHeight ) / 2.0f )
	{}
	template<class Vertex>
	Vertex& Transform( Vertex& v ) const
	{
		// adjust position x,y from perspective normalized space
		// to screen dimension space after perspective divide
		v.pos.x = (v.pos.x + 1.0f) * xFactor;
		v.pos.y = (-v.pos.y + 1.0f) * yFactor;

		return v;
	}
	template<class Vertex>
	Vertex GetTransformed( const Vertex& v ) const
	{
		return Transform( Vertex( v ) );
	}
private:
	float xFactor;
	float yFactor;
};