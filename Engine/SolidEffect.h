#pragma once

#include "Pipeline.h"
#include "DefaultGeometryShader.h"
#include "Mat2.h"
#include "Camera.h"

// solid color attribute not interpolated
class SolidEffect
{
public:
	// the vertex type that will be input into the pipeline
	using Vertex = Vec2;
	// default vs rotates and translates vertices
	// does not touch attributestemplate<class Vertex>
	class VertexShader
	{
	public:
		class Vertex
		{
		public:
			Vertex() = default;
			Vertex( const Vec3& pos )
				:
				pos( pos )
			{}
			Vertex( const Vec3& pos,const Vertex& src )
				:
				pos( pos )
			{}
			Vertex( const Vec3& pos,const Color& color )
				:
				pos( pos )
			{}
			Vertex& operator+=( const Vertex& rhs )
			{
				pos += rhs.pos;
				return *this;
			}
			Vertex operator+( const Vertex& rhs ) const
			{
				return Vertex( *this ) += rhs;
			}
			Vertex& operator-=( const Vertex& rhs )
			{
				pos -= rhs.pos;
				return *this;
			}
			Vertex operator-( const Vertex& rhs ) const
			{
				return Vertex( *this ) -= rhs;
			}
			Vertex& operator*=( float rhs )
			{
				pos *= rhs;
				return *this;
			}
			Vertex operator*( float rhs ) const
			{
				return Vertex( *this ) *= rhs;
			}
			Vertex& operator/=( float rhs )
			{
				pos /= rhs;
				return *this;
			}
			Vertex operator/( float rhs ) const
			{
				return Vertex( *this ) /= rhs;
			}
		public:
			Vec3 pos;
		};
		using Output = Vertex;
		Camera cam;
	public:
		void BindRotation( const Mat2& rotation_in )
		{
			rotation = rotation_in;
		}
		void BindTranslation( const Vec2& translation_in )
		{
			translation = translation_in;
		}
		Output operator()( const SolidEffect::Vertex& in ) const
		{
			const Vec2 xformd = (in * rotation + translation + cam.GetTranslation()) * cam.GetZoom();
			return{ {xformd.x,xformd.y,1.0f} };
		}
	private:
		Mat2 rotation;
		Vec2 translation;
	};
	// default gs passes vertices through and outputs triangle
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;
	// invoked for each pixel of a triangle
	// takes an input of attributes that are the
	// result of interpolating vertex attributes
	// and outputs a color
	class PixelShader
	{
	public:
		void BindColor( Color c )
		{
			color = c;
		}
		template<class I>
		Color operator()( const I& in ) const
		{
			return color;
		}
	private:
		Color color;
	};
public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};