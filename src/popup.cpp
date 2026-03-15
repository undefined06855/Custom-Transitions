#include "shaders.hpp"
#include <Geode/ui/GeodeUI.hpp>

$on_game(Loaded) {
    FLAlertLayer* popup = nullptr;

    std::error_code ec;

    auto path = geode::utils::string::pathToString(geode::Mod::get()->getSettingValue<std::filesystem::path>("image-path"));
    auto texture = cocos2d::CCTextureCache::get()->addImage(path.c_str(), true);

    // i think these are the most arbitrary ive made popup text colours before

    if (g_transitionShaderFailedToLoad) {
        popup = FLAlertLayer::create(
            "Custom Transitions",
            "The <co>transition shader</c> <cr>failed to load</c>!\n\n"
            "Please report this to the mod developer (<cl>@undefined06855</c>), and send your <cy>game logs</c>.",
            "ok"
        );
    } else if (!geode::Mod::get()->getSavedValue<bool>("shown-popup", false)) {
        geode::Mod::get()->setSavedValue<bool>("shown-popup", true);
        popup = geode::createQuickPopup(
            "Custom Transitions",
            "Please go into the <cy>mod settings</c> and select an <co>image</c> "
            "for the mod to use for the transition!\n\n"
            "Greyscale images are <cl>recommended</c>.",
            "Open mod settings", nullptr,
            [](auto, auto) {
                geode::openSettingsPopup(geode::Mod::get(), false);
            },
            false
        );
    } else if (!texture) {
        popup = geode::createQuickPopup(
            "Custom Transitions",
            "The <co>custom image</c> used for the transitions <cr>does not exist</c> (or is invalid)!\n"
            "Please set a <cy>new image</c> in the <cl>mod settings</c>.\n\n"
            "(only <cc>PNG</c>, <ca>JPG</c> and <cp>TIFF</c> files are allowed)",
            "Open mod settings", nullptr,
            [](auto, auto) {
                geode::openSettingsPopup(geode::Mod::get(), false);
            },
            false
        );
    }

    if (!popup) return;

    popup->m_scene = MenuLayer::get();
    popup->show();
}
