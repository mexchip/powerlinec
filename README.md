# powerlinec
A powerline-like prompt inspired by [bronze](https://github.com/reujab/bronze).

![](./preview.png)

`powerlinec` is written as an exercise to create something similar to those fancy [powerline](https://github.com/powerline/powerline) shell prompts in one of my favorite languages: `C`. I wanted to figure out what features from other languages I would miss the most while writing it.

## Installation and configuration
Copy `powerlinec` to a directory in your _path_ and add these lines to your _.bashrc_ file:
```
function _update_ps1() {
	PS1="$(~/bin/powerlinec dir:blue:black git:green:black)"
}

PROMPT_COMMAND="_update_ps1;"
```
It uses the same command line arguments format than [bronze](https://github.com/reujab/bronze), a powerline-like prompt written in `go`: segments are indicated as command line arguments following the format:
```
segment:background:foreground
```

### From source
The git segment makes use of the libgit2 library, `powerlinec` links the static version of libgit2 in an attempt to simplify the installation. As no Linux distribution includes a statically linked version of libgit2 (as far as I can tell), you'll need to compile it yourself or link to a dynamically compiled version.

Having to compile libgit2 when installing `bronze` (see [how to install bronze powerline like prompt on ubuntu](https://www.mexchip.com/en/2017/12/how-to-install-bronze-powerline-like-prompt-on-ubuntu/)), was another motivation to create `powerlinec`, I wanted to check if linking to a static version of libgit2 could avoid the need for that.

#### Compiling libgit2 statically
By default, libgit2 is compiled as a shared library, but it is ready to be compiled statically by setting the BUILD_SHARED_LIBS setting to OFF when invoking cmake. But linking this static libgit2 library is not enough to have a fully self contained executable, as libgit2 requires some additional shared libraries. In order to have a fully contained executable we'd need to link with the static versions of these other libraries or, as we do in the following commands, disable this additional functionality since we don't need it in powerlinec.

The libgit2 library is downloaded to $HOME/libgit2 where powerlinec tries to use it from. This way you don't install the compiled libgit2 to your system directories and can delete it whenever you like without affecting your system. Note that as we disable several libgit2 features, the compiled libgit2 is not useful beyond reading information from local repositories.

The following commands will clone the libgit2 repository to $HOME/libgit2, configure CMake to disable some libgit2 features and compile it statically:
```
cd
git clone https://github.com/libgit2/libgit2.git
cd libgit2
mkdir build
cd build
cmake .. -DBUILD_CLAR=OFF -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release -DPCRE_INCLUDE_DIR=OFF -DTHREADSAFE=OFF -DUSE_BUNDLED_ZLIB=ON -DUSE_GSSAPI=OFF -DUSE_HTTPS=OFF -DUSE_HTTP_PARSER=OFF -DUSE_SSH=OFF
cmake --build .
```
#### Compile powerlinec
After compiling libgit2, enter the following commands into a terminal:
```
cd
git clone https://github.com/mexchip/powerlinec.git
cd powerlinec
make
```
Now follow the [Installation and configuration](#Installation-and-configuration) instructions.

## Segments
Three segments are implemented:
- dir, displays current working directory. Replaces $HOME with an icon.
- git, displays information from the current git repository: branch name, commits ahead/behind, if the branch has modifications and if a file has been staged.
- virtualenv: displays the content of the _$VIRTUAL_ENV_ environment variable, normally defined when running a _python virtual environment_.

## Icons
In order to correctly render the icons used by the dir and git segments, I recommend you to use a font from [Nerd Fonts](https://github.com/ryanoasis/nerd-fonts), otherwise, you'll need to patch your font.

## More segments/features
`powerlinec` works only with bash, and I have implemented only the segments I used the most when I was using `bronze`, if you are using another shell (zshell, fish...) or would like more segments you can make a request, implement it yourself (pull requests are welcome) or you could try one of the following projects:
- [bronze](https://github.com/reujab/bronze), written in go, `powerlinec` is kind of an incomplete clone of this.
- [silver](https://github.com/reujab/silver), written in rust, a new version by the author of `bronze`.
- [powerline-go](https://github.com/justjanne/powerline-go), written in go.
- [powerline-shell](https://github.com/b-ryan/powerline-shell), written in python.
