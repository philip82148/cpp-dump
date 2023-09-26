set -eu

for f in ./readme/*.cpp; do
    g++ -std=c++17 $f
done
