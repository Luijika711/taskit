g++ -o main main.cpp

mv main /home

touch tasks.txt
mv tasks.txt /home

echo "alias taskit='/home/main'" >> ~/.bashrc
source ~/.bashrc

echo "Installed!"
