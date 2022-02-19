clear # Clear the terminal

mkdir -p build # Create the build directory

rm -f ./build/* # Delete files in build

gcc main.c ./lib/calculator.c -I./lib -o ./build/main # Compile the main program


a=23; b=5; op="* "; # op can be "+", "-", "//", "* ", "%"

./build/main $a "$op" $b
