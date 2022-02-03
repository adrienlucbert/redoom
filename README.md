# ReDoom Engine

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)
[![C/C++ CI](https://github.com/adrienlucbert/redoom/actions/workflows/main.yml/badge.svg)](https://github.com/adrienlucbert/redoom/actions/workflows/main.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/adrienlucbert/redoom/badge?s=ab605ed6ac504fc638d3fc42ff109336e4cb4d84)](https://www.codefactor.io/repository/github/adrienlucbert/redoom)
![Lines of code](https://img.shields.io/tokei/lines/github/adrienlucbert/redoom)
![Files count](https://img.shields.io/github/directory-file-count/adrienlucbert/redoom)

## 💡 Idea

ReDoom game engine is an unpretentious side project, conducted for learning
purposes. The main goal is to develop a game engine from the ground up, bundling
as many interesting features as possible, for me to learn as much as possible
about the different concerned fields:

- Computer graphics
- Spatialized audio
- Game physics
- Networking
- Artificial intelligence and predefined behaviours
- Debugging, software profiling, benchmarks, optimization
- ...

There is no fixed end goal, as this project aims to be a playground for any idea
I would be interested in trying. However an unofficial goal is to recreate at
some point level of [the first Doom game](https://en.wikipedia.org/wiki/Doom_(1993_video_game)),
mostly as a tribute to this game which is considered, along with its older
brother Wolfenstein 3D, to be the grandfather of first-person-shooter games.

## 🗺️ Roadmap

As this is a project with no fixed end goal, conducted only for learning
purposes, it also has no clear roadmap. I am developing features to my liking.  
For that reason, I describe herebelow not a complete roadmap, but only the
current status of the project and what I plan to do next, in the short term.

You can also visit the project's [open issues](https://github.com/adrienlucbert/redoom/issues)
where I keep track of improvements I want to bring to my project, as well as the
known bugs or performance issues I want to fix.

<table style="margin:0;width:100%">
  <tr>
    <th>Domain</th>
    <th>Feature</th>
    <th>Status</th>
  </tr>

  <th rowspan=4>Build system / CI</th>
  <tr>
    <td>Use clang-format and clang-tidy to lint and perform static analysis on the code</td>
    <td title="done">:heavy_check_mark:</td>
  </tr>
  <tr>
    <td>Use thread-sanitizer to detect potential data races</td>
    <td title="done">:heavy_check_mark:</td>
  </tr>
  <tr>
    <td>Use <a href="https://github.com/ccache/ccache">ccache</a> and <a href="https://github.com/matus-chochlik/ctcache">ctcache</a> to reduce build times in Github Actions</td>
    <td title="done">:heavy_check_mark:</td>
  </tr>

  <th rowspan=7>Core</th>
  <tr>
    <td>Implement a data-oriented Entity Component System pattern</td>
    <td title="done">:heavy_check_mark:</td>
  </tr>
  <tr>
    <td>Support custom behaviours (as components)</td>
    <td title="done">:heavy_check_mark:</td>
  </tr>
  <tr>
    <td>Perform core allocations via an Object Pool to limit kernel calls</td>
    <td title="done">:heavy_check_mark:</td>
  </tr>
  <tr>
    <td>Perform thread spawns via a Thread Pool to limit kernel calls</td>
    <td title="done">:heavy_check_mark:</td>
  </tr>
  <tr>
    <td>Handle events via an Event Queue (keyboard, mouse)</td>
    <td title="done">:heavy_check_mark:</td>
  </tr>
  <tr>
    <td>Serialize and deserialize scenes</td>
    <td title="done">:heavy_check_mark:</td>
  </tr>

  <th rowspan=2>User interface</th>
  <tr>
    <td>Basic game engine UI and scene visualization/edition</td>
    <td title="planned">📅</td>
  </tr>

  <th rowspan=7>Computer graphics</th>
  <tr>
    <td>Perspective projection camera with free movements</td>
    <td title="done">:heavy_check_mark:</td>
  </tr>
  <tr>
    <td>Render textures</td>
    <td title="done">:heavy_check_mark:</td>
  </tr>
  <tr>
    <td>Render 3D meshes</td>
    <td title="done">:heavy_check_mark:</td>
  </tr>
  <tr>
    <td>Load and render 3D models</td>
    <td title="done">:heavy_check_mark:</td>
  </tr>
  <tr>
    <td>Render fonts</td>
    <td title="planned">📅</td>
  </tr>
  <tr>
    <td>Basic lighting and shadows</td>
    <td title="planned">📅</td>
  </tr>

  <th rowspan=5>Game physics</th>
  <tr>
    <td>Gravity and movement forces</td>
    <td title="planned">📅</td>
  </tr>
  <tr>
    <td>Hitboxes and debug wireframe visualization</td>
    <td title="done">:heavy_check_mark:</td>
  </tr>
  <tr>
    <td>Collision detection</td>
    <td title="planned">📅</td>
  </tr>
  <tr>
    <td>Collision response</td>
    <td title="planned">📅</td>
  </tr>
  <!--
    <td title="planned">📅</td>
    <td title="in progress">🚧</td>
    <td title="done">:heavy_check_mark:</td>
  -->
</table>

## 🔨 Build

To build the project, follow the steps below:

```sh
# Make sure you have the necessary system packages installed
# ⚠️ The following system packages installation commands may change depending on 
# your OS. Below example is for Ubuntu 20.04
sudo apt-get update -qq
sudo apt-get install -y cmake clang ninja-build
sudo apt-get install -y --no-install-recommends libx11-dev libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev libxext-dev libxfixes-dev libgl1-mesa-dev

# clone the repository
git clone git@github.com:adrienlucbert/redoom.git
cd redoom

# clone git submodules
git submodule update --init --recursive

# apply patches (some submodules need patches to work with the project's build system
git apply .patches/system_includes.diff

# create the build directory and generate the build system
mkdir build
cd build
cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++

# build the project
ninja
```

Once the project is built, binaries are located in the following directories:
```
.
└── build/
    └── bin/
        ├── benchmarks/ # various benchmarks done to improve game engine's performance
        ├── tests/ # game enine tests
        ├── examples/ # game examples using the game engine
        └── redoom # game engine binary
```
