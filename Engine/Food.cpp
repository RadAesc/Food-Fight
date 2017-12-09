#include "Food.h"
#include "Random.h"
#include "Graphics.h"
#include <cassert>
#include "Table.h"

const Surface HotDog::spr = Surface( "Images/HotDog.bmp" );

Food::Food()
	:
	size( 50.0f,50.0f ),
	pos( 0.0f,0.0f ),
	hitbox( pos,pos + size )
{
}

Food::Food( Random& rng )
	:
	Food()
{
	pos.x = float( rng.NextInt( int( size.x ),Graphics::ScreenWidth - int( size.x ) ) );
	pos.y = float( rng.NextInt( int( size.y ),Graphics::ScreenHeight - int( size.y ) ) );
}

Food::Food( const Vec2& pos )
	:
	Food()
{
	this->pos = pos;
}

Food::Food( const Vec2& pos,const Vec2& size )
	:
	size( size ),
	pos( pos ),
	hitbox( pos,pos + size )
{
}

void Food::Update( float dt )
{
	hitbox.MoveTo( pos );
}

void Food::Draw( Graphics& gfx ) const
{
	gfx.DrawRect( int( pos.x ),int( pos.y ),int( size.x ),int( size.y ),Colors::Magenta );
}

const Vec2& Food::GetPos() const
{
	return pos;
}

const Rect& Food::GetRect() const
{
	return hitbox;
}

HotDog::HotDog()
	:
	Food( { 0.0f,0.0f },{ 30.0f,70.0f } )
{
	target = { 0.0f,0.0f };
	hitbox = spr.GetRect();
	hitbox.MoveTo( pos );
}

HotDog::HotDog( const HotDog& other )
{
	*this = other;
}

HotDog& HotDog::operator=( const HotDog& other )
{
	target = other.target;
	hitbox = other.hitbox;

	return *this;
}

void HotDog::Update( float dt,Random& rng )
{
	const int rngNum = rng.NextInt( 0,10 );
	if( rngNum > 6 )
	{
		state = MoveState::Waiting;
	}
	else if( rngNum > 4 )
	{
		state = MoveState::Moving;
	}
	else
	{
		// State is still being hurt.
	}

	if( state == MoveState::Moving )
	{
		const Vec2 diff = target - pos;
		pos += diff.GetNormalized() * speed * dt;
	}

	Food::Update( dt );
}

void HotDog::Draw( Graphics& gfx ) const
{
	assert( hitbox.IsContainedBy( Graphics::GetScreenRect() ) );
	if( state != MoveState::Hurt )
	{
		gfx.DrawSprite( int( pos.x ),int( pos.y ),spr );
	}
	else
	{
		gfx.DrawSprite( int( pos.x ),int( pos.y ),spr,Colors::White,Colors::Magenta );
	}

	gfx.DrawHitbox( hitbox,{ 255,160,0 },true );
	// gfx.DrawLine( int( pos.x ),int( pos.y ),int( target.x ),int( target.y ),Colors::Cyan );
}

void HotDog::Hurt( int amount )
{
	hp -= amount;
	state = MoveState::Hurt;
}

void HotDog::Target( const Vec2& targetPos )
{
	target = targetPos;
}

HotDog::operator bool() const
{
	return hp > 0;
}

const Vec2& HotDog::GetTarget() const
{
	return target;
}
