#!/usr/bin/env bash

# Exit immediately if any command below fails.
set -e

make


echo "Generating a test_files directory.."
mkdir -p test_files
rm -f test_files/*


echo "Generating test files.."
printf "" > test_files/empty.input
printf "hemmelighed" > test_files/hemmelig_fil.input
chmod -r  test_files/hemmelig_fil.input
printf "Hello, World!\n" > test_files/ascii.input
printf "Hello, World!" > test_files/ascii2.input
printf "\xAA\xAB\xAC" > test_files/ISO.input
printf "Hello,\xa6\xa6World!\n" > test_files/ISO2.input
printf "Hello,\xC0World!\n" > test_files/ISO3.input
printf "æøå\n" > test_files/ISO4.input
iconv -f UTF-8 -t ISO-8859-1 test_files/ISO4.input -o test_files/ISO4.input
printf "æøå\n" > test_files/UTF.input
printf "\xCE\xBB\n" > test_files/UTF2.input
printf "\xC2\xA9\n" > test_files/UTF3.input
printf "\xE0\xa6\xa6\n" > test_files/UTF4.input
printf "\xF0\xa6\xa6\xa6\n" > test_files/UTF5.input
printf "\xF0\xBF\xBF\xBF\n" > test_files/UTF6.input
printf "\x00" > test_files/UTF00.input
printf "\x80" > test_files/data.input
printf "\x9B" > test_files/data2.input
printf "\xE0\x80\n" > test_files/data3.input
printf "\xF0\x80\n" > test_files/data4.input
printf "\xF0\x80\x80\x80\n" > test_files/IS_UTF.input
printf "\xE0\x80\x80\n" > test_files/IS_UTF2.input


echo "Running the tests.."
exitcode=0
for f in test_files/*.input
do
  echo ">>> Testing ${f}.."
  file    ${f} | sed -e 's/ASCII text.*/ASCII text/' \
                         -e 's/UTF-8 Unicode text.*/UTF-8 Unicode text/' \
                         -e 's/ISO-8859 text.*/ISO-8859 text/' \
                         -e 's/writable, regular file, no read permission/cannot determine (Permission denied)/' \
                         > "${f}.expected"
  ./file  "${f}" > "${f}.actual"

  if ! diff -u "${f}.expected" "${f}.actual"
  then
    echo ">>> Failed :-("
    exitcode=1
  else
    echo ">>> Success :-)"
  fi
done
exit $exitcode
