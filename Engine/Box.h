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
#include "Boundaries.h"
#include <random>

class Box
{
public:
	class ColorTrait
	{
	public:
		virtual ~ColorTrait() = default;
		virtual Color GetColor() const = 0;
		virtual std::unique_ptr<ColorTrait> Clone() const = 0;
	};
public:
	static std::unique_ptr<Box> Box::Spawn( float size,const Boundaries& bounds,b2World& world,std::mt19937& rng );
	Box( std::unique_ptr<ColorTrait> pColorTrait, b2World& world,const Vec2& pos,
		float size = 1.0f,float angle = 0.0f,Vec2 linVel = {0.0f,0.0f},float angVel = 0.0f )
		:
		pColorTrait( std::move( pColorTrait ) ),
		size( size )
	{
		Init();
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position = b2Vec2( pos );
			bodyDef.linearVelocity = b2Vec2( linVel );
			bodyDef.angularVelocity = angVel;
			bodyDef.angle = angle;
			pBody = BodyPtr::Make( world,bodyDef );
		}
		{
			b2PolygonShape dynamicBox;
			dynamicBox.SetAsBox( size,size );
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicBox;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.0f;
			fixtureDef.restitution = 1.0f;
			pBody->CreateFixture( &fixtureDef );
		}
		pBody->SetUserData( this );
	}
	void Draw( Pipeline<SolidEffect>& pepe ) const
	{
		pepe.effect.vs.BindTranslation( GetPosition() );
		pepe.effect.vs.BindRotation( Mat2::Rotation( GetAngle() ) * Mat2::Scaling( GetSize() ) );
		pepe.effect.ps.BindColor( GetColorTrait().GetColor() );
		pepe.Draw( model );
	}
	void ApplyLinearImpulse( const Vec2& impulse )
	{
		pBody->ApplyLinearImpulse( (b2Vec2)impulse,(b2Vec2)GetPosition(),true );
	}
	void ApplyAngularImpulse( float impulse )
	{
		pBody->ApplyAngularImpulse( impulse,true );
	}
	float GetAngle() const
	{
		return pBody->GetAngle();
	}
	Vec2 GetPosition() const
	{
		return (Vec2)pBody->GetPosition();
	}
	float GetAngularVelocity() const
	{
		return pBody->GetAngularVelocity();
	}
	Vec2 GetVelocity() const
	{
		return (Vec2)pBody->GetLinearVelocity();
	}
	float GetSize() const
	{
		return size;
	}
	const ColorTrait& GetColorTrait() const
	{
		return *pColorTrait;
	}
	void MarkForDeath()
	{
		isDying = true;
	}
	bool IsDying() const
	{
		return isDying;
	}
	void AssumeColorTrait( std::unique_ptr<ColorTrait> pct )
	{
		pColorTrait = std::move( pct );
	}
	std::vector<std::unique_ptr<Box>> Box::Split( b2World& world );
private:
	static void Init()
	{
		if( model.indices.size() == 0 )
		{
			model.vertices = { { -1.0f,-1.0 },{ 1.0f,-1.0 },{ -1.0f,1.0 },{ 1.0f,1.0 } };
			model.indices = { 0,1,2, 1,2,3 };
		}
	}
private:
	static IndexedTriangleList<Vec2> model;
	float size;
	BodyPtr pBody;
	std::unique_ptr<ColorTrait> pColorTrait;
	bool isDying = false;
};