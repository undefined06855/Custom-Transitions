#include <asp/fs.hpp>

$on_mod(Loaded) {
    std::error_code ec;
    std::filesystem::remove_all(geode::Mod::get()->getConfigDir().string(), ec);
    if (ec) {
        geode::log::warn("Could not remove config dir: {} ({})", ec.message(), ec.value());
    }

    std::filesystem::copy(geode::Mod::get()->getResourcesDir().string(), geode::Mod::get()->getConfigDir().string(), ec);
    if (ec) {
        geode::log::warn("Could not copy transition examples: {} ({})", ec.message(), ec.value());
    }
}
