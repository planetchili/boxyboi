#pragma once

#include "Vec2.h"
#include "Mat2.h"
#include "Pipeline.h"
#include "SolidEffect.h"

class Camera
{
public:
	Camera()
		:
		pos( { 0.0f,0.0f } ),
		zoom( 1.0f )
	{}
	void SetZoom( float zoom )
	{
		this->zoom = zoom;
	}
	void SetPos( const Vec2& pos )
	{
		this->pos = pos;
	}
	Vec2 GetTranslation() const
	{
		return -pos;
	}
	float GetZoom() const
	{
		return zoom;
	}
private:
	Vec2 pos;
	float zoom;
};