
git init
git config --global user.name "useridid"
git config --global user.email "email"

git clone -b 2.4.10.x-prep https://github.com/rxl194/opencv.git ./ocv2.4.10

mkdir -p Ubuntu/Fedora
cd Ubuntu/Fedora
csh ../csh_make