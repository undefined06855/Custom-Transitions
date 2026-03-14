#include "ReloadSongNode.hpp"
#include "ReloadSongSetting.hpp"

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
    geode::utils::file::openFolder(geode::Mod::get()->getResourcesDir());
}

void ReloadSongNode::onCommit() {}
void ReloadSongNode::onResetToDefault() {}
bool ReloadSongNode::hasUncommittedChanges() const { return false; }
bool ReloadSongNode::hasNonDefaultValue() const { return false; }

$on_mod(Loaded) {
    (void)geode::Mod::get()->registerCustomSettingType("reload-song-button", &ReloadSongSetting::parse);
}
