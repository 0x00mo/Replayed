#include "Renderer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Renderer::Renderer(u32 width, u32 height) : 
	m_BackgroundColor(Color::RGBFromHex(0x000000)),
	m_BasicShader(Shader("res/shaders/basic.vs", "res/shaders/basic.fs")),
	m_ShadowShader(Shader("res/shaders/shadow.gs", "res/shaders/shadow.vs", "res/shaders/shadow.fs")),
	m_DownsampleShader(Shader("res/shaders/downsample.vs", "res/shaders/downsample.fs")),
	m_UpsampleShader(Shader("res/shaders/upsample.vs", "res/shaders/upsample.fs")),
	m_PresentShader(Shader("res/shaders/present.vs", "res/shaders/present.fs")),
	m_GUIShader(Shader("res/shaders/gui.vs", "res/shaders/gui.fs")),
	m_TextureShader(Shader("res/shaders/texture.vs", "res/shaders/texture.fs")),
	m_Projection(glm::ortho(0.0f, (f32)width, 0.0f, (f32)height, -1.0f, 1.0f)),
	m_Width(width),
	m_Height(height)
{

	GlCall(glViewport(0, 0, m_Width, m_Height));
	GlCall(glEnable(GL_BLEND));
	GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	//GlCall(glGenTextures(1, &m_ScreenShotTexture)); SCREENSHOT
	//GlCall(glBindTexture(GL_TEXTURE_2D, m_ScreenShotTexture));
	//GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, 0));
	//
	//GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	//GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	//GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	//GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	//
	//GlCall(glGenFramebuffers(1, &m_ScreenShotFrameBuffer));
	//GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_ScreenShotFrameBuffer));
	//GlCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_ScreenShotTexture, 0));
	//GLenum attachments100[] = { GL_COLOR_ATTACHMENT0 };
	//GlCall(glDrawBuffers(1, attachments100));
	//u32 status1 = GlCall(glCheckFramebufferStatus(GL_FRAMEBUFFER));
	//if (status1 != GL_FRAMEBUFFER_COMPLETE) {
	//	// TODO: Error
	//}
	//GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	GlCall(glGenTextures(1, &m_ColorTextureBack));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_ColorTextureBack));
	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, 0));

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

	GlCall(glGenFramebuffers(1, &m_FrameBufferBack));
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferBack));
	GlCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_ColorTextureBack, 0));
	GLenum attachments[] = { GL_COLOR_ATTACHMENT0 };
	GlCall(glDrawBuffers(1, attachments));
	u32 status = GlCall(glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		// TODO: Error
	}
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	GlCall(glGenTextures(1, &m_ColorTextureFront));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_ColorTextureFront));
	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, 0));

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

	GlCall(glGenFramebuffers(1, &m_FrameBufferFront));
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferFront));
	GlCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_ColorTextureFront, 0));
	GLenum attachments1[] = { GL_COLOR_ATTACHMENT0 };
	GlCall(glDrawBuffers(1, attachments));
	status = GlCall(glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		// TODO: Error
	}
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	GlCall(glGenTextures(1, &m_ShadowTextures));
	GlCall(glBindTexture(GL_TEXTURE_2D_ARRAY, m_ShadowTextures));

	GlCall(glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_R8, m_Width, m_Height, MAX_LIGHTS));

	GlCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GlCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GlCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GlCall(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

	GlCall(glGenFramebuffers(1, &m_ShadowFramebuffer));
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFramebuffer));
	GlCall(glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_ShadowTextures, 0, 0));
	GLenum attachments2[] = { GL_COLOR_ATTACHMENT0 };
	GlCall(glDrawBuffers(1, attachments2));
	status = GlCall(glCheckNamedFramebufferStatus(m_ShadowFramebuffer, GL_FRAMEBUFFER));
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		// TODO: Error
	}
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	glm::ivec2 textureSize = glm::ivec2(m_Width, m_Height);
	for (u32 i = 0; i < 10; i += 2) {
		textureSize /= 2;
		m_BloomTexturesSizes[i] = textureSize;
		m_BloomTexturesSizes[i + 1] = textureSize;
		GlCall(glGenTextures(1, &m_BloomTextures[i]));
		GlCall(glBindTexture(GL_TEXTURE_2D, m_BloomTextures[i]));
		GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F, textureSize.x, textureSize.y, 0, GL_RGB, GL_FLOAT, 0));

		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

		GlCall(glGenTextures(1, &m_BloomTextures[i + 1]));
		GlCall(glBindTexture(GL_TEXTURE_2D, m_BloomTextures[i + 1]));
		GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F, textureSize.x, textureSize.y, 0, GL_RGB, GL_FLOAT, 0));
		
		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
		GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
	}
	GlCall(glGenTextures(1, &m_EmitterTexture));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_EmitterTexture));
	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, 0));

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

	GlCall(glGenFramebuffers(1, &m_BloomFrameBuffer));
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BloomFrameBuffer));
	GlCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_EmitterTexture, 0));
	GLenum attachments3[] = { GL_COLOR_ATTACHMENT0 };
	GlCall(glDrawBuffers(1, attachments3));
	status = GlCall(glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		// TODO: Error
	}
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	GlCall(glCreateBuffers(1, &m_ModelMatrixBuffer));
	GlCall(glNamedBufferStorage(m_ModelMatrixBuffer, sizeof(glm::mat4) * MAX_OBJECTS, 0, GL_DYNAMIC_STORAGE_BIT));
	GlCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ModelMatrixBuffer));

	GlCall(glCreateBuffers(1, &m_ColorBuffer));
	GlCall(glNamedBufferStorage(m_ColorBuffer, sizeof(glm::vec4) * MAX_OBJECTS, 0, GL_DYNAMIC_STORAGE_BIT));
	GlCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_ColorBuffer));

	GlCall(glCreateBuffers(1, &m_SpriteBuffer));
	GlCall(glNamedBufferStorage(m_SpriteBuffer, sizeof(Sprite) * MAX_OBJECTS, 0, GL_DYNAMIC_STORAGE_BIT));
	GlCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_SpriteBuffer));

	typedef struct {
		glm::vec3 pos;
		glm::vec2 texCoord;
	} Vertex;

	std::vector<Vertex> vertices = {
		 Vertex(glm::vec3(0.5f,  0.5f, 0.0f), glm::vec2(1.0f, 1.0f)),  // top right
		 Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f)),  // bottom right
		 Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f)),  // bottom left
		 Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec2(0.0f, 1.0f))   // top left 
	};

	std::vector<u32> indices = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	GlCall(glGenVertexArrays(1, &m_QuadVAO));
	GlCall(glGenBuffers(1, &m_QuadVBO));
	GlCall(glGenBuffers(1, &m_QuadEBO));

	GlCall(glBindVertexArray(m_QuadVAO));
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO));
	GlCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW));

	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadEBO));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * indices.size(), indices.data(), GL_STATIC_DRAW));

	GlCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
	GlCall(glEnableVertexAttribArray(0));
	GlCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float))));
	GlCall(glEnableVertexAttribArray(1));

	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GlCall(glBindVertexArray(0));

	m_TextureAtlas = Texture("res/textures/atlas.png");

	m_RenderObjectsBack.reserve(MAX_OBJECTS);
	m_RenderObjectsFront.reserve(MAX_OBJECTS);

	m_Transparency = 1.0f;
}

