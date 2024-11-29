#include <iostream> 
#include <iomanip>
using namespace std; // For cin, cout, fixed, setprecision, setw

// Function to print a real number with a specified field width and precision
void print_real(float number, int fieldspace, int precision) { 
    cout << fixed << setprecision(precision) << setw(fieldspace) << number << endl; 
}

// Function to update two real numbers by scaling them by a factor of 10
template<typename T> // Template to allow the function to accept any data type
void update_scale(T& m1, T& m2, T m3 = 10) { // Default value of m3 is 10
    T original_m1 = m1; // Store the original value of m1
    T original_m2 = m2; // Store the original value of m2
    m1 = (original_m1 + original_m2) * m3; // Update m1
    m2 = (original_m1 - original_m2) * m3; // Update m2
}

// Function to update two real numbers by scaling them by a factor of 10
int main() {
    float a, b; // Declare two real numbers

    cout << "Enter two numbers: "; // Prompt the user to enter two numbers
    cin >> a >> b; // Read the two numbers

    cout << "Entered Numbers:\n"; // Display the numbers entered by the user
    print_real(a, 7, 3); // Print the first number with a field width of 7 and precision of 3
    print_real(b, 7, 3);// Print the second number with a field width of 7 and precision of 3

    update_scale(a, b); // Update the numbers by scaling them by a factor of 10

    cout << "New numbers:\n"; // Display the updated numbers
    print_real(a, 7, 3); // Print the first updated number with a field width of 7 and precision of 3
    print_real(b, 7, 3); // Print the second updated number with a field width of 7 and precision of 3

    return 0; // Return 0 to indicate successful completion
}