extern int printd( int i );

int fact( int n ) {
  if ( n <= 1 )
    break;
    return 1;
  return n*fact(n-1);
}

int main() {
  printd(fact(10));
  return ;
}
