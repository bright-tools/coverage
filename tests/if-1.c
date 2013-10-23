/*
   Part of the Bright coverage tool test suite

   Copyright 2013 John Bailey

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

int
main (void)
{
  int i;
  int t;

  /* The following structure is intended to test the instrumentation of
     non-compount chained "if .. else if .. else" statements */
  if (i == 0)
    t = 1;
  else if (i < 0)
    t = 2;
  else
    t = 3;

  /* Testing "null" statements */
  if (i == 0)
    ;
  else;

  /* Non-compound "then" with no else */
  if (i == 0)
    t = 3;

  /* Non-compound "then" with non-compound looping else */
  if( i == 0 )
    t = 3;
  else
    while( t == 3 ) {
        t = 0;
    }

  if( i == 0 )
    while( t < 4 )
      t++;
  else
    i++;

  if( i == 0 )
    for( i = 0; i < 10 ; i++ )
      if( i > 3 )
          t++;

  if( i == 0 )
  {
      t++;
  } else {
      t++;
  }

  if( i == 0 )
  {
      while( t != 1 ) {
        t = 1;
      }
  } else {
      while( t != 1 ) {
        t = 1;
      }
  }

  return 0;
}
