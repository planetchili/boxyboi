/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.cpp																			  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Box.h"
#include <algorithm>
#include <sstream>
#include <typeinfo>
#include <functional>
#include <iterator>
#include "PatternMatchingListener.h"
#include "ColorTraits.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	world( { 0.0f,-0.5f } ),
	pepe( gfx )
{
	pepe.effect.vs.cam.SetPos( { 0.0,0.0f } );
	pepe.effect.vs.cam.SetZoom( 1.0f / boundarySize );

	std::generate_n( std::back_inserter( boxPtrs ),nBoxes,[this]() {
		return Box::Spawn( boxSize,bounds,world,rng );
	} );

	static PatternMatchingListener mrLister;
	mrLister.Case<RedTrait,WhiteTrait>( []( Box& r,Box& w )
	{
		r.MarkForDeath();
	} );
	mrLister.Case<YellowTrait,BlueTrait>( [this]( Box& y,Box& b )
	{
		actionPtrs.push_back( std::make_unique<Tag>( y,b.GetColorTrait().Clone() ) );
	} );
	mrLister.Case<WhiteTrait,BlueTrait>( [this]( Box& w,Box& b )
	{
		if( w.GetSize() > b.GetSize() && w.GetSize() > 0.2f )
		{
			actionPtrs.push_back( std::make_unique<Split>( w ) );
		}
		else if( b.GetSize() > 0.2f )
		{
			actionPtrs.push_back( std::make_unique<Split>( b ) );
		}
	} );
	world.SetContactListener( &mrLister );
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = ft.Mark();
	world.Step( dt,8,3 );
	// process generated actions
	for( auto& pa : actionPtrs )
	{
		pa->Do( boxPtrs,world );
	}
	actionPtrs.clear();
	// remove dying boxes
	boxPtrs.erase(
		std::remove_if( boxPtrs.begin(),boxPtrs.end(),std::mem_fn( &Box::IsDying ) ),
		boxPtrs.end()
	);
}

void Game::ComposeFrame()
{
	for( const auto& p : boxPtrs )
	{
		p->Draw( pepe );
	}
}