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
#include <typeindex>

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

	class Listener : public b2ContactListener
	{
	private:
		std::unordered_map <std::pair<std::type_index, std::type_index>, std::function<void(Box*, Box*)> > umap;
	public:
		void BeginContact( b2Contact* contact ) override
		{

			umap[{typeid(YellowTrait), typeid(RedTrait)}] = [](Box* bptrA, Box* bptrB)
			{
				bptrA->AddFlag((int)Effects::BOX_SPLIT);
			};

			umap[{typeid(WhiteTrait), typeid(BlueTrait)}] = [](Box* bptrA, Box* bptrB)
			{
				bptrA->AddFlag((int)Effects::BOX_COLOR);
			};
			umap[{typeid(GreenTrait), typeid(Box::ColorTrait*)}] = [](Box* bptrA, Box* bptrB)
			{
				bptrA->AddFlag((int)Effects::BOX_COLOR);
			};

			const b2Body* bodyPtrs[] = { contact->GetFixtureA()->GetBody(),contact->GetFixtureB()->GetBody() };
			if( bodyPtrs[0]->GetType() == b2BodyType::b2_dynamicBody &&
				bodyPtrs[1]->GetType() == b2BodyType::b2_dynamicBody )
			{
				Box* boxPtrs[] = { 
					reinterpret_cast<Box*>(bodyPtrs[0]->GetUserData()),
					reinterpret_cast<Box*>(bodyPtrs[1]->GetUserData())
				};
				auto& tid0 = typeid(boxPtrs[0]->GetColorTrait());
				auto& tid1 = typeid(boxPtrs[1]->GetColorTrait());

				std::stringstream msg;
				msg << "Collision between " << tid0.name() << " and " << tid1.name() << std::endl;

				umap[{typeid(boxPtrs[0]->GetColorTrait()), typeid(boxPtrs[1]->GetColorTrait())}](boxPtrs[0], boxPtrs[1]);

				OutputDebugStringA( msg.str().c_str() );
				msg << "Flag set for boxPtrs[0] = " << boxPtrs[0]->GetFlags() << std::endl;
				OutputDebugStringA(msg.str().c_str());
			}
		}
	};
	static Listener mrLister;
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

	for (size_t i = 0; i < boxPtrs.size(); i++)
	{
		std::vector<std::unique_ptr<Box>>::iterator new_end;
		switch (static_cast<Effects>(boxPtrs[i]->GetFlags()))
		{
		case Effects::BOX_DESTROY:
			new_end = std::remove_if(boxPtrs.begin(), boxPtrs.end(), [](std::unique_ptr<Box>& boxPtr)
			{
				return boxPtr->HasFlag((int)Effects::BOX_DESTROY);
			});
			boxPtrs.erase(new_end, boxPtrs.end());
			break;
		case Effects::BOX_COLOR:
			boxPtrs[i]->SetColorTrait(std::make_unique<YellowTrait>());
			break;
		case Effects::BOX_SPLIT:
			if (boxPtrs[i]->GetSize() > 0.1f)
			{
				auto v = boxPtrs[i]->SpawnBoxy(boxPtrs[i]->GetWorld());
				boxPtrs.insert(boxPtrs.end(), std::make_move_iterator(v.begin()), std::make_move_iterator(v.end()));
			}
			break;
		default:
			break;
		}
	}
}

void Game::ComposeFrame()
{
	for( const auto& p : boxPtrs )
	{
		p->Draw( pepe );
	}
}