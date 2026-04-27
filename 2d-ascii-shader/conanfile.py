from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout


class AsciiShaderConan(ConanFile):
    name = "2d-ascii-shader"
    version = "0.0.1"
    settings = "os", "compiler", "build_type", "arch"
    user = "mhnaufal"
    channel = "stable"

    def requirements(self):
        self.requires("ftxui/6.1.9")

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
