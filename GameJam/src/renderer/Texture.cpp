#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::filesystem::path& path) {
    stbi_set_flip_vertically_on_load(true);

    i32 nrChannels = 4;
    m_Data = stbi_load(path.string().c_str(), (i32*)&m_Width, (i32*)&m_Height, &nrChannels, 0);

    GlCall(glGenTextures(1, &m_ID));

    GlCall(glBindTexture(GL_TEXTURE_2D, m_ID));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (m_Data) {
        GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data));
        GlCall(glGenerateMipmap(GL_TEXTURE_2D));
    }
    else {
        // TODO: Error
    }
}
