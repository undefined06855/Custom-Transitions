#pragma once
#include <Geode/modify/CCTransitionFade.hpp>

class $modify(HookedCCTransitionFade, cocos2d::CCTransitionFade) {
    virtual bool initWithDuration(float duration, cocos2d::CCScene* scene, const cocos2d::ccColor3B& color);
    virtual void onEnter();
};
