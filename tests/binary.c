/* This is a pretty involved program.  I hope it's actually correct.  Please report errors, if you can find any. */

int power2 (int);

int main(int dummy) 
{
  int n, c, k;
  int aux1, aux2;
 
  write "Enter an integer in decimal number system\n"; /* There's no official support for special characters in strings. but you need not check for that. */
  
  read n;

  write "2 to the n is ", power2(n);
 
  write "in binary number system is:\n",n;
 
  c = 31;

  while (c > 0)
  {
    /* k = n >> c;  there is no >> in our language; so we try to fake it */
    k = n / power2(c);
    
    if (k > 0)
      write "1";
    else
      write "0";

      c = c-1;
  }
 
  write "\n";
 
  return 0;
}

int power2 (int power) /* compute 2 rased to power POWER; assumes POWER>=0 */
{
  int count;
  int result;

  /* check assumptions on input and complain. */
  if (power < 0)
    write "Do not know how to compute negative powers, was given ", power;
      
  count = 0;
  result = 1;

  /* is there a one-off error here? I hope not */
  while (count<power)
    {
      result = result * 2;
      count = count + 1;
    }

  return result;
}
