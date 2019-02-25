CC="g++ -O3"

grep -E "(long|int|bool|double) [a-zA-Z]*\(const Graph ?& ?g\)" invariants.hpp |\
  sed 's/ *[a-z]* \([a-zA-Z]*\)(.*/\1/' | while read inv
do
  echo $inv
  $CC output_inv_mono.cpp -DINV=$inv -o invariants_mono/$inv
done
