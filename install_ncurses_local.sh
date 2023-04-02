wget http://ftp.gnu.org/gnu/ncurses/ncurses-6.4.tar.gz
tar zxvf ncurses-6.4.tar.gz
cd ncurses-6.4
./configure --prefix=$HOME/ncurses --enable-widec

make
make install