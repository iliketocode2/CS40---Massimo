

make clean
make

echo "Starting Tests"
echo "Test 1: UArray2b_new"

valgrind --leak-check=full --show-leak-kinds=all ./a2btest

echo "----------------------------------------------"