void Renderer::BeginFrame() {
	GlCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferFront));
	GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	GlCall(glClearColor(m_BackgroundColor.R(), m_BackgroundColor.G(), m_BackgroundColor.B(), 1.0f));
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferBack));
	GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	m_RenderingBackground = true;
	m_RenderingGUI = false;
	m_RenderObjectsBack.clear();
	m_RenderObjectsFront.clear();
	m_ShadowCaster.clear();
	m_Emitter.clear();
	m_Lights.clear();
	m_GUIObjects.clear();
	m_TextureObjects.clear();

	m_View = glm::translate(glm::mat4(1.0f), glm::vec3((-m_CameraPosition.x + 0.5f) * (f32)m_Width, (-m_CameraPosition.y + 0.25f) * (f32)m_Width, 0.0f));
	//m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); SCREENSHOT
	//m_CameraPosition = glm::vec2(0.5f, 0.25f);
}

void Renderer::EndBackground() {
	m_RenderingBackground = false;
}

void Renderer::BeginGUI() {
	m_RenderingGUI = true;
}

void Renderer::EndFrame() {
	RenderObjectsBack();
	RenderObjectsFront();
	RenderShadows();
	RenderEmitter();

	m_PresentShader.Use();
	m_PresentShader.SetMat4("projection", m_Projection);
	m_PresentShader.SetMat4("view", m_View);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3((f32)m_Width / 2.0f, (f32)m_Height / 2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(m_Width, m_Height, 1.0f));
	m_PresentShader.SetMat4("model", model);

	GlCall(glActiveTexture(GL_TEXTURE0));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_ColorTextureBack)); 
	GlCall(glActiveTexture(GL_TEXTURE1));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_ColorTextureFront));
	GlCall(glActiveTexture(GL_TEXTURE2));
	GlCall(glBindTexture(GL_TEXTURE_2D_ARRAY, m_ShadowTextures));
	GlCall(glActiveTexture(GL_TEXTURE3));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_BloomTextures[1]));
	m_PresentShader.SetInt("tex1", 0);
	m_PresentShader.SetInt("tex2", 1);
	m_PresentShader.SetInt("shadow", 2);
	m_PresentShader.SetInt("bloom", 3);
	m_PresentShader.SetFloat("alpha", m_Transparency);

	for (u32 i = 0; i < m_Lights.size(); i++) {
		m_PresentShader.SetVec2("lights[" + std::to_string(i) + "].position", (m_Lights.at(i).position + glm::vec2(-m_CameraPosition.x + 0.5f, -m_CameraPosition.y + 0.25f)) * (f32)m_Width);
		m_PresentShader.SetVec3("lights[" + std::to_string(i) + "].color", m_Lights.at(i).color);
		m_PresentShader.SetFloat("lights[" + std::to_string(i) + "].radius", m_Lights.at(i).radius);
	}
	m_PresentShader.SetInt("lightCount", m_Lights.size());

	//GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_ScreenShotFrameBuffer)); SCREENSHOT
	GlCall(glBindVertexArray(m_QuadVAO));
	GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
	GlCall(glBindVertexArray(0));

	RenderGUI();
	RenderTextures();
}

