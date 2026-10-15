#include "mod/MyMod.h"

#include <filesystem>

#include <pl/Mod.hpp>
#include <pl/ModMenu.hpp>

namespace clange_me {

namespace {

constexpr char kModuleId[] = "clange_me.hello";

} // namespace

ClangeMeMod &ClangeMeMod::instance() {
    static ClangeMeMod instance;
    return instance;
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

    // Register the Mod Menu module during load(), matching the
    // official LeviLaunchroid C++ example.
    self.getLogger().info(
        "[CLANGE_TRACE] About to register Mod Menu module");

    const bool moduleRegistered =
        pl::modmenu::ModuleBuilder(
            kModuleId,
            "It's Working GNG")
            .modId(self.getId())
            .description("If you see this, the mod loaded.")
            .defaultEnabled(true)
            .config(
                "opacity",
                "Opacity",
                pl::modmenu::ConfigType::SliderInt,
                "80",
                "0",
                "100")
            .registerModule();

    self.getLogger().info(
        "[CLANGE_TRACE] registerModule() returned: {}",
        moduleRegistered);

    if (!moduleRegistered) {
        self.getLogger().error(
            "[CLANGE_TRACE] Mod Menu registration FAILED");
        return false;
    }

    self.getLogger().info(
        "[CLANGE_TRACE] Mod Menu registration SUCCEEDED");

    self.getLogger().info(
        "[CLANGE_TRACE] ===== LOAD END =====");

    self.getLogger().info(
        "Loaded {} from {}",
        self.getName(),
        self.getModDir().string());

    return true;
}

bool ClangeMeMod::enable() {
    auto &self = getSelf();

    self.getLogger().info(
        "[CLANGE_TRACE] ===== ENABLE BEGIN =====");

    self.getLogger().info(
        "[CLANGE_TRACE] enable() reached successfully");

    self.getLogger().info(
        "[CLANGE_TRACE] ===== ENABLE END =====");

    return true;
}

bool ClangeMeMod::disable() {
    auto &self = getSelf();

    self.getLogger().info(
        "[CLANGE_TRACE] ===== DISABLE BEGIN =====");

    self.getLogger().info(
        "[CLANGE_TRACE] About to unregister Mod Menu module");

    pl::modmenu::unregisterModule(kModuleId);

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