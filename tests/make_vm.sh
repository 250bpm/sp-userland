CORE64="http://distro.ibiblio.org/tinycorelinux/4.x/x86/release/distribution_files/core64.gz"
HERE=$PWD
BUILD_DIR="$PWD/build"
DISPLAY="0:0"

if [ ! -n "$1" ]
then
    echo "Usage: `basename $0` kernel_checkout_path"
    exit -1
fi
KERNEL=$1

cd ..
make
cd tests

if [ -e $BUILD_DIR ]
then
    echo "Wiping out old build."
    sudo /bin/rm -Rf $BUILD_DIR
    mkdir $BUILD_DIR
fi

if [ ! -e core64.gz ]
then
    wget $CORE64
fi

cd $BUILD_DIR
zcat ../core64.gz | sudo cpio -i -H newc -d

cd $KERNEL
make clean
make mrproper
make defconfig
echo "CONFIG_SP=y" >> .config
make -j8 bzImage
make modules
sudo make INSTALL_MOD_PATH=$BUILD_DIR modules_install firmware_install

cp $KERNEL/arch/x86_64/boot/bzImage $HERE
cd $HERE/..
sudo cp *_client *_server $BUILD_DIR/bin

cd $BUILD_DIR
sudo find | sudo cpio -o -H newc | gzip -2 > ../spcore64.gz
cd $HERE
echo "Starting VM with display $DISPLAY"
qemu-system-x86_64 -kernel bzImage -initrd spcore64.gz -append nozswap -net nic,vlan=1,model=e1000 -net user,vlan=1 -display vnc=$DISPLAY
