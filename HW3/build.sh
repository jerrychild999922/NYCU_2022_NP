mkdir build
mkdir efs
make
sudo mount -t efs -o tls fs-0897f74d8ea87c8ee:/ efs
sudo chmod go+rw efs