//static bool saved = false; SCREENSHOT
//void Renderer::RenderScreenShot() {
//	if (saved) return;
//	std::vector<u8> pixels;
//	pixels.resize(m_Width * m_Height * 4);
//	GlCall(glReadPixels(0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data()));
//
//	std::vector<u8> flipped;
//	flipped.resize(m_Width * m_Height * 4);
//	for (int y = 0; y < m_Height; ++y) {
//		memcpy(
//			&flipped[y * m_Width * 4],
//			&pixels[(m_Height - 1 - y) * m_Width * 4],
//			m_Width * 4
//		);
//	}
//
//	stbi_write_png("res/textures/lvl5.png", m_Width, m_Height, 4, flipped.data(), m_Width * 4);
//	saved = true;
//}

void Renderer::ClearBackground(const Color& color) {
	m_BackgroundColor = color;
}

void Renderer::RenderRectangle(const glm::vec2& position, const glm::vec2& size, const Color& color) {
	if (m_RenderingGUI) {
		m_GUIObjects.push_back(RenderObject{ Rectangle{ position.x, position.y, size.x, size.y }, color, ResourceManager::Sprites["white"] });
	}
	else if (m_RenderingBackground) {
		m_RenderObjectsBack.push_back(RenderObject{ Rectangle{ position.x, position.y, size.x, size.y }, color, ResourceManager::Sprites["white"] });
	}
	else {
		m_RenderObjectsFront.push_back(RenderObject{ Rectangle{ position.x, position.y, size.x, size.y }, color, ResourceManager::Sprites["white"] });
	}
}

void Renderer::RenderSprite(const glm::vec2& position, const glm::vec2& size, Sprite sprite) {
	if (m_RenderingGUI) {
		m_GUIObjects.push_back(RenderObject{ Rectangle{ position.x, position.y, size.x, size.y }, Color::RGBFromHex(0xFFFFFF), sprite });
	}
	else if (m_RenderingBackground) {
		m_RenderObjectsBack.push_back(RenderObject{ Rectangle{ position.x, position.y, size.x, size.y }, Color::RGBFromHex(0xFFFFFF), sprite });
	}
	else {
		m_RenderObjectsFront.push_back(RenderObject{ Rectangle{ position.x, position.y, size.x, size.y }, Color::RGBFromHex(0xFFFFFF), sprite });
	}
}

