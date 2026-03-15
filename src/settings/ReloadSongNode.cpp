#include "ReloadSongNode.hpp"
#include "ReloadSongSetting.hpp"

#ifdef GEODE_IS_ANDROID
#include <Geode/cocos/platform/android/jni/JniHelper.h>
#endif

ReloadSongNode* ReloadSongNode::create(std::shared_ptr<ReloadSongSetting> setting, float width) {
    auto ret = new ReloadSongNode;
    if (ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool ReloadSongNode::init(std::shared_ptr<ReloadSongSetting> setting, float width) {
    if (!SettingNodeV3::init(setting, width)) return false;

    auto spr = geode::BasedButtonSprite::create(
        cocos2d::CCSprite::createWithSpriteFrameName("gj_folderBtn_001.png"),
        geode::BaseType::Circle,
        (int)geode::CircleBaseSize::Small,
        (int)geode::CircleBaseColor::Green
    );
    spr->setScale(.6f);
    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ReloadSongNode::onButton));
    getButtonMenu()->addChildAtPosition(btn, geode::Anchor::Center);

    updateState(nullptr);

    return true;
}

void ReloadSongNode::onButton(cocos2d::CCObject* sender) {
#ifdef GEODE_IS_ANDROID
    auto info = cocos2d::JniMethodInfo();
    if (!cocos2d::JniHelper::getStaticMethodInfo(info, "com/geode/launcher/utils/GeodeUtils", "getLauncherVersion", "()I")) {
        geode::log::warn("failed to get launcher version");
        geode::utils::file::openFolder(geode::Mod::get()->getConfigDir());
        return;
    }

    int ret = info.env->CallStaticIntMethod(info.classID, info.methodID);
    info.env->DeleteLocalRef(info.classID);

    if (ret < 26) {
        geode::createQuickPopup(
            "Custom Transitions",
            "Your <cy>android launcher</c> is <co>too outdated</c> to be able to open the folder!\n"
            "If there is an update, <cj>install it</c>, else wait for the next launcher update to be released.\n\n"
            "You can manually navigate to the folder by going to <cg>game/geode/config/undefined0.custom-transitions</c>.",
            "ok", nullptr,
            [](auto, auto) {
                geode::utils::file::openFolder(geode::Mod::get()->getConfigDir());
            }
        );
    } else {
        geode::utils::file::openFolder(geode::Mod::get()->getConfigDir());
    }
#else
    geode::utils::file::openFolder(geode::Mod::get()->getConfigDir());
#endif
}

void ReloadSongNode::onCommit() {}
void ReloadSongNode::onResetToDefault() {}
bool ReloadSongNode::hasUncommittedChanges() const { return false; }
bool ReloadSongNode::hasNonDefaultValue() const { return false; }

$on_mod(Loaded) {
    (void)geode::Mod::get()->registerCustomSettingType("reload-song-button", &ReloadSongSetting::parse);
}
