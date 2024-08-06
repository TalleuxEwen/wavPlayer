#install dep, if it is ubuntu it is apt, if it is fedora it is dnf
if [ "$(uname)" != "Darwin" ]; then
  OS_NAME=$(lsb_release -si)

  # Check if the distribution is Debian-based (uses apt)
  if test "$OS_NAME" = "Ubuntu" -o "$OS_NAME" = "Debian"; then
    sudo apt-get install libpulse-dev autoconf automake libtool
  elif test "$OS_NAME" = "Fedora"; then
    sudo dnf install pulseaudio-libs-devel autoconf automake libtool alsa-lib-devel
  elif
    test "$OS_NAME" = "Darwin"; then
    echo "MacOS"
  else
    if test "$(uname)" = "Linux"; then
      sudo dnf install pulseaudio-libs-devel autoconf automake libtool
    else

      echo "Unsupported distribution: $OS_NAME"
      exit 1
    fi
  fi
fi
git clone https://github.com/thestk/rtaudio.git
cd rtaudio
if [ "$(uname)" != "Darwin" ]; then
  ./autogen.sh
  ./configure --with-pulse
fi
mkdir build
cd build
cmake ..
make
sudo cmake --install .
cd ../..
mkdir build
cd build
cmake ..
make
mv wavPlayer ..
cd ..
sudo rm -rf rtaudio
sudo rm -rf build
echo "Done"
