#pragma once

#include <Box2D\Box2D.h>
#include "IndexedTriangleList.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Mat2.h"
#include "Colors.h"
#include "Pipeline.h"
#include "SolidEffect.h"
#include "BodyPtr.h"

class Boundaries
{
public:
	Boundaries( b2World& world,float size )
		:
		size( size )
	{
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_staticBody;
			bodyDef.position = { 0.0f,0.0f };
			pBody = BodyPtr::Make( world,bodyDef );
		}
		{
			const float extents = 0.99f * size;
			const b2Vec2 vertices[] = {
				{ -extents,-extents },
				{  extents,-extents },
				{  extents, extents },
				{ -extents, extents }
			};
			b2ChainShape chain;
			chain.CreateLoop( vertices,4 );
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &chain;
			fixtureDef.friction = 0.0f;
			fixtureDef.density = 1.0f;
			fixtureDef.restitution = 1.0f;
			pBody->CreateFixture( &fixtureDef );
		}
		pBody->SetUserData( this );
	}
	float GetSize() const
	{
		return size;
	}
private:
	float size;
	BodyPtr pBody;
};