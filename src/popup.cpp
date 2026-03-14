#include "shaders.hpp"
#include <Geode/ui/GeodeUI.hpp>

$on_game(Loaded) {
    FLAlertLayer* popup = nullptr;

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
    }

    if (!popup) return;

    popup->m_scene = MenuLayer::get();
    popup->show();
}
