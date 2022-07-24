#include "OtherObjects.h"
#include <cmath>
#include <algorithm>

using namespace std;

namespace ppvs
{

// Get number of digits
int getDigits(int number)
{
	int digits = 0;
	int i = 1;
	while (i <= number)
	{
		digits++;
		i *= 10;
	}
	return digits ? digits : 1;
}


Particle::Particle(float x, float y, int color, GameData* globalp)
{
	m_timer = 0;
	m_scale = 1;

	// Set sprite
	m_sprite.setImage(globalp->imgPuyo);
	m_sprite.setSubRect(5 * PUYOX + color * PUYOX, 11 * PUYOY, PUYOX, PUYOY);
	m_sprite.setCenter(11, 10);

	// Set position
	// Don't forget to that the position must be absolute
	m_posX = x;
	m_posY = y;

	// Set random initial speed
	m_speedX = (getRandom(601) - 300) / 100.0f;
	gravityTimer = -6 + getRandom(333) / 100.0f;
}

Particle::~Particle() = default;

void Particle::draw(FeRenderTarget* ri)
{
	m_sprite.setPosition(m_posX, m_posY);
	m_sprite.setScale(m_scale, m_scale);
	m_sprite.draw(ri);
}

// Particle's main function
void Particle::play()
{
	m_timer++;
	m_scale = 1.2f - m_timer * 0.05f;
	if (m_scale < 0.1f)
	{
		m_sprite.setVisible(false);
	}
	gravityTimer += 0.3f;
	m_posY += gravityTimer;
	m_posX += m_speedX;
}

bool Particle::destroy()
{
	return m_timer > 60;
}

//===========================================================================

