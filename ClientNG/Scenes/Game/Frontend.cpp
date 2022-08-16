#include "Frontend.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

GameFrontend::GameFrontend(RenderTarget* target)
	: m_target(target)
{
	m_blankTexture = m_target->makeTexture();
	m_quadBuffer = m_target->makeBuffer();

	m_quadVertices[0].tex = { 0, 1 };
	m_quadVertices[1].tex = { 1, 1 };
	m_quadVertices[2].tex = { 1, 0 };
	m_quadVertices[3].tex = { 0, 0 };
	m_quadVertices[0].pos = { -1, -1, 0 };
	m_quadVertices[1].pos = { 1, -1, 0 };
	m_quadVertices[2].pos = { 1, 1, 0 };
	m_quadVertices[3].pos = { -1, 1, 0 };
	m_quadVertices[0].color = { 1, 1, 1, 1 };
	m_quadVertices[1].color = { 1, 1, 1, 1 };
	m_quadVertices[2].color = { 1, 1, 1, 1 };
	m_quadVertices[3].color = { 1, 1, 1, 1 };
	m_quadBuffer->uploadIndices(m_quadIndices, std::size(m_quadIndices));
	m_quadBuffer->uploadVertices(m_quadVertices, std::size(m_quadVertices));

	m_matrixStack.push(glm::ortho(0.f, 640.f, 480.f, 0.f, -1.f, 1.f));
}

GameFrontend::~GameFrontend() = default;

ppvs::FeImage* GameFrontend::loadImage(const char* nameu8)
{
	return loadImage(std::string(nameu8));
}

ppvs::FeImage* GameFrontend::loadImage(const std::string& nameu8)
{
	std::string name = nameu8;

	// Read entire file into memory
	FILE* f = fopen(name.c_str(), "rb");
	if (!f)
		return nullptr;
	fseek(f, 0, SEEK_END);
	const size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);
	const auto data = std::unique_ptr<char[]>(new char[size]);
	fread(data.get(), 1, size, f);
	fclose(f);

	auto texture = m_target->makeTexture();
	texture->loadPng(data.get(), size);
	return new GameImage(std::move(texture));
}

ppvs::FeShader* GameFrontend::loadShader(const char* source)
{
	return nullptr;
}

ppvs::FeFont* GameFrontend::loadFont(const char* nameu8, double fontSize)
{
	return new GameFont();
}

ppvs::FeSound* GameFrontend::loadSound(const char* nameu8)
{
	return new GameSound();
}

ppvs::FeSound* GameFrontend::loadSound(const std::string& nameu8)
{
	return loadSound(nameu8.c_str());
}

void GameFrontend::musicEvent(ppvs::FeMusicEvent event)
{
}

void GameFrontend::musicVolume(float volume, bool fever)
{
}

ppvs::FeInput GameFrontend::inputState(int pl)
{
	return m_inputState;
}

void GameFrontend::pushMatrix()
{
	m_matrixStack.push(m_matrixStack.top());
}

void GameFrontend::popMatrix()
{
	m_matrixStack.pop();
}

void GameFrontend::identity()
{
	m_matrixStack.top() = glm::mat4(1.0f);
}

void GameFrontend::translate(const float x, const float y, const float z)
{
	m_matrixStack.top() = glm::translate(m_matrixStack.top(), glm::vec3(x, y, z));
}

void GameFrontend::rotate(const float v, const float x, const float y, const float z)
{
	m_matrixStack.top() = glm::rotate(m_matrixStack.top(), v, glm::vec3(x, y, z));
}

void GameFrontend::scale(const float x, const float y, const float z)
{
	m_matrixStack.top() = glm::scale(m_matrixStack.top(), glm::vec3(x, y, z));
}

ppvs::ViewportGeometry GameFrontend::viewport()
{
	auto& viewport = m_target->viewport();
	return { viewport.x, viewport.y };
}

bool GameFrontend::hasShaders()
{
	return false;
}

void GameFrontend::setBlendMode(ppvs::BlendingMode b)
{
	switch (b) {
    case ppvs::BlendingMode::NoBlending:
        m_target->setBlendMode(BlendingMode::NoBlending);
        break;
    case ppvs::BlendingMode::AlphaBlending:
        m_target->setBlendMode(BlendingMode::AlphaBlending);
        break;
    case ppvs::BlendingMode::AdditiveBlending:
        m_target->setBlendMode(BlendingMode::AdditiveBlending);
        break;
    case ppvs::BlendingMode::MultiplyBlending:
        m_target->setBlendMode(BlendingMode::MultiplyBlending);
        break;
    }
}