void Renderer::RenderColoredSprite(const glm::vec2& position, const glm::vec2& size, Sprite sprite, const Color& color){
	if (m_RenderingGUI) {
		m_GUIObjects.push_back(RenderObject{ Rectangle{ position.x, position.y, size.x, size.y }, color, sprite });
	}
	else if (m_RenderingBackground) {
		m_RenderObjectsBack.push_back(RenderObject{ Rectangle{ position.x, position.y, size.x, size.y }, color, sprite });
	}
	else {
		m_RenderObjectsFront.push_back(RenderObject{ Rectangle{ position.x, position.y, size.x, size.y }, color, sprite });
	}
}

void Renderer::RenderShadowCaster(const glm::vec2& position, const glm::vec2& size) {
	m_ShadowCaster.push_back(RenderObject{ Rectangle{ position.x, position.y, size.x, size.y }, Color::RGBFromHex(0xFFFFFF), ResourceManager::Sprites["white"] });
}

void Renderer::RenderEmitter(const glm::vec2& position, const glm::vec2& size, const Color& color) {
	m_Emitter.push_back(RenderObject{ Rectangle{ position.x, position.y, size.x, size.y }, color, ResourceManager::Sprites["white"] });
}

void Renderer::RenderSpriteEmitter(const glm::vec2& position, const glm::vec2& size, Sprite sprite, const Color& color) {
	m_Emitter.push_back(RenderObject{ Rectangle{ position.x, position.y, size.x, size.y }, color, sprite });
}

void Renderer::RenderTexture(const glm::vec2& position, const glm::vec2& size, u32 textureID, const Color& color) {
	m_TextureObjects.push_back(TextureRenderObject{ Rectangle{ position.x, position.y, size.x, size.y }, color, textureID });
}

void Renderer::RenderText(const std::string& text, const glm::vec2& position, f32 scale, const Color& color) {
	f32 cursor = position.x - scale;
	for (u8 c : text) {
		if (c == ' ') {
			cursor += 4 * scale;
		}
		else {
			std::string character = std::string(1, c);
			cursor += scale;
			f32 charWidth = (f32)ResourceManager::Sprites[character].width * scale;
			f32 charHeight = (f32)ResourceManager::Sprites[character].height * scale;
			RenderColoredSprite(glm::vec2(cursor, position.y), glm::vec2(charWidth, charHeight), ResourceManager::Sprites[character], color);
			cursor += charWidth;
		}
	}
}

f32 Renderer::GetTextWidth(const std::string& text, f32 scale) {
	i32 textWidth = -1;
	for (u8 c : text) {
		if (c == ' ') {
			textWidth += 4;
		}
		else if (!ResourceManager::Sprites.contains(std::string(1, c))) {
			assert(false && "Character not in Texture-Atlas");
		}
		else {
			textWidth++;
			textWidth += ResourceManager::Sprites[std::string(1, c)].width;
		}
	}
	return (f32)textWidth * scale;
}