	ParticleThrow::ParticleThrow(float x, float y, int color, GameData* globalp)
{
	m_timer = 0;
	m_scale = 1; m_rotate = 0;
	// Set sprite
	m_sprite.setImage(globalp->imgPuyo);
	const int xRect = 2 * PUYOX * (color / 2);
	const int yRect = 12 * PUYOY + PUYOY * (color % 2);
	m_sprite.setSubRect(xRect, yRect, PUYOX, PUYOY);
	m_sprite.setCenter(PUYOX / 2, PUYOY / 2);

	// Set position
	// Don't forget to that the position must be absolute
	m_posX = x;
	m_posY = y;

	// Set random initial speed
	m_speedX = static_cast<float>(getRandom(601) - 300) / 100.0f;
	gravityTimer = -6.f + static_cast<float>(getRandom(333)) / 100.0f;
	m_rotateSpeed = static_cast<float>((getRandom(21) - 10) * 2);
}

ParticleThrow::~ParticleThrow() = default;

void ParticleThrow::draw(FeRenderTarget* ri)
{
	m_sprite.setPosition(m_posX, m_posY);
	m_sprite.setScale(m_scale, m_scale);
	m_sprite.setRotation(m_rotate);
	m_sprite.draw(ri);
}

// Particle's main function
void ParticleThrow::play()
{
	m_timer++;

	// Scale
	m_scale = 1.2f + m_timer * 0.03f;
	if (m_scale < 0.1f)
		m_sprite.setVisible(false);

	// Rotate
	m_rotate += m_rotateSpeed;
	gravityTimer += 0.5f;
	m_posY += gravityTimer;
	m_posX += m_speedX;
}

bool ParticleThrow::destroy()
{
	return m_timer > 60;
}


//===========================================================================

ChainWord::ChainWord(GameData* gamedata)
{
	m_number = 0;
	m_posX = 0;
	m_posY = 0;
	m_scale = 1;
	m_visible = false;
	m_timer = 100;

	// Set sprite
	m_spriteChain.setImage(gamedata->imgChain);
	m_spriteN1.setImage(gamedata->imgChain);
	m_spriteN2.setImage(gamedata->imgChain);
	m_spriteChain.setSubRect(200, 0, 90, 42);
	m_spriteChain.setCenter(-6, 0);
}

ChainWord::~ChainWord() = default;

void ChainWord::draw(FeRenderTarget* rw)
{
	if (m_visible == false)
		return;

	m_spriteN1.setSubRect(20 * (m_number % 10), 0, 20, 42);
	m_spriteN1.setCenter(0, 0);
	m_spriteN2.setSubRect(20 * ((m_number / 10) % 10), 0, 20, 42);
	m_spriteN2.setCenter(0, 0);

	m_spriteChain.setPosition(m_posX, m_posY);
	m_spriteN1.setPosition(m_spriteChain.getX() - 20, m_spriteChain.getY());
	m_spriteN2.setPosition(m_spriteN1.getX() - 20, m_spriteChain.getY());

	m_spriteChain.setScale(1);
	m_spriteN1.setScale(1);
	m_spriteN2.setScale(1);

	m_spriteChain.draw(rw);
	m_spriteN1.draw(rw);
	if (m_number / 10 > 0)
	{
		m_spriteN2.draw(rw);
	}
}

void ChainWord::showAt(float x, float y, int n)
{
	m_number = n;
	m_posX = x;
	m_posY = y;
	m_visible = true;
	m_timer = 0;
}

void ChainWord::move()
{
	if (m_timer > 50)
	{
		m_visible = false;
		return;
	}

	m_timer++;
	if (m_timer < 15)
	{
		m_posY -= 3.0f * m_scale;
	}
}

//===========================================================================

SecondsObject::SecondsObject(GameData* gamedata)
{
	m_number = 0;
	m_posX = 0;
	m_posY = 0;
	m_scale = 1;
	m_visible = false;
	m_timer = 100;

	// Set sprite
	m_spritePlus.setImage(gamedata->imgTime);
	m_spriteN1.setImage(gamedata->imgTime);
	m_spriteN2.setImage(gamedata->imgTime);
	m_spritePlus.setSubRect(160, 0, 16, 32);
	m_spritePlus.setCenter(0, 0);
}

SecondsObject::~SecondsObject() = default;

void SecondsObject::draw(FeRenderTarget* rw)
{
	if (m_visible == false)
	{
		return;
	}

	m_spriteN1.setSubRect(16 * (m_number / 60 % 10), 0, 16, 32);
	m_spriteN1.setCenter(0, 0);
	const int half = (m_number % 60) ? 1 : 0;
	m_spriteN2.setSubRect(176 + 16 * half, 0, 16, 32);
	m_spriteN2.setCenter(0, 0);

	m_spritePlus.setPosition(m_posX, m_posY);
	m_spriteN1.setPosition(m_spritePlus.getX() + 16 * m_scale, m_spritePlus.getY());
	m_spriteN2.setPosition(m_spriteN1.getX() + 16 * m_scale, m_spritePlus.getY());

	m_spritePlus.setScale(m_scale);
	m_spriteN1.setScale(m_scale);
	m_spriteN2.setScale(m_scale);

	m_spritePlus.draw(rw);
	m_spriteN1.draw(rw);
	m_spriteN2.draw(rw);
}

void SecondsObject::showAt(float x, float y, int n)
{
	m_number = n;
	m_posX = x;
	m_posY = y;
	m_visible = true;
	m_timer = 0;
}

void SecondsObject::move()
{
	m_timer++;
	if (m_timer < 20)
	{
		m_posY -= 3.0f * m_scale;
	}
}

//========================================================================================

LightEffect::LightEffect(GameData* gamedata, PosVectorFloat& startpv, PosVectorFloat& middlepv, PosVectorFloat& endpv)
{
	m_traileffect = 0;
	m_timer = 0;
	m_start = startpv; m_middle = middlepv; m_end = endpv;
	m_visible = true;
	m_sprite.setImage(gamedata->imgLight);
	m_sprite.setCenter();
	m_tail.setImage(gamedata->imgLight_s);
	m_tail.setCenter();
	m_tail.setScale(0.75);
	m_hitlight.setImage(gamedata->imgLight_hit);
	m_hitlight.setCenter();
	m_hitlight.setVisible(false);
}

LightEffect::~LightEffect() = default;

void LightEffect::setTimer(float timer)
{
	if (m_timer < 100)
	{
		m_timer += timer;
	}
	else
	{
		m_timer += 4.8f;
	}

	if (!m_visible)
	{
		return;
	}
}


void LightEffect::draw(FeRenderTarget* rw)
{
	// There is another condition for when this function should be called: EQ>0
	if (!m_visible)
	{
		return;
	}

	for (int i = 59; i >= 0; i--)
	{
		const float temp_timer = (m_timer - i / 3.0f) / 100.0f;
		const float x = m_start.x + (m_middle.x - m_start.x) * temp_timer + (m_middle.x + (m_end.x - m_middle.x) * temp_timer
			- (m_start.x + (m_middle.x - m_start.x) * temp_timer)) * temp_timer;
		const float y = m_start.y + (m_middle.y - m_start.y) * temp_timer + (m_middle.y + (m_end.y - m_middle.y) * temp_timer
			- (m_start.y + (m_middle.y - m_start.y) * temp_timer)) * temp_timer;
		m_tail.setPosition(x, y);
		m_tail.setTransparency(static_cast<float>(60 - i) / 60.0f);
		if (m_timer - static_cast<float>(i) / 3.0f < 100)
		{
			m_tail.draw(rw);
		}
		if (i == 0 && m_timer - static_cast<float>(i) / 3.0f < 100)
		{
			m_sprite.setPosition(x, y);
			m_sprite.draw(rw);
		}
	}

	// Hit light effect
	if (m_timer > 100 && m_timer < 250)
	{
		m_hitlight.setVisible(true);
		m_hitlight.setPosition(m_end.x, m_end.y);
		m_hitlight.setScale(1.0f + (m_timer - 100) / 200.0f);
		if (m_timer > 150 && m_timer < 200)
		{
			m_hitlight.setTransparency((200 - m_timer) / 50.0f);
		}
		if (m_timer > 220)
		{
			m_hitlight.setVisible(false);
		}
		m_hitlight.setRotation((m_timer - 100) / 2.0f);
		m_hitlight.draw(rw);
		m_hitlight.setRotation(-(m_timer - 100) / 2.0f);
		m_hitlight.draw(rw);
	}
}

//==================================================================

FeverLight::FeverLight(GameData* gamedata)
{
	m_visible = false;
	m_timer = 0;
	m_speed = 0;
	PosVectorFloat pv;
	pv.x = 0; pv.y = 0;
	m_start = pv; m_middle = pv; m_end = pv;
	m_sprite.setImage(gamedata->imgfLight);
	m_sprite.setCenter();
	m_sprite.setScale(0.5);
	m_hitLight.setImage(gamedata->imgfLight_hit);
	m_hitLight.setCenter();
}

FeverLight::~FeverLight() = default;

void FeverLight::setTimer(float timer)
{
	m_speed += timer;
	m_timer += m_speed;
}

void FeverLight::init(PosVectorFloat& startpv, PosVectorFloat& middlepv, PosVectorFloat& endpv)
{
	m_visible = true;
	m_timer = 0;
	m_speed = 0;
	m_start = startpv; m_middle = middlepv; m_end = endpv;
}

void FeverLight::draw(FeRenderTarget* rw)
{
	if (!m_visible)
	{
		return;
	}

	const float tempTimer = m_timer / 100.f;

	float x = (m_start.x + (m_middle.x - m_start.x) * tempTimer) + ((m_middle.x + (m_end.x - m_middle.x) * tempTimer) - (m_start.x + (m_middle.x - m_start.x) * tempTimer)) * tempTimer;
	float y = (m_start.y + (m_middle.y - m_start.y) * tempTimer) + ((m_middle.y + (m_end.y - m_middle.y) * tempTimer) - (m_start.y + (m_middle.y - m_start.y) * tempTimer)) * tempTimer;
	if (m_timer < 100)
	{
		m_sprite.setPosition(x, y);
		m_sprite.draw(rw);
	}
	else
	{
		// Draw hitlight at end
		m_hitLight.setPosition(m_end.x, m_end.y);
		m_hitLight.setScale(1.f + 2.f * atan((m_timer - 100.f) / 100.0f) * 2.f / PI);
		m_hitLight.setTransparency(max(0.0f, 1.f - (m_timer - 100.f) / 100.0f));
		m_hitLight.draw(rw);
	}

	if (m_timer > 200.f)
	{
		m_visible = false;
	}
}


//==================================================================

NuisanceTray::NuisanceTray() = default;

NuisanceTray::~NuisanceTray() = default;

void NuisanceTray::init(GameData* globalp)
{
	gamedata = globalp;
	darken = false;
	m_timer = 0;
	offsetX = 0;
	offsetY = 0;
	m_globalScale = 1;
	for (int i = 0; i < 6; i++)
	{
		m_sprite[i].setImage(gamedata->imgPuyo);
		setImage(i, 0);
	}
}

// Set position
void NuisanceTray::align(float x, float y, float scale)
{
	offsetX = x;
	offsetY = y;
	m_globalScale = scale;
	for (int i = 0; i < 6; i++)
	{
		m_sprite[i].setPosition(x + (PUYOX / 2 + PUYOX * i) * scale, y);
		m_sprite[i].setScale(scale);
	}
}

void NuisanceTray::setVisible(bool b)
{
	for (auto& i : m_sprite)
	{
		i.setVisible(b);
	}
}

void NuisanceTray::update(int trayValue)
{
	m_timer = 0;
	for (int i = 0; i < 6; i++)
	{
		int tempTray = 0;
		// See which fits
		if (trayValue >= 720 && tempTray == 0)
		{
			tempTray = 720; setImage(i, 6);
		}
		else if (trayValue >= 360 && tempTray == 0)
		{
			tempTray = 360; setImage(i, 5);
		}
		else if (trayValue >= 180 && tempTray == 0)
		{
			tempTray = 180; setImage(i, 4);
		}
		else if (trayValue >= 30 && tempTray == 0)
		{
			tempTray = 30; setImage(i, 3);
		}
		else if (trayValue >= 6 && tempTray == 0)
		{
			tempTray = 6; setImage(i, 2);
		}
		else if (trayValue >= 1 && tempTray == 0)
		{
			tempTray = 1; setImage(i, 1);
		}
		else if (trayValue <= 0 && tempTray == 0)
		{
			tempTray = 0; setImage(i, 0);
		}
		trayValue -= tempTray;
	}
	setVisible(true);
}

void NuisanceTray::play()
{
	for (int i = 0; i < 6; i++)
	{
		if (m_timer == 0)
		{
			m_animationTimer[i] = 0;
		}
		else
		{
			m_animationTimer[i] = max(m_timer - i * 3, 0.0f);
		}
		// Set correct height?

		// Do the scaling
		if (m_animationTimer[i] > 0 && m_animationTimer[i] < 160)
		{
			m_sprite[i].setScale((((std::exp(m_animationTimer[i] * -1.0f / 30.f) * -1.f) * std::cos((m_animationTimer[i] * 7.f - 45.f) * PI / 180.f) + 1.f) + std::exp(m_animationTimer[i] * -0.5f)) * m_globalScale);
		}
		else
		{
			m_sprite[i].setScale(m_globalScale);
		}
	}
	m_timer += 1.5f;
}

void NuisanceTray::draw()
{
	for (int i = 0; i < 6; i++)
	{
		darken ? m_sprite[i].setColor(180, 180, 180) : m_sprite[i].setColor(255, 255, 255);
		if (m_animationTimer[i] > 6 && m_timer > 1)
		{
			m_sprite[i].draw(gamedata->front);
		}
	}
}

void NuisanceTray::setImage(int spr, const int image)
{
	m_sprite[spr].setVisible(true);
	switch (image)
	{
	case 1:
		m_sprite[spr].setSubRect(PUYOX * 14, PUYOY * 12, PUYOX, PUYOY);
		break;
	case 2:
		m_sprite[spr].setSubRect(PUYOX * 13, PUYOY * 12, PUYOX, PUYOY);
		break;
	case 3:
		m_sprite[spr].setSubRect(PUYOX * 12, PUYOY * 12, PUYOX, PUYOY);
		break;
	case 4:
		m_sprite[spr].setSubRect(PUYOX * 12, PUYOY * 11, PUYOX, PUYOY);
		break;
	case 5:
		m_sprite[spr].setSubRect(PUYOX * 11, PUYOY * 11, PUYOX, PUYOY);
		break;
	case 6:
		m_sprite[spr].setSubRect(PUYOX * 10, PUYOY * 11, PUYOX, PUYOY);
		break;
	case 7:
		m_sprite[spr].setSubRect(PUYOX * 12, PUYOY * 7, 2 * PUYOX, 2 * PUYOY);
		break;
	default:
		// Don't show
		m_sprite[spr].setSubRect(0, 0, 0, 0);
		break;
	}
	m_sprite[spr].setCenter(PUYOX / 2, PUYOY / 2);
}

//======================================================================================

ScoreCounter::ScoreCounter() = default;

ScoreCounter::~ScoreCounter() = default;

void ScoreCounter::init(GameData* g, float x, float y, float scale)
{
	gamedata = g;
	timer = 121;
	score = 0;
	point = 0; bonus = 0;
	// Load and align images
	for (int i = 0; i < 9; i++)
	{
		m_sprite[i].setImage(gamedata->imgScore);
		setImage(i, 0);
		m_sprite[i].setPosition(x + (6 + i * 20) * scale, y);
		m_sprite[i].setScale(scale);
	}
}

void ScoreCounter::setCounter(int val)
{
	score = val;
}

void ScoreCounter::setPointBonus(int p, int b)
{
	point = p;
	bonus = b;
	timer = 0;
}

void ScoreCounter::draw()
{
	// Set sprites for normal view
	if (timer > 30 || point == 0)
	{
		for (int i = 0; i < 9; i++)
		{
			setImage(8 - i, (score / static_cast<int>(pow(10., i))) % 10);
		}
	}
	else // Show pointxbonus
	{
		int widthB = getDigits(bonus);
		int widthP = getDigits(point);
		for (auto& sprite : m_sprite)
		{
			sprite.setVisible(false);
		}
		// Bonus
		for (int i = 0; i < widthB; i++)
		{
			setImage(8 - i, (bonus / int(pow(10., i))) % 10);
		}
		// X
		setImage(8 - widthB, 10);

		// Point
		for (int i = 0; i < widthP; i++)
		{
			setImage(8 - i - widthB - 1, (point / int(pow(10., i))) % 10);
		}

	}

	for (auto& sprite : m_sprite)
	{
		sprite.draw(gamedata->front);
	}
	if (timer < 31)
	{
		timer++;
	}
}

void ScoreCounter::setImage(int spr, int val)
{
	m_sprite[spr].setSubRect(20 * val, 0, 20, 34);
	m_sprite[spr].setVisible(true);
}

}
