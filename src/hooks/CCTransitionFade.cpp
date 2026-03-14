#include "CCTransitionFade.hpp"
#include "../CustomFadeLayer.hpp"

const unsigned int kSceneFade = 0xFADEFADE;

bool HookedCCTransitionFade::initWithDuration(float duration, cocos2d::CCScene* scene, const cocos2d::ccColor3B& color) {
    duration = geode::Mod::get()->getSettingValue<float>("duration");
    return CCTransitionFade::initWithDuration(duration, scene, color);
}

// taken from cocos but reformatted and changed cclayercolor to customfadelayer
void HookedCCTransitionFade::onEnter() {
    CCTransitionScene::onEnter();

    auto layer = CustomFadeLayer::create(m_tColor);
    m_pInScene->setVisible(false);

    this->addChild(layer, 2, kSceneFade);

    layer->runAction(cocos2d::CCSequence::create(
        cocos2d::CCFadeIn::create(m_fDuration / 2.f),
        cocos2d::CCCallFunc::create(this, callfunc_selector(CCTransitionScene::hideOutShowIn)),
        cocos2d::CCFadeOut::create(m_fDuration / 2.f),
        cocos2d::CCCallFunc::create(this, callfunc_selector(CCTransitionScene::finish)),
        nullptr
    ));
}
