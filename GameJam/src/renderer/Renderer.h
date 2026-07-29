#pragma once
#include <vector>

#include "../core/GLValidation.h"
#include "../core/ResourceManager.h"
#include "Color.h"
#include "Shader.h"
#include "Sprite.h"
#include "Texture.h"

typedef struct {
	Rectangle rect;
	Color color;
	Sprite sprite;
} RenderObject;

typedef struct {
	Rectangle rect;
	Color color;
	u32 textureID;
} TextureRenderObject;

typedef struct {
	glm::vec2 position;
	glm::vec3 color;
	f32 radius;
} Light;

constexpr u32 MAX_OBJECTS = 2000;
constexpr u32 MAX_SHADOW_CASTER = 100;
constexpr u32 MAX_LIGHTS = 15;

class Renderer {
public:
	Renderer(u32 width, u32 height);

	void BeginFrame();
	void EndBackground();
	void BeginGUI();
	void EndFrame();
	//void RenderScreenShot(); SCREENSHOT

	void ClearBackground(const Color& color);
	void RenderRectangle(const glm::vec2& position, const glm::vec2& size, const Color& color);
	void RenderSprite(const glm::vec2& position, const glm::vec2& size, Sprite sprite);
	void RenderColoredSprite(const glm::vec2& position, const glm::vec2& size, Sprite sprite, const Color& color);
	void RenderShadowCaster(const glm::vec2& position, const glm::vec2& size);
	void RenderEmitter(const glm::vec2& position, const glm::vec2& size, const Color& color);
	void RenderSpriteEmitter(const glm::vec2& position, const glm::vec2& size, Sprite sprite, const Color& color);
	void RenderLight(const Light& light) { m_Lights.push_back(light); }
	void RenderTexture(const glm::vec2& position, const glm::vec2& size, u32 textureID, const Color& color);

	void RenderText(const std::string& text, const glm::vec2& position, f32 scale, const Color& color);
	f32 GetTextWidth(const std::string& text, f32 scale);

	void UpdateCameraPosition(glm::vec2 position) { m_CameraPosition = glm::mix(m_CameraPosition, position, 0.1f); }
	void SetTransparency(f32 transparency) { m_Transparency = transparency; }
private:
	void RenderObjectsBack();
	void RenderObjectsFront();
	void RenderShadows();
	void RenderEmitter();
	void RenderGUI();
	void RenderTextures();
private:
	u32 m_Width;
	u32 m_Height;
	Color m_BackgroundColor;
	f32 m_Transparency;

	bool m_RenderingBackground;
	bool m_RenderingGUI;
	std::vector<RenderObject> m_RenderObjectsBack;
	std::vector<RenderObject> m_RenderObjectsFront;
	std::vector<RenderObject> m_ShadowCaster;
	std::vector<RenderObject> m_Emitter;
	std::vector<Light> m_Lights;
	std::vector<RenderObject> m_GUIObjects;
	std::vector<TextureRenderObject> m_TextureObjects;

	Shader m_BasicShader;
	Shader m_ShadowShader;
	Shader m_DownsampleShader;
	Shader m_UpsampleShader;
	Shader m_PresentShader;
	Shader m_GUIShader;
	Shader m_TextureShader;
	glm::mat4 m_Projection;
	glm::mat4 m_View;
	glm::vec2 m_CameraPosition;

	u32 m_FrameBufferBack;
	u32 m_ColorTextureBack;
	u32 m_FrameBufferFront;
	u32 m_ColorTextureFront;
	u32 m_ShadowFramebuffer;
	u32 m_ShadowTextures;
	u32 m_BloomFrameBuffer;
	u32 m_BloomTextures[10];
	glm::vec2 m_BloomTexturesSizes[10];
	u32 m_EmitterTexture;

	//u32 m_ScreenShotFrameBuffer; SCREENSHOT
	//u32 m_ScreenShotTexture;

	u32 m_ModelMatrixBuffer;
	u32 m_ColorBuffer;
	u32 m_SpriteBuffer;

	u32 m_QuadVAO;
	u32 m_QuadVBO;
	u32 m_QuadEBO;

	Texture m_TextureAtlas;
};