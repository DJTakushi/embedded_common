# TODO : install versions used in alpine (or don't and mention potential conflicts)
sudo apt install -y libssl-dev=3.0.13-0ubuntu3.4
sudo apt-get install -y libcurl4-gnutls-dev=8.5.0-2ubuntu10.5
sudo apt-get install -y uuid-dev=2.39.3-9ubuntu6.1

sudo apt-add-repository ppa:mosquitto-dev/mosquitto-ppa -y
sudo apt-get update
sudo apt-get install -y mosquitto-dev=2.0.20* # TODO: get library, but don't run

mkdir -p third_party

cd third_party
sudo apt-get install -y xsltproc docbook-xsl
git clone  git clone git@github.com:eclipse-mosquitto/mosquitto.git --branch v2.0.20
cd mosquitto
mkdir -p build && cd build
sudo cmake .. && sudo make install
cd ../..

wget https://boostorg.jfrog.io/artifactory/main/release/1.84.0/source/boost_1_84_0.tar.bz2
tar --bzip2 -xf boost_1_84_0.tar.bz2
sudo cp -r boost_1_84_0/boost /usr/include/
cd ..

git clone https://github.com/nlohmann/json.git --branch v3.11.3 third_party/json
sudo cp -r third_party/json/include/* /usr/include/

git clone https://github.com/Azure/azure-iot-sdk-c.git --branch 1.14.0 third_party/azure-iot-sdk-c
mkdir -p third_party/azure-iot-sdk-c/cmake
cd third_party/azure-iot-sdk-c/cmake
git submodule init && git submodule update
cmake -Duse_edge_modules=ON ..
sudo make install
cd ../../../

# NOTE : this installation is not identical to dockerfile
cd third_party
git clone https://github.com/eclipse/tahu.git --branch v1.0.11
cd tahu/c/core
sed -i "s|#define SPARKPLUG_DEBUG 1|//#define SPARKPLUG_DEBUG 1 |g"
    \ include/tahu.h
make
sudo cp include/* /usr/include/
sudo cp lib/libtahu.a /usr/lib/