void Renderer::RenderObjectsBack() {
	assert(m_RenderObjectsBack.size() <= MAX_OBJECTS && "Rendering too many objects");

	std::vector<glm::mat4> modelMatrices;
	std::vector<glm::vec4> colors;
	std::vector<Sprite> sprites;
	modelMatrices.reserve(MAX_OBJECTS);
	colors.reserve(MAX_OBJECTS);
	sprites.reserve(MAX_OBJECTS);

	for (RenderObject& object : m_RenderObjectsBack) {
		Rectangle& rect = object.rect;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3((rect.x + rect.width / 2) * m_Width, (rect.y + rect.height / 2) * m_Width, 0.0f));
		model = glm::scale(model, glm::vec3(rect.width * m_Width, rect.height * m_Width, 1.0f));
		modelMatrices.push_back(model);

		colors.push_back(object.color.ToVec4());
		sprites.push_back(object.sprite);
	}

	GlCall(glNamedBufferSubData(m_ModelMatrixBuffer, 0, sizeof(glm::mat4) * modelMatrices.size(), modelMatrices.data()));
	GlCall(glNamedBufferSubData(m_ColorBuffer, 0, sizeof(glm::vec4) * colors.size(), colors.data()));
	GlCall(glNamedBufferSubData(m_SpriteBuffer, 0, sizeof(Sprite) * sprites.size(), sprites.data()));

	m_BasicShader.Use();
	m_BasicShader.SetMat4("projection", m_Projection);

	GlCall(glActiveTexture(GL_TEXTURE0));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_TextureAtlas.GetID()));
	m_BasicShader.SetInt("textureAtlas", 0);
	m_BasicShader.SetMat4("view", m_View);

	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferBack));
	GlCall(glBindVertexArray(m_QuadVAO));
	GlCall(glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_RenderObjectsBack.size()));
	GlCall(glBindVertexArray(0));
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Renderer::RenderObjectsFront() {
	assert(m_RenderObjectsFront.size() <= MAX_OBJECTS && "Rendering too many objects");

	std::vector<glm::mat4> modelMatrices;
	std::vector<glm::vec4> colors;
	std::vector<Sprite> sprites;
	modelMatrices.reserve(MAX_OBJECTS);
	colors.reserve(MAX_OBJECTS);
	sprites.reserve(MAX_OBJECTS);

	for (RenderObject& object : m_RenderObjectsFront) {
		Rectangle& rect = object.rect;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3((rect.x + rect.width / 2) * m_Width, (rect.y + rect.height / 2) * m_Width, 0.0f));
		model = glm::scale(model, glm::vec3(rect.width * m_Width, rect.height * m_Width, 1.0f));
		modelMatrices.push_back(model);

		colors.push_back(object.color.ToVec4());
		sprites.push_back(object.sprite);
	}

	GlCall(glNamedBufferSubData(m_ModelMatrixBuffer, 0, sizeof(glm::mat4) * modelMatrices.size(), modelMatrices.data()));
	GlCall(glNamedBufferSubData(m_ColorBuffer, 0, sizeof(glm::vec4) * colors.size(), colors.data()));
	GlCall(glNamedBufferSubData(m_SpriteBuffer, 0, sizeof(Sprite) * sprites.size(), sprites.data()));

	m_BasicShader.Use();
	m_BasicShader.SetMat4("projection", m_Projection);

	GlCall(glActiveTexture(GL_TEXTURE0));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_TextureAtlas.GetID()));
	m_BasicShader.SetInt("textureAtlas", 0);
	m_BasicShader.SetMat4("view", m_View);

	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferFront));
	GlCall(glBindVertexArray(m_QuadVAO));
	GlCall(glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_RenderObjectsFront.size()));
	GlCall(glBindVertexArray(0));
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Renderer::RenderShadows() {
	assert(m_ShadowCaster.size() <= MAX_SHADOW_CASTER && "Rendering too many shadow caster");
	GlCall(glDisable(GL_BLEND));
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFramebuffer));
	for (u32 i = 0; i < m_Lights.size(); i++) {
		GlCall(glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_ShadowTextures, 0, i));
		GLenum attachments[] = { GL_COLOR_ATTACHMENT0 };
		GlCall(glDrawBuffers(1, attachments));
		u32 status = GlCall(glCheckNamedFramebufferStatus(m_ShadowFramebuffer, GL_FRAMEBUFFER));
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			// TODO: Error
		}
		GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		for (u32 j = 0; j < m_ShadowCaster.size(); j++) {
			RenderObject shadowCaster = m_ShadowCaster.at(j);
			Rectangle rect = shadowCaster.rect;

			m_ShadowShader.Use();
			m_ShadowShader.SetMat4("projection", m_Projection);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3((rect.x + rect.width / 2) * m_Width, (rect.y + rect.height / 2) * m_Width, 0.0f));
			model = glm::scale(model, glm::vec3(rect.width * m_Width, rect.height * m_Width, 1.0f));
			m_ShadowShader.SetMat4("model", model);
			m_ShadowShader.SetMat4("view", m_View);
			m_ShadowShader.SetVec2("lightPos", (m_Lights.at(i).position + glm::vec2(-m_CameraPosition.x + 0.5f, -m_CameraPosition.y + 0.25f)) * (f32)m_Width);
			m_ShadowShader.SetVec2("resolution", glm::vec2(m_Width, m_Height));
			GlCall(glBindVertexArray(m_QuadVAO));
			GlCall(glDrawArrays(GL_LINE_LOOP, 0, 4));
			GlCall(glBindVertexArray(0));
		}
	}
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GlCall(glEnable(GL_BLEND));
}

