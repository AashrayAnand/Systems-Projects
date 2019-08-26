// Copyright 2019 Aashray Anand
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::streamsize;
using std::numeric_limits;

void output_factors(int num) {
  int i;
  for (i = 1; i <= num / 2; i++) {
    if (num % i == 0) {
      cout << i << endl;
    }
  }
}

int main(int argc, char **argv) {
  int num;
  cout << "Enter a number, I will output all positive factors" << endl;
  // failed extraction of proper data will result in stream
  // evaluating to false, can use this while loop to ensure
  // input is an integer
  while (!(cin >> num)) {
    cout << "Please enter an integer!" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }

  output_factors(num);
}
