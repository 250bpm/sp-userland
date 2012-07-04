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
    wget http://distro.ibiblio.org/tinycorelinux/4.x/x86/release/distribution_files/core64.gz
    wget http://distro.ibiblio.org/tinycorelinux/4.x/x86/tcz/ncurses-common.tcz
    wget http://distro.ibiblio.org/tinycorelinux/4.x/x86/tcz/ncurses.tcz
    wget http://distro.ibiblio.org/tinycorelinux/4.x/x86/tcz/screen.tcz
fi

unsquashfs -d ncc ncurses-common.tcz
unsquashfs -d nc ncurses.tcz
unsquashfs -d sc screen.tcz

cd $BUILD_DIR
zcat ../core64.gz | sudo cpio -i -H newc -d
sudo cp -R ../ncc/* .
sudo cp -R ../nc/* .
sudo cp -R ../sc/* .

cd $KERNEL
if [ ! -e .config ]
then
    make clean
    make mrproper
    make defconfig
    echo "
CONFIG_SP=y
CONFIG_SQUASHFS=y
CONFIG_SQUASHFS_XATTR=y
CONFIG_SQUASHFS_ZLIB=y
CONFIG_SQUASHFS_LZO=y
CONFIG_SQUASHFS_XZ=y
CONFIG_SQUASHFS_4K_DEVBLK_SIZE=y
CONFIG_SQUASHFS_EMBEDDED=y
CONFIG_SQUASHFS_FRAGMENT_CACHE_SIZE=3
CONFIG_KGDB=y
CONFIG_KGDB_SERIAL_CONSOLE=y
CONFIG_KGDB_TESTS=n
CONFIG_KGDB_LOW_LEVEL_TRAP=n
CONFIG_KGDB_KDB=y
CONFIG_KDB_KEYBOARD=y
CONFIG_HAVE_ARCH_KMEMCHECK=y
" >> .config
fi

make -j8 bzImage
make modules
sudo make INSTALL_MOD_PATH=$BUILD_DIR modules_install firmware_install

cp $KERNEL/arch/x86_64/boot/bzImage $HERE

cd $BUILD_DIR
sudo mkdir home/tc/sp-userland
cd ../..
make clean
make LDFLAGS=-static
cd $BUILD_DIR
sudo cp ../../*_server home/tc/sp-userland
sudo cp ../../*_client home/tc/sp-userland

sudo find | sudo cpio -o -H newc | gzip -2 > ../spcore64.gz
cd $HERE
echo "Starting VM with display $DISPLAY"
qemu-system-x86_64 -kernel bzImage -initrd spcore64.gz -append nozswap -net nic,vlan=1,model=e1000 -net user,vlan=1 -display vnc=$DISPLAY
