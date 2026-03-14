#pragma once

class CustomFadeLayer : public cocos2d::CCLayerColor {
public:
    static CustomFadeLayer* create(const cocos2d::ccColor4B& color);
    virtual bool initWithColor(const cocos2d::ccColor4B& color) override;

    cocos2d::ccVertex2F m_pSquareTexCoords[4];
    cocos2d::CCTexture2D* m_texture;
    float m_edgeSize;

    virtual void draw() override;
};