void Renderer::RenderEmitter() {
	assert(m_Emitter.size() <= MAX_OBJECTS && "Rendering too many emitters");
	std::vector<glm::mat4> modelMatrices;
	std::vector<glm::vec4> colors;
	std::vector<Sprite> sprites;
	modelMatrices.reserve(MAX_OBJECTS);
	colors.reserve(MAX_OBJECTS);
	sprites.reserve(MAX_OBJECTS);

	for (RenderObject& object : m_Emitter) {
		Rectangle& rect = object.rect;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3((rect.x + rect.width / 2) * m_Width, (rect.y + rect.height / 2) * m_Width, 0.0f));
		model = glm::scale(model, glm::vec3(rect.width * m_Width, rect.height * m_Width, 1.0f));
		modelMatrices.push_back(model);

		colors.push_back(object.color.ToVec4());
		sprites.push_back(object.sprite);
	}

	GlCall(glNamedBufferSubData(m_ModelMatrixBuffer, 0, sizeof(glm::mat4) * modelMatrices.size(), modelMatrices.data()));
	GlCall(glNamedBufferSubData(m_ColorBuffer, 0, sizeof(glm::vec4) * colors.size(), colors.data()));
	GlCall(glNamedBufferSubData(m_SpriteBuffer, 0, sizeof(Sprite) * sprites.size(), sprites.data()));

	m_BasicShader.Use();
	m_BasicShader.SetMat4("projection", m_Projection);
	m_BasicShader.SetMat4("view", m_View);

	GlCall(glActiveTexture(GL_TEXTURE0));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_TextureAtlas.GetID()));
	m_BasicShader.SetInt("textureAtlas", 0);

	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BloomFrameBuffer));
	GlCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_EmitterTexture, 0));
	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BloomFrameBuffer));
	GlCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GlCall(glBindVertexArray(m_QuadVAO));
	GlCall(glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_Emitter.size()));
	GlCall(glBindVertexArray(0));

	m_DownsampleShader.Use();
	m_DownsampleShader.SetMat4("projection", m_Projection);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3((f32)m_Width / 2.0f, (f32)m_Height / 2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(m_Width, m_Height, 1.0f));
	m_DownsampleShader.SetMat4("model", model);

	glActiveTexture(GL_TEXTURE0);
	GlCall(glBindTexture(GL_TEXTURE_2D, m_EmitterTexture));
	m_DownsampleShader.SetInt("srcTexture", 0);

	glm::ivec2 mipSize = glm::ivec2(m_Width, m_Height);
	for (int i = 0; i < 10; i += 2) {
		mipSize /= 2;
		GlCall(glViewport(0, 0, mipSize.x, mipSize.y));
		GlCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BloomTextures[i], 0));
		GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BloomFrameBuffer));
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cerr << "FBO not complete!" << std::endl;
		}

		//m_DownsampleShader.SetBool("horizontal", true);

		GlCall(glBindVertexArray(m_QuadVAO));
		GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
		GlCall(glBindVertexArray(0));
		glActiveTexture(GL_TEXTURE0);
		GlCall(glBindTexture(GL_TEXTURE_2D, m_BloomTextures[i]));

		//m_DownsampleShader.SetBool("horizontal", false);
		//glActiveTexture(GL_TEXTURE0);
		//GlCall(glBindTexture(GL_TEXTURE_2D, m_BloomTextures[i]));
		//GlCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BloomTextures[i + 1], 0));
		//GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BloomFrameBuffer));
		//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		//	std::cerr << "FBO not complete!" << std::endl;
		//}
		//
		//GlCall(glBindVertexArray(m_QuadVAO));
		//GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
		//GlCall(glBindVertexArray(0));
	}

	m_UpsampleShader.Use();
	m_UpsampleShader.SetMat4("projection", m_Projection);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3((f32)m_Width / 2.0f, (f32)m_Height / 2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(m_Width, m_Height, 1.0f));
	m_UpsampleShader.SetMat4("model", model);

	m_UpsampleShader.SetInt("srcTexture1", 0);
	m_UpsampleShader.SetInt("srcTexture2", 1);

	bool first = true;
	for (int i = 9; i > 0; i -= 2) {
		u32 mip1;
		u32 mip2;
		u32 mip3;
		if (first) {
			mip1 = 8;
			mip2 = 6;
			mip3 = 7;
			first = false;
			i--;
		}
		else {
			mip1 = i + 1;
			mip2 = i - 2;
			mip3 = i - 1;
		}
		GlCall(glActiveTexture(GL_TEXTURE0));
		GlCall(glBindTexture(GL_TEXTURE_2D, m_BloomTextures[mip1]));
		GlCall(glActiveTexture(GL_TEXTURE1));
		GlCall(glBindTexture(GL_TEXTURE_2D, m_BloomTextures[mip2]));
		GlCall(glViewport(0, 0, m_BloomTexturesSizes[mip2].x, m_BloomTexturesSizes[mip2].y));
		GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BloomFrameBuffer));
		GlCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BloomTextures[mip3], 0));
		GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BloomFrameBuffer));
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cerr << "FBO not complete!" << std::endl;
		}
		
		GlCall(glBindVertexArray(m_QuadVAO));
		GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
		GlCall(glBindVertexArray(0));
	}

	GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GlCall(glViewport(0, 0, m_Width, m_Height));
}

