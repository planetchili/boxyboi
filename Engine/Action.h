#pragma once

#include "Box.h"
#include <vector>
#include <memory>

class Action
{
public:
	virtual void Do( std::vector<std::unique_ptr<Box>>& boxes,b2World& world ) = 0;
};

class Split : public Action
{
public:
	Split( Box& target )
		:
		pTarget( &target )
	{}
	void Do( std::vector<std::unique_ptr<Box>>& boxes,b2World& world ) override
	{
		auto children = pTarget->Split( world );
		boxes.insert( boxes.end(),
			std::make_move_iterator( children.begin() ),
			std::make_move_iterator( children.end() )
		);
	}
private:
	Box* pTarget;
};

class Tag : public Action
{
public:
	Tag( Box& target,std::unique_ptr<Box::ColorTrait> pColorTrait )
		:
		pTarget( &target ),
		pColorTrait( std::move( pColorTrait ) )
	{}
	void Do( std::vector<std::unique_ptr<Box>>& boxes,b2World& world ) override
	{
		pTarget->AssumeColorTrait( std::move( pColorTrait ) );
	}
private:
	std::unique_ptr<Box::ColorTrait> pColorTrait;
	Box* pTarget;
};