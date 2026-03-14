#include "CustomFadeLayer.hpp"

CustomFadeLayer* CustomFadeLayer::create(const cocos2d::ccColor4B& color) {
    auto ret = new CustomFadeLayer;
    if (ret->initWithColor(color)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool CustomFadeLayer::initWithColor(const cocos2d::ccColor4B& color) {
    if (!CCLayerColor::initWithColor(color)) return false;

    this->setShaderProgram(cocos2d::CCShaderCache::sharedShaderCache()->programForKey("transition_fade_shader"_spr));

    auto path = geode::utils::string::pathToString(geode::Mod::get()->getSettingValue<std::filesystem::path>("image-path"));
    m_texture = cocos2d::CCTextureCache::get()->addImage(path.c_str(), true);

    if (!m_texture) {
        m_edgeSize = -1; // show pink
    } else {
        m_edgeSize = geode::Mod::get()->getSettingValue<double>("edge-size");
    }

    m_pSquareTexCoords[0] = { 0.f, 1.f };
    m_pSquareTexCoords[1] = { 1.f, 1.f };
    m_pSquareTexCoords[2] = { 0.f, 0.f };
    m_pSquareTexCoords[3] = { 1.f, 0.f };

    return true;
}

void CustomFadeLayer::draw() {
    CC_NODE_DRAW_SETUP();

    cocos2d::ccGLEnableVertexAttribs(cocos2d::kCCVertexAttribFlag_PosColorTex);

    glVertexAttribPointer(cocos2d::kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, m_pSquareVertices);
    glVertexAttribPointer(cocos2d::kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, m_pSquareTexCoords);
    glVertexAttribPointer(cocos2d::kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 0, m_pSquareColors);

    if (m_texture) cocos2d::ccGLBindTexture2D(m_texture->getName());
    cocos2d::ccGLBlendFunc(m_tBlendFunc.src, m_tBlendFunc.dst);

    auto loc = getShaderProgram()->getUniformLocationForName("u_edgeSize");
    getShaderProgram()->setUniformLocationWith1f(loc, m_edgeSize);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // CC_INCREMENT_GL_DRAWS(1);
}
