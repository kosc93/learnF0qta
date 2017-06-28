##### NLOPT #####
cd tools/
tar -zxvf nlopt-2.4.2.tar.gz
cd nlopt-2.4.2/
./configure --prefix=/tmp/nlopt-install/
make
make install
cp /tmp/nlopt-install/include/nlopt.h ../../include/
cp /tmp/nlopt-install/include/nlopt.hpp ../../include/
cp /tmp/nlopt-install/lib/libnlopt.a ../../lib/
rm -r /tmp/nlopt-install/
cd ../
rm -r nlopt-2.4.2/
cd ../

##### LIBSVM #####
cd tools/
tar -zxvf libsvm-3.22.tar.gz
cd libsvm-3.22/
make
cp svm-scale ../
cp svm-train ../
cp svm-predict ../
cp tools/grid.py ../
cd ../
rm -r libsvm-3.22/
cd ../

##### Build #####
make all
