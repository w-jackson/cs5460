int b = 3, c = 4, d;

// int linear_transform(int a) {
//     return a * c + b * d;
// }

// int linear_transform(int a) {
//     d = 5;
//     return a * c + b * d;
// }

char s[11] = "1234567890";
int linear_transform(int a) {
    return s[a] - '0' + c * b + d;
}