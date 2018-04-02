#pragma once
#include <Box2D\Box2D.h>
#include <functional>
#include <memory>

class BodyPtr : public std::unique_ptr<b2Body,std::function<void(b2Body*)>>
{
public:
	static BodyPtr Make( b2World& world,const b2BodyDef& bodyDef )
	{
		return { 
			world.CreateBody( &bodyDef ),
			[&world]( b2Body* pBody ) 
			{ world.DestroyBody( pBody );}
		};
	}
	BodyPtr() = default;
private:
	BodyPtr( b2Body* p,std::function<void(b2Body*)> f )
		:
		unique_ptr( p,f )
	{}
};