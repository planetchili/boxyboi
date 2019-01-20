/*#pragma once
#include "Box.h"
namespace Effect
{
	class Color
	{
	public:
		Color()
		{}
		void operator()(Box** boxPtrs)
		{
			boxPtrs[0]->SetColorTrait(boxPtrs[1]->GetColorTrait().Clone());
		}
	};
	class Destroy
	{
	public:
		Destroy()
		{}
		void operator()(Box** boxPtrs)
		{
			boxPtrs[0]->MarkForDeletion();
		}
	};
	class Split
	{
		Split(std::vector<std::unique_ptr<Box>>& _vec)
			:
			vec(std::move(_vec))
		{}
		void operator()(Box** boxPtrs)
		{
			std::vector<std::unique_ptr<Box>> v;
			Vec2 parent_pos = boxPtrs[0]->GetPosition();
			Vec2 pos[4] = { parent_pos };

			pos[0] = boxPtrs[0]->GetWorldPoint(Vec2(-0.25f*boxPtrs[0]->GetSize(), -0.25f*boxPtrs[0]->GetSize()));
			pos[1] = boxPtrs[0]->GetWorldPoint(Vec2(0.25f*boxPtrs[0]->GetSize(), -0.25f*boxPtrs[0]->GetSize()));
			pos[2] = boxPtrs[0]->GetWorldPoint(Vec2(-0.25f*boxPtrs[0]->GetSize(), 0.25f*boxPtrs[0]->GetSize()));
			pos[3] = boxPtrs[0]->GetWorldPoint(Vec2(0.25f*boxPtrs[0]->GetSize(), 0.25f*boxPtrs[0]->GetSize()));
			for (int i = 0; i < 4; i++)
			{
				auto pColorTraitBoxy = boxPtrs[0]->GetColorTrait().Clone();
				v.emplace_back(std::make_unique<Box>(std::move(pColorTraitBoxy),
					boxPtrs[0]->GetWorld(), pos[i], boxPtrs[0]->GetSize()*0.5f, boxPtrs[0]->GetAngle(),
					boxPtrs[0]->GetVelocity(), boxPtrs[0]->GetAngularVelocity()));
			}

			for (size_t i = 0; i < vec.size(); i++)
			{
				if (vec[i]->GetSize() > 0.1f)
				{
				auto v = vec[i]->SpawnBoxy(boxPtrs[0]->GetWorld());
				vec.insert(vec.end(), std::make_move_iterator(v.begin()), std::make_move_iterator(v.end()));
				}
			}
			boxPtrs[0]->MarkForDeletion();
		}
		std::vector<std::unique_ptr<Box>> vec;
	};
}
*/