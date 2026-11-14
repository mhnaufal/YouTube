# Pikuma 3D Programming from Scratch

<https://youtu.be/DC9FBRQKNck?si=GohTsfj-d979AQkQ>

```bash
cmake -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build --config RelWithDebInfo

# Conan 2
conan install --profile tools\conanprofiles\win_msvc_194 --build=missing -s build_type=RelWithDebInfo .
conan build --profile tools\conanprofiles\win_msvc_194 --build=missing -s build_type=RelWithDebInfo .
```

HOT RELOAD
