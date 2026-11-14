from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout


class BelajarVulkanConan(ConanFile):
    name = "belajar-vulkan"
    version = "0.1.0"
    settings = "os", "compiler", "build_type", "arch"
    user = "mhnaufal"
    channel = "stable"

    def requirements(self):
        self.requires("sdl/3.4.16")
        self.requires("glm/1.0.3")
        self.requires("spdlog/1.17.0")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
