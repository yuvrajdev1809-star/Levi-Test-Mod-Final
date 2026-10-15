#include "mod/MyMod.h"

#include <filesystem>

#include <pl/Mod.hpp>
#include <pl/ModMenu.hpp>

namespace clange_me {

ClangeMeMod &ClangeMeMod::instance() {
    return []() -> ClangeMeMod & {
        return *new ClangeMeMod();
    }();
}

ClangeMeMod::ClangeMeMod()
    : mSelf(*ll::mod::NativeMod::current()) {
    mSelf.getLogger().info("[CLANGE_TRACE] Constructor entered");
    mSelf.getLogger().info(
        "[CLANGE_TRACE] Mod ID: {}",
        mSelf.getId());
    mSelf.getLogger().info(
        "[CLANGE_TRACE] Mod name: {}",
        mSelf.getName());
}

bool ClangeMeMod::load() {
    auto &self = getSelf();

    self.getLogger().info("[CLANGE_TRACE] ===== LOAD BEGIN =====");

    self.getLogger().info(
        "[CLANGE_TRACE] Data directory: {}",
        self.getDataDir().string());

    self.getLogger().info(
        "[CLANGE_TRACE] Config directory: {}",
        self.getConfigDir().string());

    self.getLogger().info(
        "[CLANGE_TRACE] Mod directory: {}",
        self.getModDir().string());

    std::error_code ec;

    self.getLogger().info(
        "[CLANGE_TRACE] Creating data directory");

    std::filesystem::create_directories(self.getDataDir(), ec);

    if (ec) {
        self.getLogger().error(
            "[CLANGE_TRACE] Data directory creation FAILED: {}",
            ec.message());
        return false;
    }

    self.getLogger().info(
        "[CLANGE_TRACE] Data directory OK");

    ec.clear();

    self.getLogger().info(
        "[CLANGE_TRACE] Creating config directory");

    std::filesystem::create_directories(self.getConfigDir(), ec);

    if (ec) {
        self.getLogger().error(
            "[CLANGE_TRACE] Config directory creation FAILED: {}",
            ec.message());
        return false;
    }

    self.getLogger().info(
        "[CLANGE_TRACE] Config directory OK");

    self.getLogger().info(
        "[CLANGE_TRACE] Constructing ConfigFile");

    mConfigFile.emplace();

    self.getLogger().info(
        "[CLANGE_TRACE] ConfigFile constructed");

    self.getLogger().info(
        "[CLANGE_TRACE] Loading typed config");

    if (!mConfigFile->load()) {
        self.getLogger().error(
            "[CLANGE_TRACE] Typed config LOAD FAILED");
        mConfigFile.reset();
        return false;
    }

    self.getLogger().info(
        "[CLANGE_TRACE] Typed config loaded");

    mConfig = mConfigFile->value();

    self.getLogger().info(
        "[CLANGE_TRACE] Config value copied");

    self.getLogger().info(
        "[CLANGE_TRACE] ===== LOAD END =====");

    return true;
}

bool ClangeMeMod::enable() {
    auto &self = getSelf();

    self.getLogger().info("[CLANGE_TRACE] ===== ENABLE BEGIN =====");

    self.getLogger().info(
        "[CLANGE_TRACE] About to construct ModuleBuilder");

    auto builder =
        pl::modmenu::ModuleBuilder(
            "clange_me.hello",
            "It's Working GNG");

    self.getLogger().info(
        "[CLANGE_TRACE] ModuleBuilder constructed");

    self.getLogger().info(
        "[CLANGE_TRACE] About to call modId()");

    builder.modId(self.getId());

    self.getLogger().info(
        "[CLANGE_TRACE] modId() completed");

    self.getLogger().info(
        "[CLANGE_TRACE] About to call description()");

    builder.description(
        "If you see this, the mod loaded.");

    self.getLogger().info(
        "[CLANGE_TRACE] description() completed");

    self.getLogger().info(
        "[CLANGE_TRACE] About to call defaultEnabled()");

    builder.defaultEnabled(true);

    self.getLogger().info(
        "[CLANGE_TRACE] defaultEnabled() completed");

    self.getLogger().info(
        "[CLANGE_TRACE] About to call config()");

    builder.config(
        "opacity",
        "Opacity",
        pl::modmenu::ConfigType::SliderInt,
        "80",
        "0",
        "100");

    self.getLogger().info(
        "[CLANGE_TRACE] config() completed");

    self.getLogger().info(
        "[CLANGE_TRACE] About to call registerModule()");

    const bool ok = builder.registerModule();

    self.getLogger().info(
        "[CLANGE_TRACE] registerModule() returned: {}",
        ok);

    if (ok) {
        self.getLogger().info(
            "[CLANGE_TRACE] ===== ENABLE SUCCESS =====");
    } else {
        self.getLogger().error(
            "[CLANGE_TRACE] ===== ENABLE REGISTRATION FAILED =====");
    }

    return true;
}

bool ClangeMeMod::disable() {
    auto &self = getSelf();

    self.getLogger().info(
        "[CLANGE_TRACE] ===== DISABLE BEGIN =====");

    self.getLogger().info(
        "[CLANGE_TRACE] About to unregister module");

    pl::modmenu::unregisterModule("clange_me.hello");

    self.getLogger().info(
        "[CLANGE_TRACE] unregisterModule() completed");

    self.getLogger().info(
        "[CLANGE_TRACE] ===== DISABLE END =====");

    return true;
}

bool ClangeMeMod::unload() {
    auto &self = getSelf();

    self.getLogger().info(
        "[CLANGE_TRACE] ===== UNLOAD BEGIN =====");

    self.getLogger().info(
        "[CLANGE_TRACE] Resetting ConfigFile");

    mConfigFile.reset();

    self.getLogger().info(
        "[CLANGE_TRACE] ConfigFile reset");

    self.getLogger().info(
        "[CLANGE_TRACE] ===== UNLOAD END =====");

    return true;
}

} // namespace clange_me