void GameFrontend::setColor(const int r, const int g, const int b, const int a)
{
	m_color = { static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f, static_cast<float>(a) / 255.f };
}

void GameFrontend::unsetColor()
{
	m_color = { 1, 1, 1, 1 };
}

void GameFrontend::setDepthFunction(const ppvs::DepthFunction func)
{
	switch (func) {
    case ppvs::DepthFunction::Always:
        m_target->setDepthFunction(DepthFunction::Always);
        break;
    case ppvs::DepthFunction::LessOrEqual:
        m_target->setDepthFunction(DepthFunction::LessOrEqual);
        break;
    case ppvs::DepthFunction::GreaterOrEqual:
        m_target->setDepthFunction(DepthFunction::GreaterOrEqual);
        break;
    case ppvs::DepthFunction::Equal:
        m_target->setDepthFunction(DepthFunction::Equal);
        break;
    }
}

void GameFrontend::clearDepth()
{
	m_target->clearDepth();
}

void GameFrontend::enableAlphaTesting(float tolerance)
{
	// Not supported.
	return;
}

void GameFrontend::disableAlphaTesting()
{
	// Not supported.
	return;
}

void GameFrontend::drawRect(ppvs::FeImage* image, double subx, double suby, double subw, double subh)
{
	double u1, v1, u2, v2, w = subw, h = subh;

	u1 = v1 = u2 = v2 = 0.;

	if (image) {
		static_cast<GameImage*>(image)->bind();
		const double tw = image->width(), th = image->height();
		u1 = (subx + 0.25) / tw;
		v1 = 1. - (suby + subh - 0.5) / th;
		u2 = (subx + subw - 0.25) / tw;
		v2 = 1. - (suby + 0.5) / th;
	} else {
		m_blankTexture->bind(0);
	}

	m_quadVertices[0].tex = { u1, 1.0 - v2 };
	m_quadVertices[1].tex = { u2, 1.0 - v2 };
	m_quadVertices[2].tex = { u2, 1.0 - v1 };
	m_quadVertices[3].tex = { u1, 1.0 - v1 };
	m_quadVertices[0].pos = { 0 + 0.25, 0 + 0.25, 0 };
	m_quadVertices[1].pos = { w - 0.25, 0 + 0.25, 0 };
	m_quadVertices[2].pos = { w - 0.25, h + 0.25, 0 };
	m_quadVertices[3].pos = { 0 + 0.25, h + 0.25, 0 };
	m_quadVertices[0].color = m_color;
	m_quadVertices[1].color = m_color;
	m_quadVertices[2].color = m_color;
	m_quadVertices[3].color = m_color;
	m_quadBuffer->uploadVertices(m_quadVertices, std::size(m_quadVertices));
	m_target->setModelView(m_matrixStack.top());
	m_quadBuffer->render(PolyShader::Simple);
}

void GameFrontend::clear()
{
	m_target->clear(0, 0, 0, 1);
}

void GameFrontend::swapBuffers()
{
}

void GameFrontend::setInputState(ppvs::FeInput inputState)
{
	m_inputState = inputState;
}

GameImage::GameImage(std::unique_ptr<Texture> texture)
	: m_texture(std::move(texture))
{
}

GameImage::~GameImage() = default;

int GameImage::width()
{
	return m_texture->width();
}

int GameImage::height()
{
	return m_texture->height();
}

ppvs::FePixel GameImage::pixel(int x, int y)
{
	return ppvs::FePixel(0, 0, 0, 0);
}

bool GameImage::error()
{
	return false;
}

void GameImage::setFilter(ppvs::FilterType)
{
	// Not implemented yet.
	return;
}

GameFont::GameFont()
{
}

GameFont::~GameFont()
{
}

ppvs::FeText* GameFont::render(const char* str)
{
	return new GameText();
}

GameText::GameText()
{
}

GameText::~GameText()
{
}

void GameText::draw(float x, float y)
{
}

void GameSound::play()
{
}

void GameSound::stop()
{
}