void Renderer::RenderGUI() {
	assert(m_GUIObjects.size() <= MAX_OBJECTS && "Rendering too many objects");

	std::vector<glm::mat4> modelMatrices;
	std::vector<glm::vec4> colors;
	std::vector<Sprite> sprites;
	modelMatrices.reserve(MAX_OBJECTS);
	colors.reserve(MAX_OBJECTS);
	sprites.reserve(MAX_OBJECTS);

	for (RenderObject& object : m_GUIObjects) {
		Rectangle& rect = object.rect;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3((rect.x + rect.width / 2) * m_Width, (rect.y + rect.height / 2) * m_Width, 0.0f));
		model = glm::scale(model, glm::vec3(rect.width * m_Width, rect.height * m_Width, 1.0f));
		modelMatrices.push_back(model);

		colors.push_back(object.color.ToVec4());
		sprites.push_back(object.sprite);
	}

	GlCall(glNamedBufferSubData(m_ModelMatrixBuffer, 0, sizeof(glm::mat4) * modelMatrices.size(), modelMatrices.data()));
	GlCall(glNamedBufferSubData(m_ColorBuffer, 0, sizeof(glm::vec4) * colors.size(), colors.data()));
	GlCall(glNamedBufferSubData(m_SpriteBuffer, 0, sizeof(Sprite) * sprites.size(), sprites.data()));

	m_GUIShader.Use();
	m_GUIShader.SetMat4("projection", m_Projection);
	m_GUIShader.SetFloat("transparency", m_Transparency);

	GlCall(glActiveTexture(GL_TEXTURE0));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_TextureAtlas.GetID()));
	m_GUIShader.SetInt("textureAtlas", 0);

	GlCall(glBindVertexArray(m_QuadVAO));
	GlCall(glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_GUIObjects.size()));
	GlCall(glBindVertexArray(0));
}

void Renderer::RenderTextures() {
	for (TextureRenderObject object : m_TextureObjects) {
		Rectangle& rect = object.rect;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3((rect.x + rect.width / 2) * m_Width, (rect.y + rect.height / 2) * m_Width, 0.0f));
		model = glm::scale(model, glm::vec3(rect.width * m_Width, rect.height * m_Width, 1.0f));

		m_TextureShader.Use();
		m_TextureShader.SetMat4("projection", m_Projection);
		m_TextureShader.SetMat4("model", model);
		m_TextureShader.SetVec3("color", object.color.ToVec3());
		m_TextureShader.SetFloat("alpha", m_Transparency);

		GlCall(glActiveTexture(GL_TEXTURE0));
		GlCall(glBindTexture(GL_TEXTURE_2D, object.textureID));
		m_TextureShader.SetInt("tex", 0);

		GlCall(glBindVertexArray(m_QuadVAO));
		GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
		GlCall(glBindVertexArray(0));
	}
}
