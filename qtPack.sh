
#! /bin/sh

#./qtenv.sh

#exe="simulator"
#des="/media/xlink/src/source_code/XlinkDeviceSimulator/qt/build-simulator-Desktop_Qt_5_12_1_GCC_64bit-Release/release"

#deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')

#cp $deplist $des

build_day=$(date +%Y%m%d)

RELEASE_DIR="./linux-qcom"
SRC_EXE_DIR="./build-*-Release"
OUTPUT_DIR="../output"
EXE="qcom"  

mkdir -p $RELEASE_DIR
mkdir -p $OUTPUT_DIR

cp -rf $SRC_EXE_DIR/$EXE $RELEASE_DIR/

cd $RELEASE_DIR

envpath="/opt/Qt/5.12.2"
basepath=$(cd `dirname $0`; pwd)

source /etc/profile

export PATH=${envpath}/gcc_64/bin:$PATH
export LD_LIBRARY_PATH=${envpath}/gcc_64/lib/:$LD_LIBRARY_PATH

echo "FILE PATH IS:"
echo $basepath

echo "BIN PATH IS:"
printenv PATH

echo "LD_LIBRARY_PATH IS:"
printenv LD_LIBRARY_PATH

linuxdeployqt-5-x86_64.AppImage $EXE -appimage

# cp ../../CHANGELOG.md .

cd ../

# zip -r $OUTPUT_DIR/$build_day-qt-simulator.zip $RELEASE_DIR



